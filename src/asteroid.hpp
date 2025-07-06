#ifndef ASTEROID_H_
#define ASTEROID_H_

#include "raylib.h"
#include <cstdlib>
#include <ctime>  

#define ASTEROID_LIFE 10.0f //seconds
#define MAX_ASTEROIDS 64 //yes also declared in main.cpp i will change later

typedef enum AsteroidType {
    ASTEROID_SMALL = 1,
    ASTEROID_MEDIUM = 2,
    ASTEROID_LARGE = 4
} AsteroidType;

class Asteroid {
    public:
        bool active;

        Vector2 position;
        AsteroidType type;
        float size; 
        float rotation;
        float rotationSpeed;
        float creationTime;
        Vector2 velocity;
        static Asteroid createAsteroid(Vector2 position, Vector2 velocity, AsteroidType size);
        void AsteroidUpdate(float frameTime, float time);
        void AsteroidDraw();
        void AddAsteroid(Vector2 position, AsteroidType size, float speedMod, bool spawn);
        Vector2 GetNextAsteroidPosition(void);
        int UpdateAsteroids(void);
        void DestroyAsteroid(int index, float angle);
        void ResetAsteroids(void);
        Asteroid AsteroidsArray(void);
        void DrawAsteroids(void);
};

extern Asteroid asteroids[MAX_ASTEROIDS];
#endif