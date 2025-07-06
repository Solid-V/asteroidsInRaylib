#ifndef PLAYER_H_
#define PLAYER_H_
#include "raylib.h"

#define PLAYER_ROT_SPEED 300.0f
#define PLAYER_ACCELERATION 400.0f
#define PLAYER_DECELERATION 375.0f
#define PLAYER_SPEED 180.0f
#define PLAYER_RADIUS 24.0f

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

class Player {
    public:
        Vector2 position;
        Vector2 velocity;
        float rotation;
        bool invincible = false;
        float invincibleTime = 0.0f;
        void PlayerUpdate();
        void PlayerDraw();
        void UpdateWrap(float frameTime);
        Vector2 GetFacingDirection();
        void ResetPlayer();
};

extern Player player;

#endif // PLAYER_H