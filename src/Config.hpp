#pragma once

// --- CHANGED: Scale Factor ---
// Old: 10.0f -> New: 20.0f
// This makes 1 meter in physics equal to 20 pixels on screen (2x visual size)
constexpr float PIXELS_PER_METER = 20.f;
constexpr float METERS_PER_PIXEL = 1.f / PIXELS_PER_METER;

// Physics Settings
constexpr float TIME_STEP = 1.f / 60.f;
constexpr int VELOCITY_ITERS = 8;
constexpr int POSITION_ITERS = 3;

// Simulation Constants
constexpr float G_CONSTANT = 5.0f; 
constexpr float SUN_MASS = 5000.0f; 

// --- CHANGED: Sun Size ---
// Old: 30.0f -> New: 50.0f (Bigger Sun)
constexpr float SUN_RADIUS = 100.0f;