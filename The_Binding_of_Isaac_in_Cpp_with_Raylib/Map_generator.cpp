#include "Map_generator.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <raylib.h>

Map_generator::Map_generator()
    : max_tunnels(1000), padding(0), max_steps(2)
{}

short Map_generator::canGenerate(short i, short j, short dx, short dy, short width, short height)
{
    short generate = 1;
    if ((i == (height - 3) && dy == 1) || (i == 2 && dy == -1))
        generate = 0;
    if ((j == (width - 3) && dx == 1) || (j == 2 && dx == -1))
        generate = 0;
    return generate;
}

#include "Map_generator.h"

void Map_generator::generateMap(short map[30][40]) {
    // Очищаємо карту (1 — підлога)
    for (int r = 0; r < map_height; r++) {
        for (int c = 0; c < map_width; c++) {
            map[r][c] = 1;
        }
    }

    // Заповнюємо камінням лише центральну частину кімнати з частішими проходами
    for (int r = 4; r < map_height - 4; r++) {
        for (int c = 4; c < map_width - 4; c++) {
            if (rand() % 100 < 15) { // 15% шанс появи блоку
                map[r][c] = 0;
            }
        }
    }
}

void Map_generator::printMap(short m[map_height][map_width])
{
    for (short row = 0; row < map_height; row++) {
        for (short col = 0; col < map_width; col++) {
            if (m[row][col] == 1)
                printf(" ");
            else
                printf("#");
        }
        printf("\n");
    }
}

void Map_generator::drawMap(short map[30][40], Texture2D block_texture, Rectangle room_bounds) {
    float tile_w = room_bounds.width / map_width;
    float tile_h = room_bounds.height / map_height;

    for (int r = 0; r < map_height; r++) {
        for (int c = 0; c < map_width; c++) {
            // Малюємо ТІЛЬКИ стіни / блоки (0). 
            // Плитки підлоги (1) пропускаємо, щоб було видно фон кімнати.
            if (map[r][c] == 0) {
                Rectangle tile_rect = {
                    room_bounds.x + c * tile_w,
                    room_bounds.y + r * tile_h,
                    tile_w,
                    tile_h
                };

                if (block_texture.id != 0) {
                    Rectangle src = { 0, 0, (float)block_texture.width, (float)block_texture.height };
                    DrawTexturePro(block_texture, src, tile_rect, { 0, 0 }, 0.0f, WHITE);
                }
                else {
                    DrawRectangleRec(tile_rect, DARKGRAY);
                }
            }
        }
    }
}