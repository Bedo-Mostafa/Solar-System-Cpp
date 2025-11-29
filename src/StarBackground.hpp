#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

class StarBackground {
public:
    StarBackground(float width, float height) {
        m_size = sf::Vector2f(width, height);
        m_rectangle.setSize(m_size);

        // SHARP GALAXY SHADER
        const std::string fragmentShader = R"(
            #version 120
            uniform float u_time;
            uniform vec2 u_resolution;

            float random(vec2 st) {
                return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123);
            }

            void main() {
                // Normalize coordinates
                vec2 st = gl_FragCoord.xy / u_resolution.xy;
                st.x *= u_resolution.x / u_resolution.y;

                // Deep Space Black Background
                vec3 color = vec3(0.0, 0.0, 0.02);

                // --- STAR LAYER 1: Distant Dust (Subtle) ---
                float r = random(st * 10.0); // High frequency
                if (r > 0.90) color += vec3(0.05);

                // --- STAR LAYER 2: Main Stars (Sharp) ---
                float r2 = random(st * 80.0);
                if (r2 > 0.99) {
                    float twinkle = 0.5 + 0.5 * sin(u_time * 3.0 + r2 * 100.0);
                    color += vec3(0.8) * twinkle;
                }

                // --- STAR LAYER 3: Bright Stars (Rare) ---
                float r3 = random(st * 20.0);
                if (r3 > 0.997) {
                    color += vec3(1.0, 0.9, 0.8);
                }

                // --- MILKY WAY BAND (Crisp, not blurry) ---
                // We use a simple distance calculation instead of noise
                float bandPos = st.x + st.y * 0.5;
                float band = sin(bandPos * 3.0 + u_time * 0.01);
                
                // Only add color where the band is, keep it dark
                if (band > 0.9) {
                    color += vec3(0.05, 0.0, 0.1) * (band); 
                }

                gl_FragColor = vec4(color, 1.0);
            }
        )";

        if (!m_shader.loadFromMemory(fragmentShader, sf::Shader::Fragment)) {
            std::cerr << "Failed to load star shader\n";
        }
    }

    void update(float dt) {
        m_time += dt;
        m_shader.setUniform("u_time", m_time);
        m_shader.setUniform("u_resolution", m_size);
    }

    void render(sf::RenderWindow& window) {
        if (m_size.x != window.getSize().x || m_size.y != window.getSize().y) {
            m_size = sf::Vector2f(window.getSize());
            m_rectangle.setSize(m_size);
        }
        window.draw(m_rectangle, &m_shader);
    }

private:
    sf::RectangleShape m_rectangle;
    sf::Shader m_shader;
    sf::Vector2f m_size;
    float m_time = 0.0f;
};