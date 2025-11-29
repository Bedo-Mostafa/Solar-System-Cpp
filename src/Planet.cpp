#include "Planet.hpp"
#include "Config.hpp"

// Constructor Implementation
Planet::Planet(b2World &world, sf::Vector2f position, GameResources &resources)
    : CelestialBody(nullptr, nullptr),
      m_shader(resources.planetShader),
      m_trail(sf::LineStrip)
{
    // --- CHANGED: Bigger Planets ---
    // Old: 6 to 14 -> New: 15 to 35
    float radius = Utils::randomFloat(15.0f, 35.0f);
    float density = Utils::randomFloat(0.8f, 2.5f);
    m_baseColor = sf::Color(
        (sf::Uint8)Utils::randomFloat(80, 220),
        (sf::Uint8)Utils::randomFloat(80, 220),
        (sf::Uint8)Utils::randomFloat(255, 255));

    // Physics
    b2BodyDef def;
    def.type = b2_dynamicBody;
    def.position = Utils::toMeters(position);
    def.angularDamping = 0.1f;
    def.allowSleep = false;

    // Store pointer for collision
    def.userData.pointer = reinterpret_cast<uintptr_t>(this);

    m_body = world.CreateBody(&def);

    b2CircleShape shape;
    shape.m_radius = radius * METERS_PER_PIXEL;
    b2FixtureDef fixture;
    fixture.shape = &shape;
    fixture.density = density;
    fixture.friction = 0.5f;
    fixture.restitution = 0.0f;
    m_body->CreateFixture(&fixture);

    // Visuals
    auto vShape = std::make_shared<sf::CircleShape>(radius);
    vShape->setOrigin(radius, radius);
    vShape->setFillColor(sf::Color::White);
    vShape->setTexture(&resources.dummyTexture);
    m_shape = vShape;

    // Info Text Setup
    // --- CHANGED: Bigger Text ---
    m_infoText.setFont(resources.font);
    m_infoText.setCharacterSize(16); // Old: 12 -> New: 16
    m_infoText.setFillColor(sf::Color::White);
    m_infoText.setOutlineColor(sf::Color::Black);
    m_infoText.setOutlineThickness(1.0f);
}

void Planet::startDeath()
{
    if (m_state == ALIVE)
    {
        m_state = DYING;
        if (m_body)
        {
            b2Filter filter;
            filter.maskBits = 0;
            m_body->GetFixtureList()->SetFilterData(filter);
            m_body->SetLinearDamping(5.0f);
        }
    }
}

bool Planet::isDead() const { return m_state == DEAD; }
bool Planet::isDying() const { return m_state == DYING; }

void Planet::update(float dt)
{
    if (m_state == DYING)
    {
        m_deathTimer += dt;
        float progress = m_deathTimer / 1.5f;
        if (progress >= 1.0f)
        {
            m_state = DEAD;
            return;
        }

        float scale = 1.0f - progress;
        if (m_shape)
            m_shape->setScale(scale, scale);
        m_dissolveAmount = progress;
    }

    if (m_state == ALIVE)
    {
        sf::Vector2f currentPos = Utils::toPixels(m_body->GetPosition());

        // Trail Logic
        bool shouldAdd = false;
        if (m_trailPoints.empty())
        {
            shouldAdd = true;
        }
        else
        {
            sf::Vector2f last = m_trailPoints.front().position;
            float dx = currentPos.x - last.x;
            float dy = currentPos.y - last.y;
            if ((dx * dx + dy * dy) > 4.0f)
                shouldAdd = true;
        }

        if (shouldAdd)
        {
            m_trailPoints.push_front(sf::Vertex(currentPos, m_baseColor));
            if (m_trailPoints.size() > MAX_TRAIL_POINTS)
                m_trailPoints.pop_back();
        }
        else if (!m_trailPoints.empty())
        {
            m_trailPoints.front().position = currentPos;
        }

        // Info Text Logic
        b2Vec2 vel = m_body->GetLinearVelocity();
        float speed = vel.Length();
        float mass = m_body->GetMass();

        std::stringstream ss;
        ss << std::fixed << std::setprecision(1);
        ss << "M: " << mass << "\n";
        ss << "V: " << speed;

        m_infoText.setString(ss.str());
        m_infoText.setPosition(currentPos.x - 20.f, currentPos.y - 60.f);
    }

    // Rebuild Trail
    m_trail.clear();
    for (size_t i = 0; i < m_trailPoints.size(); ++i)
    {
        sf::Vertex v = m_trailPoints[i];
        float alpha = 255.0f * (1.0f - (float)i / m_trailPoints.size());
        if (m_state == DYING)
            alpha *= (1.0f - m_dissolveAmount);
        v.color.a = static_cast<sf::Uint8>(alpha);
        m_trail.append(v);
    }
}

void Planet::render(sf::RenderWindow &window, sf::Shader *)
{
    window.draw(m_trail);

    if (m_body && m_shape)
    {
        sf::Vector2f sunPos = sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f);
        sf::Vector2f myPos = Utils::toPixels(m_body->GetPosition());
        sf::Vector2f lightDir = sunPos - myPos;

        float len = std::sqrt(lightDir.x * lightDir.x + lightDir.y * lightDir.y);
        if (len > 0)
            lightDir /= len;

        m_shader.setUniform("u_baseColor", sf::Glsl::Vec4(m_baseColor));
        m_shader.setUniform("u_lightDir", lightDir);
        m_shader.setUniform("u_dissolve", m_dissolveAmount);

        CelestialBody::render(window, &m_shader);
    }

    if (!isDying() && !isDead())
    {
        window.draw(m_infoText);
    }
}