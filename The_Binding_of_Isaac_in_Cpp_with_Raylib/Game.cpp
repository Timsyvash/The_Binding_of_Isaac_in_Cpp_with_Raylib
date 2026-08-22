#include "Game.h"

Game::Game(const char* bg_path, const char* door_path, const char* portal_path) {
    texture_background = LoadTexture(bg_path);
    texture_door1 = LoadTexture(door_path);
    texture_portal1 = LoadTexture(portal_path);
}

Game::~Game() {
    UnloadTexture(texture_background);
    UnloadTexture(texture_door1);
    UnloadTexture(texture_portal1);
}

void Game::Update(const std::vector<Enemy>& enemies, int player_hp) {
    if (player_hp <= 0) {
        state = GAME_OVER;
        return;
    }

    bool has_alive_enemies = false;
    for (const auto& enemy : enemies) {
        if (enemy.is_alive) {
            has_alive_enemies = true;
            break;
        }
    }

    // Якщо живих ворогів немає — кімната зачищена
    is_room_cleared = !has_alive_enemies;
}

void Game::DrawBackground() {
    // Малюємо фон кімнати на самому нижньому шарі
    DrawTexture(texture_background, 0, 0, WHITE);

    // Двері зверху кімнати
    Vector2 door_pos = { (float)GetScreenWidth() / 2.0f - texture_door1.width / 2.0f, 10.0f };
    DrawTexture(texture_door1, (int)door_pos.x, (int)door_pos.y, is_room_cleared ? WHITE : RED);

    // Портал (якщо кімната зачищена)
    if (state == GAME_PLAYING && is_room_cleared) {
        Rectangle src = { 0, 0, (float)texture_portal1.width, (float)texture_portal1.height };
        DrawTexturePro(texture_portal1, src, portal_rect, { 0, 0 }, 0.0f, WHITE);
    }
}

void Game::DrawUI(int player_hp, int player_bombs, Texture2D tex_dead) {
    if (state == GAME_PLAYING) {
        // Інтерфейс поверх гри
        DrawText(TextFormat("HP: %d", player_hp), 60, 50, 20, RED);
        DrawText(TextFormat("BOMBS (E): %d", player_bombs), 60, 75, 20, YELLOW);
        DrawText(TextFormat("ROOM: %d/%d", current_room, max_rooms), 60, 100, 20, RAYWHITE);
    }
    else if (state == GAME_OVER) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.85f));
        DrawTextureEx(tex_dead, { GetScreenWidth() / 2.0f - tex_dead.width, GetScreenHeight() / 2.0f - 100 }, 0.0f, 2.0f, WHITE);
        DrawText("GAME OVER", GetScreenWidth() / 2 - 130, GetScreenHeight() / 2 + 20, 40, RED);
        DrawText("Press 'R' to Restart", GetScreenWidth() / 2 - 110, GetScreenHeight() / 2 + 70, 20, RAYWHITE);
    }
    else if (state == GAME_WIN) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.85f));
        DrawText("VICTORY!", GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 - 40, 50, GREEN);
        DrawText("Press 'R' to Restart", GetScreenWidth() / 2 - 110, GetScreenHeight() / 2 + 30, 20, RAYWHITE);
    }
}