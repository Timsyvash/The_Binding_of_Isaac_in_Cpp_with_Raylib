#pragma once
#include <raylib.h>
#include <vector>

enum BombState { BOMB_IDLE, BOMB_ACTIVE, BOMB_EXPLODING, BOMB_DONE };

class Bomb {
public:
    Vector2 pos;
    float timer = 2.0f;       // 2 секунди до вибуху
    float explode_timer = 0.3f; // Тривалість спалаху вибуху
    float radius = 80.0f;
    BombState state = BOMB_IDLE;

    Texture2D tex_idle;
    Texture2D tex_active;
    Texture2D tex_explosion;

    Bomb(Vector2 position, Texture2D idle, Texture2D active, Texture2D explosion);
    void Update(float dt);
    void Draw();
};