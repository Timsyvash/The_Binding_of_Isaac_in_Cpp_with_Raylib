#pragma once
#include <raylib.h>

enum Dir {
    DOWN = 0,
    UP = 1,
    RIGHT = 2,
    LEFT = 3
};

class Player {
private:
    Texture2D texture_player;
    short current_frame;
    double timer;
    double frame_time;

    short cols;
    short rows;

    double frame_width;
    double frame_height;

    bool is_moving;
    Rectangle room_bounds;

public:
    double speed;
    Dir curr_dir_player;
    Vector2 pos;

    Player(const char* filePath);
    ~Player();

    // ВИПРАВЛЕНО: Додано параметр map та оголошено CheckMapCollision
    void Update(double delta_time, Vector2& position, double move_speed, short map[30][40]);
    void Draw(Vector2 position);
    void KeepPlayerInBounds(Vector2& player_pos, float player_radius);
    bool CheckMapCollision(Vector2 next_pos, short map[30][40], Rectangle bounds);
};