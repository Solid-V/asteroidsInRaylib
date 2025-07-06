# Asteroids Clone in Raylib
Made an asteroids clone in raylib and c++ 

Does not have lives system or a game loop but it is playable
The hud on the top determines how many asteroids are still present in the game
based on this [tutorial](https://www.youtube.com/watch?v=abIlGCx_Yq8&t=1064s)

build it with\
g++ main.cpp asteroid.cpp projectice.cpp player.cpp -o main -I./include -L./lib -lraylib -lopengl32 -lgdi32 -lwinmm -std=c++17

yes i have not made a cmake file :)))
