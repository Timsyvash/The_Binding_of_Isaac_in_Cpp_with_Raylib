#include "Player.h"

Player::Player(const char* filePath)
{
    texture_player = LoadTexture(filePath);

    cols = 10;
    rows = 4;

    speed = 250.0; // Швидкість переміщення в пікселях на секунду
    frame_time = 0.1; // Затримка між кадрами анімації (10 кадрів/сек)

    current_frame = 0;
    timer = 0.0;

    frame_width = (double)texture_player.width / cols;
    frame_height = (double)texture_player.height / rows;

    curr_dir_player = DOWN;
    is_moving = false;

    // Внутрішня межа кімнати
    room_bounds = { 40.0f, 40.0f, 1120.0f, 520.0f };
}

Player::~Player()
{
    UnloadTexture(texture_player);
}

void Player::Update(double delta_time, Vector2& position, double move_speed, short map[30][40])
{
    is_moving = false;
    Vector2 next_pos = position;

    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
        next_pos.y -= (float)(move_speed * delta_time);
        curr_dir_player = UP;
        is_moving = true;
    }
    else if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
        next_pos.y += (float)(move_speed * delta_time);
        curr_dir_player = DOWN;
        is_moving = true;
    }

    // Перевірка Y
    if (!CheckMapCollision(next_pos, map, room_bounds)) {
        position.y = next_pos.y;
    }

    next_pos = position;

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        next_pos.x -= (float)(move_speed * delta_time);
        curr_dir_player = LEFT;
        is_moving = true;
    }
    else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        next_pos.x += (float)(move_speed * delta_time);
        curr_dir_player = RIGHT;
        is_moving = true;
    }

    // Перевірка X
    if (!CheckMapCollision(next_pos, map, room_bounds)) {
        position.x = next_pos.x;
    }

    // Анімація та межі кімнати
    if (is_moving) {
        timer += delta_time;
        if (timer >= frame_time) {
            timer = 0.0;
            current_frame = (current_frame + 1) % cols;
        }
    }
    else {
        current_frame = 0;
    }

    KeepPlayerInBounds(position, (float)frame_width / 2.0f);
}

void Player::Draw(Vector2 position)
{
    Rectangle source_rec = {
        (float)(current_frame * frame_width),
        (float)((short)curr_dir_player * frame_height),
        (float)frame_width,
        (float)frame_height
    };

    Rectangle dest_rec = { position.x, position.y, (float)frame_width, (float)frame_height };
    Vector2 origin = { (float)frame_width / 2.0f, (float)frame_height / 2.0f };

    DrawTexturePro(texture_player, source_rec, dest_rec, origin, 0.0f, WHITE);
}

void Player::KeepPlayerInBounds(Vector2& player_pos, float player_radius)
{
    if (player_pos.x - player_radius < room_bounds.x) {
        player_pos.x = room_bounds.x + player_radius;
    }
    if (player_pos.x + player_radius > room_bounds.x + room_bounds.width) {
        player_pos.x = room_bounds.x + room_bounds.width - player_radius;
    }

    if (player_pos.y - player_radius < room_bounds.y) {
        player_pos.y = room_bounds.y + player_radius;
    }
    if (player_pos.y + player_radius > room_bounds.y + room_bounds.height) {
        player_pos.y = room_bounds.y + room_bounds.height - player_radius;
    }
}

bool Player::CheckMapCollision(Vector2 next_pos, short map[30][40], Rectangle bounds)
{
    float tile_w = bounds.width / 40.0f;
    float tile_h = bounds.height / 30.0f;

    // Зменшений радіус колізії для уникнення застрягання в кутах
    float radius = (float)frame_width / 6.0f;

    Vector2 points[4] = {
        { next_pos.x - radius, next_pos.y - radius },
        { next_pos.x + radius, next_pos.y - radius },
        { next_pos.x - radius, next_pos.y + radius },
        { next_pos.x + radius, next_pos.y + radius }
    };

    for (int p = 0; p < 4; p++) {
        // Трансформуємо координати у локальні відносно room_bounds
        int col = (int)((points[p].x - bounds.x) / tile_w);
        int row = (int)((points[p].y - bounds.y) / tile_h);

        // Перевіряємо вихід за межі масиву
        if (row >= 0 && row < 30 && col >= 0 && col < 40) {
            if (map[row][col] == 0) return true; // Перешкода
        }
    }
    return false;
}