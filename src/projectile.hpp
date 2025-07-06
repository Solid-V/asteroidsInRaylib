#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP
#include "raylib.h"

#define PROJECTILE_SPEED 400.0f
#define PROJECTILE_LIFETIME 2.0f
#define PROJECTILE_THICK 10.0f
#define PROJECTILE_LEN 30.0f
#define PROJECTILES_MAX 12


class Projectile {
    public:
        //Projectile();
        Vector2 position;
        float rotation;
        float creationTime;
        bool active;
        void createProjectile(Vector2 position, float rotation);
        bool projectileUpdate(float frameTime, float time);
        void projectileDraw();
        //bool checkCollisionProjectile();
        static void Addprojectile(Vector2 position, float rotation);
        static int UpdateProjectiles(void);
        static void DrawProjectiles(void);
        static void ResetProjectiles(void);
    };


//Global array of projectiles
extern Projectile projectiles[PROJECTILES_MAX];

#endif