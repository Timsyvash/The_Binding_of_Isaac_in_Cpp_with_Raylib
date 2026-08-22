#pragma once
#include <raylib.h>
#include <vector>
#include "Tear.h"

using namespace std;

// Попереднє оголошення класу Enemy
class Enemy;

class Tears {
private:
    Texture2D texture_tear;
    std::vector<Tear> tears_list;
    float speed;

public:
    Tears(const char* path_tear);
    ~Tears();

    void Update(Vector2 player_pos, float delta_time);
    void Draw();
    void CheckTileCollision(short map[30][40], Rectangle bounds);
    void CheckEnemyCollision(vector<Enemy>& enemies);
};