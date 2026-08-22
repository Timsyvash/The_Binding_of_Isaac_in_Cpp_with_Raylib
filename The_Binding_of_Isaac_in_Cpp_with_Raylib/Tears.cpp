#include "Tears.h"
#include "Enemy.h"
#include <cmath>
#include <algorithm>

Tears::Tears(const char* path_tear)
{
    texture_tear = LoadTexture(path_tear);
    speed = 400.0f;
}

Tears::~Tears()
{
    UnloadTexture(texture_tear);
}

void Tears::Update(Vector2 player_pos, float delta_time)
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mouse_pos = GetMousePosition();
        Vector2 dir = { mouse_pos.x - player_pos.x, mouse_pos.y - player_pos.y };

        float len = sqrtf(dir.x * dir.x + dir.y * dir.y);
        if (len != 0) {
            dir.x /= len;
            dir.y /= len;
        }

        // Кути для трьох сліз (в радіанах, ~15 градусів розльоту)
        float angles[] = { -0.26f, 0.0f, 0.26f };

        for (float angle : angles) {
            // Повертаємо вектор напрямку на потрібний кут
            float rot_x = dir.x * cosf(angle) - dir.y * sinf(angle);
            float rot_y = dir.x * sinf(angle) + dir.y * cosf(angle);

            Tear new_tear;
            new_tear.pos = player_pos;
            new_tear.vel = { rot_x * speed, rot_y * speed };
            new_tear.active = true;

            tears_list.push_back(new_tear);
        }
    } // Завершення обробки кліку миші

    // Оновлення позицій сліз
    for (auto& t : tears_list) {
        if (!t.active) continue;

        t.pos.x += t.vel.x * delta_time;
        t.pos.y += t.vel.y * delta_time;

        if (t.pos.x < 0 || t.pos.x > GetScreenWidth() ||
            t.pos.y < 0 || t.pos.y > GetScreenHeight()) {
            t.active = false;
        }
    }

    tears_list.erase(
        std::remove_if(tears_list.begin(), tears_list.end(), [](const Tear& t) { return !t.active; }),
        tears_list.end()
    );
}

void Tears::Draw()
{
    // В оригінальній текстурі tear.png рівно 13 кадрів у ряд
    int total_frames = 13;
    float frame_width = (float)texture_tear.width / (float)total_frames;
    float frame_height = (float)texture_tear.height;

    // Беремо ТОЧНО 1-й кадр (звичайнісіньку круглу сльозу)
    Rectangle src = { 0.0f, 0.0f, frame_width, frame_height };

    for (const auto& t : tears_list) {
        if (!t.active) continue;

        // Малюємо сльозу нормального розміру (наприклад, 20x20 пікселів)
        Rectangle dest = { t.pos.x, t.pos.y, 20.0f, 20.0f };
        Vector2 origin = { 10.0f, 10.0f }; // Центрування спрайта

        DrawTexturePro(texture_tear, src, dest, origin, 0.0f, WHITE);
    }
}

void Tears::CheckTileCollision(short map[30][40], Rectangle bounds) {
    float tile_w = bounds.width / 40.0f;
    float tile_h = bounds.height / 30.0f;

    for (size_t i = 0; i < tears_list.size(); i++) {
        if (!tears_list[i].active) continue;

        int col = (tears_list[i].pos.x - bounds.x) / tile_w;
        int row = (tears_list[i].pos.y - bounds.y) / tile_h;

        if (row >= 0 && row < 30 && col >= 0 && col < 40) {
            if (map[row][col] == 0) {
                // Видаляємо сльозу при влучанні в блок
                tears_list[i].active = false;
            }
        }
    }
}

void Tears::CheckEnemyCollision(vector<Enemy>& enemies)
{
    for (size_t t = 0; t < tears_list.size(); t++) {
        if (!tears_list[t].active) continue;

        for (auto& enemy : enemies) {
            if (!enemy.is_alive) continue;

            if (CheckCollisionCircles(tears_list[t].pos, 8.0f, enemy.pos, 20.0f)) {
                enemy.hp--;
                if (enemy.hp <= 0) {
                    enemy.is_alive = false;
                }

                tears_list[t].active = false;
                break;
            }
        }
    }
}