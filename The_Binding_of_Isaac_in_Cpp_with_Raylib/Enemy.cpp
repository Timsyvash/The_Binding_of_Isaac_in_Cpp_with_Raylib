#include "Enemy.h"
#include <cmath>

Enemy::Enemy(Vector2 position, EnemyType enemy_type, Texture2D tex)
    : pos(position), type(enemy_type), texture(tex) {
    if (type == TYPE_MONSTRO) {
        hp = 10;
    }
    else if (type == TYPE_GAPER) {
        hp = 3;
    }
    else if (type == TYPE_POOTER) {
        hp = 2;
    }
}

void Enemy::Update(Vector2 player_pos, float dt) {
    if (!is_alive) return;

    // Простий рух у бік гравця
    Vector2 dir = { player_pos.x - pos.x, player_pos.y - pos.y };
    float len = sqrtf(dir.x * dir.x + dir.y * dir.y);

    if (len != 0) {
        dir.x /= len;
        dir.y /= len;
    }

    float speed = (type == TYPE_POOTER) ? 50.0f : ((type == TYPE_GAPER) ? 80.0f : 40.0f);
    pos.x += dir.x * speed * dt;
    pos.y += dir.y * speed * dt;
}

void Enemy::Draw() const {
    if (!is_alive) return;

    // Вказуємо кількість кадрів у рядку для кожного типу ворога
    int cols = 1;
    int rows = 1;

    if (type == TYPE_POOTER) {
        cols = 4; // Якщо у файл-спрайті Pooter 4 кадри по горизонталі
    }
    else if (type == TYPE_GAPER) {
        cols = 4; // Якщо у файл-спрайті Gaper 4 кадри
    }
    else if (type == TYPE_MONSTRO) {
        cols = 1; // Monstro окремий кадр або оберіть потрібне
    }

    float frame_w = (float)texture.width / cols;
    float frame_h = (float)texture.height / rows;

    // Беремо перший кадр (0, 0)
    Rectangle src = { 0.0f, 0.0f, frame_w, frame_h };

    // Фіксований підсумковий розмір на екрані
    Rectangle dest = { pos.x, pos.y, 40.0f, 40.0f };
    Vector2 origin = { 20.0f, 20.0f };

    DrawTexturePro(texture, src, dest, origin, 0.0f, WHITE);
}