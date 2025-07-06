#include <iostream>
#include "raylib.h"
#include "raymath.h"

#include "asteroid.hpp"
#include "player.hpp"
#include "projectile.hpp"

const int screenWidth = 800;
const int screenHeight = 600;
const Vector2 screenCenter = { (float) screenWidth / 2.0f, (float) screenHeight / 2.0f};

//#define NEARBLACK (Color){ 15, 15, 15, 255 }
Color NEARBLACK = { 15, 15, 15, 255 }; 

#define MAX_ASTEROIDS 64
#define ASTEROID_RANDOM_ANGLE 30 * DEG2RAD
#define ASTEROID_DELAY 0.75f
static AsteroidType asteroidTypes[] = { ASTEROID_SMALL, ASTEROID_MEDIUM, ASTEROID_LARGE };
Asteroid asteroids[MAX_ASTEROIDS];  //static limits the scope of the asteroids array to the current source file
//since asteroids is a defined class with a constructor, then {0} does not zero everything, it instead calls 
//default constructor and fails if it does not exist.
static float _lastAsteroidCreationTime = -1.0f;

//Player player;
Projectile projectiles[PROJECTILES_MAX];

void UpdateDrawFrame(void);
void AddAsteroid(Vector2 position, AsteroidType size);
Vector2 GetNextAsteroidPosition(void);

//DEBUG CONSTS
bool _showAsteroidCount = true;
//

int main() {
    // Initialize the window
    InitWindow(screenWidth, screenHeight, "Asteroids");
    SetTargetFPS(60);

    Vector2 velocity = {0, 0};
    player.position = {screenCenter.x, screenCenter.y};
    player.rotation = 180.0f;
    player.velocity = velocity;

    while (!WindowShouldClose()) {
        UpdateDrawFrame();
    }

    CloseWindow(); 
    return 0;
}

void UpdateDrawFrame(void) {
    
    float frameTime = GetFrameTime();
    float time = GetTime();
    
    player.PlayerUpdate();
    Projectile::UpdateProjectiles();


    for (int i = 0; i < MAX_ASTEROIDS; i++) {

        asteroids[i].AsteroidUpdate(frameTime, time); // not ideal

    }

    if (time > _lastAsteroidCreationTime + ASTEROID_DELAY) {
        AsteroidType nextType = asteroidTypes[GetRandomValue(0, 3)];
        AddAsteroid(GetNextAsteroidPosition(), nextType);
        _lastAsteroidCreationTime = time;
    }  

    BeginDrawing();
    
        ClearBackground(NEARBLACK);
        
        player.PlayerDraw();
        Projectile::DrawProjectiles();
        for (int i = 0; i < MAX_ASTEROIDS; i++) {
            asteroids[i].AsteroidDraw();
        }

        if (_showAsteroidCount) {
            int count = 0;
            for (int i = 0; i < MAX_ASTEROIDS; i++) {
                if (asteroids[i].active){
                    count++;
                }
            }
            DrawRectangle(10, 10, 200, 52, Fade(BLACK, 0.6f));
            DrawText(TextFormat("Asteroids: %d", count), 20, 20, 32, WHITE);
        }

    EndDrawing();
}

void AddAsteroid(Vector2 position, AsteroidType size) {
    Vector2 velocity = Vector2Subtract(screenCenter, position);
    velocity = Vector2Scale(Vector2Normalize(velocity), GetRandomValue(100, 300));
    velocity = Vector2Rotate(velocity, float(GetRandomValue(-ASTEROID_RANDOM_ANGLE, ASTEROID_RANDOM_ANGLE)));

    Asteroid asteroid = Asteroid::createAsteroid(position, velocity, size);
    bool created = false;

    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (!asteroids[i].active) {
            asteroids[i] = asteroid;
            created = true;
            break;
        }

    }

    if (!created) {
        std::cerr << "Failed to create asteroid, no inactive slots in the array" << std::endl;
    }
}

Vector2 GetNextAsteroidPosition(void) {
    
    float padding = 128; // Padding around the screen edges
    Vector2 result = {-padding, -padding};

    if (GetRandomValue(0, 1)){
        if (GetRandomValue(0, 1)) {
            result.y = screenHeight + padding;
        } 
        
        result.x = GetRandomValue(-padding, screenWidth + padding);

    } else {
        if (GetRandomValue(0, 1)) {
            result.x = screenWidth + padding;
        } 
        
        result.y = GetRandomValue(-padding, screenHeight + padding);
    }

    return result;
}

