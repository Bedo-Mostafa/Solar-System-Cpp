#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>

// Raw shaders included here for simplicity of file transfer
const std::string SUN_FRAG_SHADER = R"(
#version 120
uniform vec4 u_color;
void main() {
    vec2 coord = gl_TexCoord[0].xy * 2.0 - 1.0;
    float dist = length(coord);
    float core = smoothstep(0.7, 0.2, dist);
    float corona = smoothstep(1.0, 0.4, dist) * 0.5;
    gl_FragColor = mix(u_color, vec4(1.0, 1.0, 0.8, 1.0), core * 0.8) * (core + corona);
}
)";

const std::string PLANET_FRAG_SHADER = R"(
#version 120
uniform vec4 u_baseColor;
uniform vec2 u_lightDir;
uniform float u_dissolve;

float hash(vec2 p) { return fract(sin(dot(p, vec2(12.9898, 78.233))) * 43758.5453); }
float noise(vec2 p) {
    vec2 i = floor(p); vec2 f = fract(p); f = f*f*(3.0-2.0*f);
    return mix(mix(hash(i), hash(i+vec2(1,0)), f.x), mix(hash(i+vec2(0,1)), hash(i+vec2(1,1)), f.x), f.y);
}

void main() {
    vec2 coord = gl_TexCoord[0].xy * 2.0 - 1.0;
    float dist = length(coord);
    if (dist > 1.0) discard;

    float n = noise(gl_FragCoord.xy * 0.1); 
    if (n < u_dissolve) discard;
    float burnEdge = step(n, u_dissolve + 0.1);
    vec3 burnColor = vec3(1.0, 0.4, 0.0) * burnEdge * 2.0;

    vec3 normal = vec3(coord, sqrt(1.0 - dot(coord, coord)));
    vec3 lightDir3D = normalize(vec3(u_lightDir, 0.5)); 
    float diffuse = max(dot(normal, lightDir3D), 0.0);
    float rim = pow(1.0 - max(dot(normal, vec3(0,0,1)), 0.0), 3.0) * 0.5;
    
    vec3 finalColor = u_baseColor.rgb * (diffuse + 0.1 + rim);
    gl_FragColor = vec4(finalColor + burnColor, 1.0);
}
)";

struct GameResources {
    sf::Shader sunShader;
    sf::Shader planetShader;
    sf::Texture dummyTexture;
    sf::Font font;
    sf::Music bgMusic;
    bool loaded = false;

    void load() {
        if (loaded) return;

        // Create dummy texture
        sf::Image img; img.create(1, 1, sf::Color::White);
        dummyTexture.loadFromImage(img);
        dummyTexture.setSmooth(true);

        // Load Shaders
        sunShader.loadFromMemory(SUN_FRAG_SHADER, sf::Shader::Fragment);
        planetShader.loadFromMemory(PLANET_FRAG_SHADER, sf::Shader::Fragment);
        sunShader.setUniform("texture", sf::Shader::CurrentTexture);
        planetShader.setUniform("texture", sf::Shader::CurrentTexture);

        // --- Load Font from Windows System Folder ---
        // This avoids you needing to download a .ttf file manually
        if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
            std::cerr << "Failed to load Arial font. Make sure it exists in C:/Windows/Fonts/\n";
        }
        // --- Load Music ---
        if (!bgMusic.openFromFile("music.wav")) { 
            // Fallback: Try looking in src folder
            if (!bgMusic.openFromFile("src/music.wav")) {
                std::cerr << "Failed to load music.wav\n";
            }
        } else {
            bgMusic.setLoop(true);
            bgMusic.setVolume(50.0f); // 50% Volume
        }
        
        loaded = true;
    }
};