#pragma once
#include <raylib.h> // Додано підключення Raylib для роботи з Texture2D

class Map_generator {
private:
    const unsigned short max_tunnels;
    const unsigned short padding;
    const unsigned short max_steps;

public:
    static constexpr unsigned short map_width = 40;
    static constexpr unsigned short map_height = 30;

    Map_generator();
    short canGenerate(short i, short j, short dx, short dy, short width, short height);
    void generateMap(short m[map_height][map_width]);
    void printMap(short m[map_height][map_width]);
    void drawMap(short m[map_height][map_width], Texture2D wall_texture, Rectangle bounds);
};