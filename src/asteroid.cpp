#include "asteroid.hpp"
#include "raymath.h"
#include <cstdlib>
#include <iostream>

//some constants for this file only, maybe make a separate header file for these?
#define ASTEROID_RANDOM_ANGLE 20 * DEG2RAD
#define ASTEROID_SPLIT_ANGLE 80 * DEG2RAD
#define SCREEN_CENTER CLITERAL(Vector2){600.0f / 2, 600.0f / 2}

//this feels super hacky but whatever
typedef struct vector2 {
    float x;
    float y;
} vector2;

static float asteroidBaseAngle = 0.0f;
Asteroid Asteroid::createAsteroid(Vector2 position, Vector2 velocity, AsteroidType size) {
    Asteroid asteroid;
    asteroid.active = true;
    asteroid.position = position;
    asteroid.velocity = velocity;
    asteroid.type = size;
    asteroid.size = Clamp((float)size, 1.0f, 4.0f);  // Only allows sizes 1, 2, 4 this is a hacky method 
    asteroid.rotation = GetRandomValue(0, 360); // Random rotation angle
    asteroid.rotationSpeed = GetRandomValue(50, 240);
    asteroid.creationTime = GetTime();
    return asteroid;
}

void Asteroid::AsteroidUpdate(float frameTime, float time) {

    if (!active) {
        return;
    }

    if (time > creationTime + ASTEROID_LIFE) {
        active = false;
        return;
    }

    position = Vector2Add(position, 
        Vector2Scale(velocity, frameTime));
    rotation += rotationSpeed * frameTime;
}

void Asteroid::AsteroidDraw() {
    if (!active) {
        return;
    }

    DrawPolyLines(position, 3, 16 * (int)(size), rotation, WHITE);
}

void Asteroid::AddAsteroid(Vector2 position, AsteroidType size, float speedMod, bool spawn) {
    bool created = false;

    float tweakAngle = spawn ? ASTEROID_RANDOM_ANGLE : ASTEROID_SPLIT_ANGLE;
    Vector2 up = {0, -1};

    Vector2 velocity = spawn ? 
        Vector2Subtract(SCREEN_CENTER, position) :
        Vector2Rotate(up, asteroidBaseAngle * DEG2RAD);

        velocity = Vector2Scale(Vector2Normalize(velocity), speedMod * GetRandomValue(100, 225)); //Asteroid speed min is 100 and max is 225

        velocity = Vector2Rotate(velocity, (float)GetRandomValue(-tweakAngle, tweakAngle));

        for (int i = 0; i < MAX_ASTEROIDS; i++) {
            if (asteroids[i].active) {
                continue;
            }
            asteroids[i] = Asteroid::createAsteroid(position, velocity, size);
            created = true;
            break;
        } 


}

void Asteroid::DestroyAsteroid(int index, float angle) {
    const float largeSpeedMod = 0.5f;
    const float mediumSpeedMod = 0.33f;

    //C type programming syntax might edit later
    Asteroid* asteroid = &asteroids[index];
    asteroid->active = false; 

    float asteroidBaseAngle = angle;
    switch (asteroid->type) {
        case ASTEROID_LARGE: 
            AddAsteroid(asteroid->position, ASTEROID_MEDIUM, mediumSpeedMod, false);
            AddAsteroid(asteroid->position, ASTEROID_MEDIUM, mediumSpeedMod, false);
            break;
        case ASTEROID_MEDIUM:
            AddAsteroid(asteroid->position, ASTEROID_SMALL, largeSpeedMod, false);
            AddAsteroid(asteroid->position, ASTEROID_SMALL, largeSpeedMod, false);
            break;
        default:
            //stops spawning more asteroids
            asteroidBaseAngle = 0.0f;
            break;
    }
}