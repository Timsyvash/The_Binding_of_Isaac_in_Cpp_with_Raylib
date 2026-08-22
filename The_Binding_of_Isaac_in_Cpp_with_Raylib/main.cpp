#include <vector>
#include <cstdlib>
#include <ctime>
#include "Settings.h"
#include "Player.h"
#include "Game.h"
#include "Tears.h"
#include "Map_generator.h"
#include "Enemy.h"
#include "Bomb.h"

using namespace std;

void ResetRoom(short map[30][40], Map_generator& map_gen, vector<Enemy>& enemies,
    Texture2D t_monstro, Texture2D t_gaper, Texture2D t_pooter,
    Vector2& player_pos, Rectangle room_bounds) {
    map_gen.generateMap(map);
    enemies.clear();

    float tile_w = room_bounds.width / Map_generator::map_width;
    float tile_h = room_bounds.height / Map_generator::map_height;

    for (int r = 0; r < Map_generator::map_height; r++) {
        for (int c = 0; c < Map_generator::map_width; c++) {
            if (map[r][c] == 1) {
                player_pos = { room_bounds.x + (c + 0.5f) * tile_w, room_bounds.y + (r + 0.5f) * tile_h };
                break;
            }
        }
    }

    int count = rand() % 4 + 2;
    while (enemies.size() < (size_t)count) {
        int r = rand() % Map_generator::map_height;
        int c = rand() % Map_generator::map_width;

        if (map[r][c] == 1) {
            Vector2 pos = { room_bounds.x + (c + 0.5f) * tile_w, room_bounds.y + (r + 0.5f) * tile_h };

            if (!CheckCollisionCircles(pos, 20.0f, player_pos, 150.0f)) {
                int roll = rand() % 3;
                if (roll == 0) enemies.push_back(Enemy(pos, TYPE_MONSTRO, t_monstro));
                else if (roll == 1) enemies.push_back(Enemy(pos, TYPE_GAPER, t_gaper));
                else enemies.push_back(Enemy(pos, TYPE_POOTER, t_pooter));
            }
        }
    }
}

int main() {
    srand((unsigned int)time(NULL));
    InitWindow(window_width, window_height, title);
    SetTargetFPS(fps);

    InitAudioDevice(); // 1. Ініціалізація аудіо

    Game game(path_background, path_door1, path_portal1);
    Player player(path_isaac);
    Tears tears(path_tear);
    Map_generator map_generator;

    // Завантаження текстур
    Texture2D tex_dead = LoadTexture(path_isaac_dead);
    Texture2D tex_monstro = LoadTexture(path_monstro);
    Texture2D tex_gaper = LoadTexture(path_gaper);
    Texture2D tex_pooter = LoadTexture(path_pooter);

    Texture2D tex_b_idle = LoadTexture(path_bomb_idle);
    Texture2D tex_b_act = LoadTexture(path_bomb_active);
    Texture2D tex_b_exp = LoadTexture(path_explosion);
    Texture2D tex_block = LoadTexture(path_block);

    // 2. Завантаження звуків ДО циклу while
    Sound snd_shoot = LoadSound("sounds/sound_shoot.wav");
    Sound snd_explosion = LoadSound("sounds/sound_explosion.wav");
    Sound snd_hurt = LoadSound("sounds/sound_hurt.wav");

    int player_hp = 3;
    int player_bombs = 3;
    float dmg_cooldown = 0.0f;
    Vector2 player_pos = { 480.0f, 270.0f };

    short map[Map_generator::map_height][Map_generator::map_width];
    Rectangle room_bounds = { 40.0f, 40.0f, 1120.0f, 520.0f };

    vector<Enemy> enemies;
    vector<Bomb> bombs;

    ResetRoom(map, map_generator, enemies, tex_monstro, tex_gaper, tex_pooter, player_pos, room_bounds);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        if (game.state != GAME_PLAYING) {
            if (IsKeyPressed(KEY_R)) {
                player_hp = 3;
                player_bombs = 3;
                game.current_room = 1;
                game.state = GAME_PLAYING;
                bombs.clear();
                ResetRoom(map, map_generator, enemies, tex_monstro, tex_gaper, tex_pooter, player_pos, room_bounds);
            }
        }
        else {
            // Звук пострілу при кліку миші
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                PlaySound(snd_shoot);
            }

            // Оновлення гравця та сліз
            player.Update(dt, player_pos, 250.0f, map);
            tears.Update(player_pos, dt);
            tears.CheckTileCollision(map, room_bounds);
            tears.CheckEnemyCollision(enemies);

            // Закладка бомби
            if (IsKeyPressed(KEY_E) && player_bombs > 0) {
                player_bombs--;
                bombs.push_back(Bomb(player_pos, tex_b_idle, tex_b_act, tex_b_exp));
            }

            // Оновлення бомб
            for (auto it = bombs.begin(); it != bombs.end(); ) {
                it->Update(dt);
                if (it->state == BOMB_EXPLODING) {
                    // Звук вибуху
                    PlaySound(snd_explosion);

                    float tile_w = room_bounds.width / Map_generator::map_width;
                    float tile_h = room_bounds.height / Map_generator::map_height;

                    for (int r = 0; r < Map_generator::map_height; r++) {
                        for (int c = 0; c < Map_generator::map_width; c++) {
                            if (map[r][c] == 0) {
                                Vector2 block_center = {
                                    room_bounds.x + (c + 0.5f) * tile_w,
                                    room_bounds.y + (r + 0.5f) * tile_h
                                };
                                if (CheckCollisionCircles(it->pos, it->radius, block_center, tile_w / 2.0f)) {
                                    map[r][c] = 1;
                                }
                            }
                        }
                    }

                    for (auto& enemy : enemies) {
                        if (enemy.is_alive && CheckCollisionCircles(it->pos, it->radius, enemy.pos, 20)) {
                            enemy.hp -= 5;
                            if (enemy.hp <= 0) enemy.is_alive = false;
                        }
                    }

                    if (dmg_cooldown <= 0.0f && CheckCollisionCircles(it->pos, it->radius, player_pos, 15)) {
                        player_hp--;
                        dmg_cooldown = 1.0f;
                        PlaySound(snd_hurt); // Звук отримання шкоди від вибуху
                    }
                }
                if (it->state == BOMB_DONE) it = bombs.erase(it);
                else ++it;
            }

            // Шкода від ворогів
            if (dmg_cooldown > 0.0f) dmg_cooldown -= dt;
            for (auto& enemy : enemies) {
                if (!enemy.is_alive) continue;
                enemy.Update(player_pos, dt);
                if (dmg_cooldown <= 0.0f && CheckCollisionCircles(player_pos, 15.0f, enemy.pos, 15.0f)) {
                    player_hp--;
                    dmg_cooldown = 1.0f;
                    PlaySound(snd_hurt); // Звук отримання шкоди від ворога
                }
            }

            game.Update(enemies, player_hp);

            if (game.is_room_cleared) {
                Rectangle p_rect = { player_pos.x - 16, player_pos.y - 16, 32, 32 };
                if (CheckCollisionRecs(p_rect, game.portal_rect)) {
                    if (game.current_room >= game.max_rooms) game.state = GAME_WIN;
                    else {
                        game.current_room++;
                        bombs.clear();
                        ResetRoom(map, map_generator, enemies, tex_monstro, tex_gaper, tex_pooter, player_pos, room_bounds);
                    }
                }
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        game.DrawBackground();

        if (game.state == GAME_PLAYING) {
            map_generator.drawMap(map, tex_block, room_bounds);
            for (auto& bomb : bombs) bomb.Draw();
            for (auto& enemy : enemies) enemy.Draw();
            tears.Draw();
            player.Draw(player_pos);
        }

        game.DrawUI(player_hp, player_bombs, tex_dead);

        EndDrawing();
    }

    // 3. Вивантаження ресурсів
    UnloadTexture(tex_dead); UnloadTexture(tex_monstro);
    UnloadTexture(tex_gaper); UnloadTexture(tex_pooter);
    UnloadTexture(tex_b_idle); UnloadTexture(tex_b_act); UnloadTexture(tex_b_exp);
    UnloadTexture(tex_block);

    UnloadSound(snd_shoot);
    UnloadSound(snd_explosion);
    UnloadSound(snd_hurt);

    CloseAudioDevice(); // Закриття аудіосистеми
    CloseWindow();
    return 0;
}