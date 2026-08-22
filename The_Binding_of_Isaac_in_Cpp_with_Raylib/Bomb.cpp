#include "Bomb.h"

Bomb::Bomb(Vector2 position, Texture2D idle, Texture2D active, Texture2D explosion)
    : pos(position), tex_idle(idle), tex_active(active), tex_explosion(explosion) {}

void Bomb::Update(float dt) {
    if (state == BOMB_IDLE || state == BOMB_ACTIVE) {
        timer -= dt;
        if (timer <= 0.8f) state = BOMB_ACTIVE;
        if (timer <= 0.0f) state = BOMB_EXPLODING;
    }
    else if (state == BOMB_EXPLODING) {
        explode_timer -= dt;
        if (explode_timer <= 0.0f) state = BOMB_DONE;
    }
}

void Bomb::Draw() {
    if (state == BOMB_IDLE) {
        DrawTexturePro(tex_idle, { 0, 0, (float)tex_idle.width, (float)tex_idle.height },
            { pos.x, pos.y, (float)tex_idle.width, (float)tex_idle.height },
            { (float)tex_idle.width / 2.0f, (float)tex_idle.height / 2.0f }, 0.0f, WHITE);
    }
    else if (state == BOMB_ACTIVE) {
        Color tint = ((int)(timer * 10) % 2 == 0) ? RED : WHITE;
        DrawTexturePro(tex_active, { 0, 0, (float)tex_active.width, (float)tex_active.height },
            { pos.x, pos.y, (float)tex_active.width, (float)tex_active.height },
            { (float)tex_active.width / 2.0f, (float)tex_active.height / 2.0f }, 0.0f, tint);
    }
    else if (state == BOMB_EXPLODING) {
        DrawTexturePro(tex_explosion, { 0, 0, (float)tex_explosion.width, (float)tex_explosion.height },
            { pos.x, pos.y, radius * 2, radius * 2 },
            { radius, radius }, 0.0f, WHITE);
    }
}