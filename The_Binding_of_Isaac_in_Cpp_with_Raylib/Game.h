#pragma once
#include <raylib.h>
#include <vector>
#include "Enemy.h"

enum GameState {
    GAME_PLAYING,
    GAME_OVER,
    GAME_WIN
};

class Game {
private:
    Texture2D texture_background;
    Texture2D texture_door1;
    Texture2D texture_portal1;

public:
    GameState state = GAME_PLAYING;
    int current_room = 1;
    int max_rooms = 5;
    bool is_room_cleared = false;
    Rectangle portal_rect = { 575.0f, 270.0f, 50.0f, 50.0f };

    Game(const char* bg_path, const char* door_path, const char* portal_path);
    ~Game();

    void Update(const std::vector<Enemy>& enemies, int player_hp);
    void DrawBackground();
    void DrawUI(int player_hp, int player_bombs, Texture2D tex_dead);
};