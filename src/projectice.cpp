#include "projectile.hpp"
#include "asteroid.hpp"
#include "raymath.h"
#include <cmath>
#include <iostream>

void Projectile::createProjectile(Vector2 position, float rotation) {
    this->position = position;
    this->rotation = rotation;
    active = true;
    creationTime = GetTime();

}

bool Projectile::projectileUpdate(float frameTime, float time) {
    if (!active) { return false;}

    if (time > creationTime + PROJECTILE_LIFETIME) {
        active  = false;
        return false;
    }

    double radians = DEG2RAD * rotation;
    /*Vector2 direction = { -cosf(rotation), -sinf(rotation)};
    position.x += PROJECTILE_SPEED * direction.x * frameTime;
    position.y += PROJECTILE_SPEED * direction.y * frameTime;
    */
    position.x += PROJECTILE_SPEED * cosf(radians) * frameTime;
    position.y += PROJECTILE_SPEED * sinf(radians) * frameTime;

    return true;
}

void Projectile::projectileDraw() {

    if (!active) { return; }
    Rectangle rect = {position.x, position.y, PROJECTILE_THICK, PROJECTILE_LEN};
    Vector2 origin = {rect.width / 2, rect.height / 2};
    DrawRectanglePro(rect, origin, rotation + 90.0f, RED);
}

//from this point on, the tutorial had separated these functions to game_projectile.cpp, but i combined it
//these below functions are used to modify game projectiles not a single projectile
void Projectile::Addprojectile(Vector2 position, float rotation) {
    bool created = false;

    for (int i = 0; i < PROJECTILES_MAX; i++) {
        if (projectiles[i].active) {
            continue;
        }

        projectiles[i].createProjectile(position, rotation);
        created = true;
        break;
    }

    if (!created) {
        std::cout << "Failed to create projectile: No available slots." << std::endl;
    }
}

int Projectile::UpdateProjectiles(void) {
    float frameTime = GetFrameTime();
    float time = GetTime();

    int projectileCount = 0;
    for (int i = 0; i < PROJECTILES_MAX; i++) {

        if (projectiles[i].projectileUpdate(frameTime, time)) {
            projectileCount++;
            
            bool hit = false;
            for (int j = 0; j < MAX_ASTEROIDS; j++) {
                if (!asteroids[j].active){
                    continue;
                }

                if (CheckCollisionCircles(projectiles[i].position, PROJECTILE_THICK / 2.0f,
                                          asteroids[j].position, asteroids[j].type * 10.0f)) {
                    hit = true;
                    asteroids[j].DestroyAsteroid(j, projectiles[i].rotation);
                    projectiles[i].active = false;
                    break;
                }

            }

        }
    }

    return projectileCount;
}

void Projectile::DrawProjectiles(void) {
    for (int i = 0; i < PROJECTILES_MAX; i++) {
        if (projectiles[i].active) {
            projectiles[i].projectileDraw();
        }
    }
}

void Projectile::ResetProjectiles(void) {
    for (int i = 0; i < PROJECTILES_MAX; i++) {
        projectiles[i] = Projectile();
    }
}