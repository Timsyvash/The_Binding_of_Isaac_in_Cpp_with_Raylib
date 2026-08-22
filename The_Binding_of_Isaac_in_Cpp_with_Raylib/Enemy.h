#pragma once
#include <raylib.h>

enum EnemyType {
    TYPE_MONSTRO,
    TYPE_GAPER,
    TYPE_POOTER
};

class Enemy {
public:
    Vector2 pos;
    Vector2 vel = { 0, 0 };
    EnemyType type;
    Texture2D texture;
    int hp;
    bool is_alive = true;

    Enemy(Vector2 position, EnemyType enemy_type, Texture2D tex);
    void Update(Vector2 player_pos, float dt);
    void Draw() const;
};