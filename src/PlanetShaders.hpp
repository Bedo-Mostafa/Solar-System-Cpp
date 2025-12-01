#pragma once
#include <string>

// Mercury - Gray, cratered surface
const std::string MERCURY_SHADER = R"(
#version 120
uniform vec4 u_baseColor;
uniform vec2 u_lightDir;
uniform float u_dissolve;
uniform float u_time;

float hash(vec2 p) { return fract(sin(dot(p, vec2(12.9898, 78.233))) * 43758.5453); }
float noise(vec2 p) {
    vec2 i = floor(p); vec2 f = fract(p); f = f*f*(3.0-2.0*f);
    return mix(mix(hash(i), hash(i+vec2(1,0)), f.x), mix(hash(i+vec2(0,1)), hash(i+vec2(1,1)), f.x), f.y);
}

void main() {
    vec2 coord = gl_TexCoord[0].xy * 2.0 - 1.0;
    float dist = length(coord);
    if (dist > 1.0) discard;
    
    // --- DISSOLVE FIX ---
    float n = noise(gl_FragCoord.xy * 0.1);
    if (n < u_dissolve) discard;
    
    vec3 burnColor = vec3(0.0);
    if (u_dissolve > 0.0) {
        float burnEdge = step(n, u_dissolve + 0.1);
        burnColor = vec3(1.0, 0.4, 0.0) * burnEdge * 2.0;
    }
    // --------------------
    
    vec3 normal = vec3(coord, sqrt(1.0 - dot(coord, coord)));
    vec3 lightDir3D = normalize(vec3(u_lightDir, 0.5));
    float diffuse = max(dot(normal, lightDir3D), 0.0);
    
    // Add craters
    float craters = noise(gl_FragCoord.xy * 0.05) * 0.3;
    vec3 color = u_baseColor.rgb * (diffuse * 0.8 + 0.2) * (1.0 - craters);
    
    gl_FragColor = vec4(color + burnColor, 1.0);
}
)";

// Venus - Swirling yellow clouds
const std::string VENUS_SHADER = R"(
#version 120
uniform vec4 u_baseColor;
uniform vec2 u_lightDir;
uniform float u_dissolve;
uniform float u_time;

float hash(vec2 p) { return fract(sin(dot(p, vec2(12.9898, 78.233))) * 43758.5453); }
float noise(vec2 p) {
    vec2 i = floor(p); vec2 f = fract(p); f = f*f*(3.0-2.0*f);
    return mix(mix(hash(i), hash(i+vec2(1,0)), f.x), mix(hash(i+vec2(0,1)), hash(i+vec2(1,1)), f.x), f.y);
}

void main() {
    vec2 coord = gl_TexCoord[0].xy * 2.0 - 1.0;
    float dist = length(coord);
    if (dist > 1.0) discard;
    
    // --- DISSOLVE FIX ---
    float n = noise(gl_FragCoord.xy * 0.1);
    if (n < u_dissolve) discard;
    
    vec3 burnColor = vec3(0.0);
    if (u_dissolve > 0.0) {
        float burnEdge = step(n, u_dissolve + 0.1);
        burnColor = vec3(1.0, 0.4, 0.0) * burnEdge * 2.0;
    }
    // --------------------
    
    vec3 normal = vec3(coord, sqrt(1.0 - dot(coord, coord)));
    vec3 lightDir3D = normalize(vec3(u_lightDir, 0.5));
    float diffuse = max(dot(normal, lightDir3D), 0.0);
    
    // Swirling clouds
    vec2 cloudCoord = gl_FragCoord.xy * 0.02;
    float clouds = noise(cloudCoord + vec2(u_time * 0.1, 0)) * 0.5 + 
                   noise(cloudCoord * 2.0 - vec2(u_time * 0.15, 0)) * 0.25;
    
    vec3 color = u_baseColor.rgb * (diffuse * 0.7 + 0.3 + clouds * 0.2);
    gl_FragColor = vec4(color + burnColor, 1.0);
}
)";

// Earth - Blue oceans, green land, white moving clouds
const std::string EARTH_SHADER = R"(
#version 120
uniform vec4 u_baseColor;
uniform vec4 u_secondaryColor;
uniform vec2 u_lightDir;
uniform float u_dissolve;
uniform float u_time;

float hash(vec2 p) { return fract(sin(dot(p, vec2(12.9898, 78.233))) * 43758.5453); }
float noise(vec2 p) {
    vec2 i = floor(p); vec2 f = fract(p); f = f*f*(3.0-2.0*f);
    return mix(mix(hash(i), hash(i+vec2(1,0)), f.x), mix(hash(i+vec2(0,1)), hash(i+vec2(1,1)), f.x), f.y);
}

void main() {
    vec2 coord = gl_TexCoord[0].xy * 2.0 - 1.0;
    float dist = length(coord);
    if (dist > 1.0) discard;
    
    // --- DISSOLVE FIX ---
    float n = noise(gl_FragCoord.xy * 0.1);
    if (n < u_dissolve) discard;
    
    vec3 burnColor = vec3(0.0);
    if (u_dissolve > 0.0) {
        float burnEdge = step(n, u_dissolve + 0.1);
        burnColor = vec3(1.0, 0.4, 0.0) * burnEdge * 2.0;
    }
    // --------------------
    
    vec3 normal = vec3(coord, sqrt(1.0 - dot(coord, coord)));
    vec3 lightDir3D = normalize(vec3(u_lightDir, 0.5));
    float diffuse = max(dot(normal, lightDir3D), 0.0);
    
    // Land/ocean pattern
    float landMask = step(0.4, noise(gl_FragCoord.xy * 0.03));
    vec3 surface = mix(u_baseColor.rgb, u_secondaryColor.rgb, landMask);
    
    // Moving clouds
    vec2 cloudCoord = gl_FragCoord.xy * 0.025;
    float clouds = noise(cloudCoord + vec2(u_time * 0.2, u_time * 0.05)) * noise(cloudCoord * 1.5 + vec2(u_time * 0.15, 0));
    clouds = smoothstep(0.3, 0.7, clouds);
    
    vec3 color = mix(surface, vec3(1.0, 1.0, 1.0), clouds * 0.6) * (diffuse * 0.8 + 0.2);
    gl_FragColor = vec4(color + burnColor, 1.0);
}
)";

// Mars - Red desert with polar ice caps
const std::string MARS_SHADER = R"(
#version 120
uniform vec4 u_baseColor;
uniform vec2 u_lightDir;
uniform float u_dissolve;
uniform float u_time;

float hash(vec2 p) { return fract(sin(dot(p, vec2(12.9898, 78.233))) * 43758.5453); }
float noise(vec2 p) {
    vec2 i = floor(p); vec2 f = fract(p); f = f*f*(3.0-2.0*f);
    return mix(mix(hash(i), hash(i+vec2(1,0)), f.x), mix(hash(i+vec2(0,1)), hash(i+vec2(1,1)), f.x), f.y);
}

void main() {
    vec2 coord = gl_TexCoord[0].xy * 2.0 - 1.0;
    float dist = length(coord);
    if (dist > 1.0) discard;
    
    // --- DISSOLVE FIX ---
    float n = noise(gl_FragCoord.xy * 0.1);
    if (n < u_dissolve) discard;
    
    vec3 burnColor = vec3(0.0);
    if (u_dissolve > 0.0) {
        float burnEdge = step(n, u_dissolve + 0.1);
        burnColor = vec3(1.0, 0.4, 0.0) * burnEdge * 2.0;
    }
    // --------------------
    
    vec3 normal = vec3(coord, sqrt(1.0 - dot(coord, coord)));
    vec3 lightDir3D = normalize(vec3(u_lightDir, 0.5));
    float diffuse = max(dot(normal, lightDir3D), 0.0);
    
    // Desert variation
    float desert = noise(gl_FragCoord.xy * 0.04) * 0.3;
    
    // Polar ice caps
    float latitude = abs(coord.y);
    float iceCap = smoothstep(0.7, 0.9, latitude);
    
    vec3 color = mix(u_baseColor.rgb * (1.0 + desert), vec3(1.0, 0.95, 0.95), iceCap);
    color *= (diffuse * 0.8 + 0.2);
    
    gl_FragColor = vec4(color + burnColor, 1.0);
}
)";

// Jupiter - Massive gas giant with swirling bands
const std::string JUPITER_SHADER = R"(
#version 120
uniform vec4 u_baseColor;
uniform vec4 u_secondaryColor;
uniform vec2 u_lightDir;
uniform float u_dissolve;
uniform float u_time;

float hash(vec2 p) { return fract(sin(dot(p, vec2(12.9898, 78.233))) * 43758.5453); }
float noise(vec2 p) {
    vec2 i = floor(p); vec2 f = fract(p); f = f*f*(3.0-2.0*f);
    return mix(mix(hash(i), hash(i+vec2(1,0)), f.x), mix(hash(i+vec2(0,1)), hash(i+vec2(1,1)), f.x), f.y);
}

void main() {
    vec2 coord = gl_TexCoord[0].xy * 2.0 - 1.0;
    float dist = length(coord);
    if (dist > 1.0) discard;
    
    // --- DISSOLVE FIX ---
    float n = noise(gl_FragCoord.xy * 0.1);
    if (n < u_dissolve) discard;
    
    vec3 burnColor = vec3(0.0);
    if (u_dissolve > 0.0) {
        float burnEdge = step(n, u_dissolve + 0.1);
        burnColor = vec3(1.0, 0.4, 0.0) * burnEdge * 2.0;
    }
    // --------------------
    
    vec3 normal = vec3(coord, sqrt(1.0 - dot(coord, coord)));
    vec3 lightDir3D = normalize(vec3(u_lightDir, 0.5));
    float diffuse = max(dot(normal, lightDir3D), 0.0);
    
    // Horizontal bands with turbulence
    float bandY = coord.y * 8.0 + noise(gl_FragCoord.xy * 0.02 + vec2(u_time * 0.3, 0)) * 2.0;
    float bands = sin(bandY) * 0.5 + 0.5;
    
    // Great Red Spot
    vec2 spotCenter = vec2(0.3, -0.2);
    float spotDist = length(coord - spotCenter);
    float redSpot = smoothstep(0.25, 0.15, spotDist);
    
    vec3 color = mix(u_baseColor.rgb, u_secondaryColor.rgb, bands);
    color = mix(color, vec3(0.8, 0.3, 0.2), redSpot * 0.8);
    color *= (diffuse * 0.7 + 0.3);
    
    gl_FragColor = vec4(color + burnColor, 1.0);
}
)";

// Saturn - Pale gold with subtle bands
const std::string SATURN_SHADER = R"(
#version 120
uniform vec4 u_baseColor;
uniform vec4 u_secondaryColor;
uniform vec2 u_lightDir;
uniform float u_dissolve;
uniform float u_time;

float hash(vec2 p) { return fract(sin(dot(p, vec2(12.9898, 78.233))) * 43758.5453); }
float noise(vec2 p) {
    vec2 i = floor(p); vec2 f = fract(p); f = f*f*(3.0-2.0*f);
    return mix(mix(hash(i), hash(i+vec2(1,0)), f.x), mix(hash(i+vec2(0,1)), hash(i+vec2(1,1)), f.x), f.y);
}

void main() {
    vec2 coord = gl_TexCoord[0].xy * 2.0 - 1.0;
    float dist = length(coord);
    if (dist > 1.0) discard;
    
    // --- DISSOLVE FIX ---
    float n = noise(gl_FragCoord.xy * 0.1);
    if (n < u_dissolve) discard;
    
    vec3 burnColor = vec3(0.0);
    if (u_dissolve > 0.0) {
        float burnEdge = step(n, u_dissolve + 0.1);
        burnColor = vec3(1.0, 0.4, 0.0) * burnEdge * 2.0;
    }
    // --------------------
    
    vec3 normal = vec3(coord, sqrt(1.0 - dot(coord, coord)));
    vec3 lightDir3D = normalize(vec3(u_lightDir, 0.5));
    float diffuse = max(dot(normal, lightDir3D), 0.0);
    
    // Subtle horizontal bands
    float bandY = coord.y * 6.0 + noise(gl_FragCoord.xy * 0.015) * 0.5;
    float bands = sin(bandY) * 0.15 + 0.85;
    
    vec3 color = u_baseColor.rgb * bands * (diffuse * 0.8 + 0.2);
    gl_FragColor = vec4(color + burnColor, 1.0);
}
)";

// Uranus - Smooth cyan ice giant
const std::string URANUS_SHADER = R"(
#version 120
uniform vec4 u_baseColor;
uniform vec2 u_lightDir;
uniform float u_dissolve;
uniform float u_time;

float hash(vec2 p) { return fract(sin(dot(p, vec2(12.9898, 78.233))) * 43758.5453); }
float noise(vec2 p) {
    vec2 i = floor(p); vec2 f = fract(p); f = f*f*(3.0-2.0*f);
    return mix(mix(hash(i), hash(i+vec2(1,0)), f.x), mix(hash(i+vec2(0,1)), hash(i+vec2(1,1)), f.x), f.y);
}

void main() {
    vec2 coord = gl_TexCoord[0].xy * 2.0 - 1.0;
    float dist = length(coord);
    if (dist > 1.0) discard;
    
    // --- DISSOLVE FIX ---
    float n = noise(gl_FragCoord.xy * 0.1);
    if (n < u_dissolve) discard;
    
    vec3 burnColor = vec3(0.0);
    if (u_dissolve > 0.0) {
        float burnEdge = step(n, u_dissolve + 0.1);
        burnColor = vec3(1.0, 0.4, 0.0) * burnEdge * 2.0;
    }
    // --------------------
    
    vec3 normal = vec3(coord, sqrt(1.0 - dot(coord, coord)));
    vec3 lightDir3D = normalize(vec3(u_lightDir, 0.5));
    float diffuse = max(dot(normal, lightDir3D), 0.0);
    
    // Very smooth with minimal features
    float smoothVar = noise(gl_FragCoord.xy * 0.02) * 0.1;
    vec3 color = u_baseColor.rgb * (1.0 + smoothVar) * (diffuse * 0.75 + 0.25);
    
    gl_FragColor = vec4(color + burnColor, 1.0);
}
)";

// Neptune - Deep blue with storm systems
const std::string NEPTUNE_SHADER = R"(
#version 120
uniform vec4 u_baseColor;
uniform vec2 u_lightDir;
uniform float u_dissolve;
uniform float u_time;

float hash(vec2 p) { return fract(sin(dot(p, vec2(12.9898, 78.233))) * 43758.5453); }
float noise(vec2 p) {
    vec2 i = floor(p); vec2 f = fract(p); f = f*f*(3.0-2.0*f);
    return mix(mix(hash(i), hash(i+vec2(1,0)), f.x), mix(hash(i+vec2(0,1)), hash(i+vec2(1,1)), f.x), f.y);
}

void main() {
    vec2 coord = gl_TexCoord[0].xy * 2.0 - 1.0;
    float dist = length(coord);
    if (dist > 1.0) discard;
    
    // --- DISSOLVE FIX ---
    float n = noise(gl_FragCoord.xy * 0.1);
    if (n < u_dissolve) discard;
    
    vec3 burnColor = vec3(0.0);
    if (u_dissolve > 0.0) {
        float burnEdge = step(n, u_dissolve + 0.1);
        burnColor = vec3(1.0, 0.4, 0.0) * burnEdge * 2.0;
    }
    // --------------------
    
    vec3 normal = vec3(coord, sqrt(1.0 - dot(coord, coord)));
    vec3 lightDir3D = normalize(vec3(u_lightDir, 0.5));
    float diffuse = max(dot(normal, lightDir3D), 0.0);
    
    // Storm systems
    vec2 stormCoord = gl_FragCoord.xy * 0.025;
    float storms = noise(stormCoord + vec2(u_time * 0.1, 0)) * 0.2;
    
    // Great Dark Spot
    vec2 spotCenter = vec2(-0.2, 0.1);
    float spotDist = length(coord - spotCenter);
    float darkSpot = smoothstep(0.2, 0.1, spotDist) * 0.3;
    
    vec3 color = u_baseColor.rgb * (1.0 + storms - darkSpot) * (diffuse * 0.7 + 0.3);
    gl_FragColor = vec4(color + burnColor, 1.0);
}
)";

// Sun shader
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

// Black Hole shader
const std::string BLACKHOLE_FRAG_SHADER = R"(
#version 120
uniform float u_time;
uniform float u_eventHorizonRadius;

float hash(vec2 p) { return fract(sin(dot(p, vec2(12.9898, 78.233))) * 43758.5453); }
float noise(vec2 p) {
    vec2 i = floor(p); vec2 f = fract(p); f = f*f*(3.0-2.0*f);
    return mix(mix(hash(i), hash(i+vec2(1,0)), f.x), mix(hash(i+vec2(0,1)), hash(i+vec2(1,1)), f.x), f.y);
}

void main() {
    vec2 coord = gl_TexCoord[0].xy * 2.0 - 1.0;
    float dist = length(coord);
    if (dist > 1.0) discard;
    
    // Event horizon (pure black circle in center)
    if (dist < u_eventHorizonRadius) {
        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }
    
    // Accretion disk (swirling matter around black hole)
    float angle = atan(coord.y, coord.x);
    float spiral = angle + u_time + dist * 10.0;
    
    // Create swirling bands
    float bands = sin(spiral * 3.0) * 0.5 + 0.5;
    float turbulence = noise(vec2(spiral, dist * 5.0)) * 0.3;
    
    // Gravitational lensing effect (matter closer to horizon is brighter/redder)
    float intensity = (1.0 - dist) * 1.5;
    float redshift = smoothstep(u_eventHorizonRadius, 1.0, dist);
    
    // Color gradient: red near event horizon, orange/yellow further out
    vec3 innerColor = vec3(1.0, 0.2, 0.0);  // Red-orange
    vec3 outerColor = vec3(1.0, 0.8, 0.3);  // Yellow
    
    vec3 color = mix(innerColor, outerColor, redshift);
    color *= (bands + turbulence) * intensity;
    
    // Fade out at edges
    float alpha = smoothstep(1.0, u_eventHorizonRadius + 0.1, dist);
    
    gl_FragColor = vec4(color, alpha);
}
)";