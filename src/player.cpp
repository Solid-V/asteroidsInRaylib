#include "player.hpp"
#include "projectile.hpp"
#include "asteroid.hpp"
#include "raymath.h"
#include <iostream>

Player player;

void Player::PlayerUpdate() {
    float frameTime = GetFrameTime();

    if (invincible) {
    invincibleTime -= frameTime;
    if (invincibleTime <= 0.0f) {
        invincible = false;
    }
    }


    int xIn = (int)IsKeyDown(KEY_RIGHT) - (int)IsKeyDown(KEY_LEFT);
    rotation += xIn * PLAYER_ROT_SPEED * GetFrameTime();

    int yIn = (int)IsKeyDown(KEY_UP) - (int)IsKeyDown(KEY_DOWN);
    float speed = Vector2Length(velocity);
    if (yIn > 0) {
        Vector2 Vecpos = {1, 0};
        Vector2 facingDirection = Vector2Rotate(Vecpos, rotation * DEG2RAD);
        velocity = Vector2Add(velocity, Vector2Scale(facingDirection, PLAYER_ACCELERATION
        * frameTime));
        if (speed > PLAYER_SPEED) {
            velocity = Vector2Scale(velocity, PLAYER_SPEED / speed);
        }
    } else {
        if (speed > 0) {
            float speedSqr = speed * speed;

            float xSign = (velocity.x < 0) ? -1.0f: 1.0f;
            float ySign = (velocity.y < 0) ? -1.0f: 1.0f;

            float xAbs = velocity.x * xSign;
            float yAbs = velocity.y * ySign;

            float xWeight = xAbs * xAbs / speedSqr;
            float yWeight = yAbs * yAbs / speedSqr;

            float xDecel = xWeight * PLAYER_DECELERATION * xSign * frameTime;
            float yDecel = yWeight * PLAYER_DECELERATION * ySign * frameTime;

            velocity.x = (xAbs > xDecel) ? velocity.x - xDecel : 0;
            velocity.y = (yAbs > yDecel) ? velocity.y - yDecel : 0;
        }
    }
    
    bool hit = false;
    for (int j = 0; j < MAX_ASTEROIDS; j++) {

        if (!asteroids[j].active) {
            continue;
        }
        if (CheckCollisionCircles(position, PLAYER_RADIUS, asteroids[j].position, asteroids[j].size)) {
            //Collision detected with an asteroid
            // reset the player
                    std::cout << "[DEBUG] Collision with asteroid " << j << " at (" 
                  << asteroids[j].position.x << "," 
                  << asteroids[j].position.y << ")\n";

            std::cout << "[DEBUG] Asteroid[" << j << "] at (" 
              << asteroids[j].position.x << "," << asteroids[j].position.y 
              << ") with size " << asteroids[j].size << "\n";

            hit = true;
            ResetPlayer();
            asteroids[j].active = false; // Destroy the asteroid
            return;
        }
    }
    position = Vector2Add(position, Vector2Scale(velocity, frameTime));


    //write the shooting projectiles part here
    if (IsKeyPressed(KEY_SPACE)) {
        Vector2 spawnOffset = Vector2Scale(GetFacingDirection(), PLAYER_RADIUS);
        Vector2 spawnPosition = Vector2Add(position, spawnOffset);
        Projectile::Addprojectile(spawnPosition, rotation);
        //Projectile::Addprojectile(Vector2Add(player.position, Vector2Scale(GetFacingDirection(player), PLAYER_RADIUS)), rotation);
    }
    UpdateWrap(frameTime);

    

}

void Player::UpdateWrap(float frametime) {
    if (position.x > SCREEN_WIDTH + PLAYER_RADIUS) {
        position.x = -PLAYER_RADIUS;
    } else if (position.x < -PLAYER_RADIUS) {
        position.x = SCREEN_WIDTH + PLAYER_RADIUS;
    }

    if (position.y > SCREEN_HEIGHT + PLAYER_RADIUS) {
        position.y = -PLAYER_RADIUS;
    } else if (position.y < -PLAYER_RADIUS) {
        position.y = SCREEN_HEIGHT + PLAYER_RADIUS;
    }
}

void Player::PlayerDraw() {
    DrawPoly(position, 3, 32, rotation, RAYWHITE);
}

//for the main script to shoot projectiles
Vector2 Player::GetFacingDirection() {
    //Vector2 position = {1, 0};
    return Vector2Rotate({1, 0}, rotation * DEG2RAD);
}

void Player::ResetPlayer() {
    position = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
    velocity = {0, 0};
    rotation = 0.0f;
    invincible = true;
    invincibleTime = 2.0f;
}