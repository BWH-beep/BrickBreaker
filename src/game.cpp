#include <cstring>
#include "game.h"
#include <cmath>
#include <algorithm> 
#include "menu.h"
#include <future>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

std::vector<std::vector<int>> LoadLevelFromJSON(const std::string& path) {
    std::vector<std::vector<int>> layout;
    std::ifstream file(path);
    if (!file.is_open()) {
        TraceLog(LOG_WARNING, "无法打开关卡文件: %s", path.c_str());
        return layout;
    }
    json j;
    file >> j;
    auto jlayout = j["layout"];
    for (const auto& row : jlayout) {
        std::vector<int> r;
        for (const auto& val : row) {
            r.push_back(val.get<int>());
        }
        layout.push_back(r);
    }
    return layout;
}

// 关卡图案定义 (0=空, 1=砖块, 2=障碍物, 3=恶魔)
std::vector<std::vector<int>> g_levelPatterns[3][3] = {
    // ========== 关卡1：爱心 ==========
    {
        // Easy：空心爱心
        {
            {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
            {0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0},
            {0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0},
            {0,0,0,1,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0,0,0},
            {0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0},
            {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
            {0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
            {0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
            {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
            {0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
            {0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
        },
        // Normal：实心爱心
        {
            {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
            {0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0},
            {0,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,0},
            {0,0,0,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,0,0,0},
            {0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
            {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
            {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
            {0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
            {0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0},
            {0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
            {0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0},
            {0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
        },
        // Hard：实心+障碍+恶魔
        {
            {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
            {0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0},
            {0,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,0},
            {0,0,0,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,0,0,0},
            {0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
            {0,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,0},
            {1,1,1,1,1,1,1,1,1,1,3,1,1,1,3,1,1,1,1,1,1,1,1,1,1},
            {0,1,1,1,1,1,1,2,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,0},
            {0,0,1,1,1,1,1,1,1,1,2,1,1,1,2,1,1,1,1,1,1,1,1,0,0},
            {0,0,0,1,1,1,1,1,1,1,1,3,1,3,1,1,1,1,1,1,1,1,0,0,0},
            {0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
            {0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0},
            {0,0,0,0,0,0,1,1,1,1,1,2,1,2,1,1,1,1,1,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,1,1,1,1,1,3,1,1,1,1,1,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,1,3,1,3,1,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,2,1,2,0,0,0,0,0,0,0,0,0,0,0},
        },
    },
    // ========== 关卡2：猫咪 ==========
    {
        // Easy：实心猫咪
        {
            {0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0},
            {0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0},
            {0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0},
            {0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0},
            {0,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,0},
            {1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1},
            {1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1},
            {1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1},
            {0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0},
            {0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0},
            {0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0},
        },
        // Normal：猫咪+障碍
        {
            {0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0},
            {0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0},
            {0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0},
            {0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0},
            {0,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,0},
            {1,1,1,0,0,0,0,0,0,0,1,2,1,2,1,0,0,0,0,0,0,0,1,1,1},
            {1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1},
            {1,1,1,1,1,1,1,1,1,1,1,0,2,0,1,1,1,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1},
            {0,1,1,1,1,1,1,1,0,0,2,0,0,0,2,0,0,1,1,1,1,1,1,1,0},
            {0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0},
            {0,0,0,2,2,0,0,0,0,0,0,2,0,2,0,0,0,0,0,0,2,2,0,0,0},
        },
        // Hard：猫咪+障碍+恶魔
        {
            {0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0},
            {0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0},
            {0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0},
            {0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0},
            {0,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,0},
            {1,1,1,0,0,0,0,0,0,0,1,2,1,2,1,0,0,0,0,0,0,0,1,1,1},
            {1,1,0,0,0,0,0,0,0,0,0,1,3,1,0,0,0,0,0,0,0,0,0,1,1},
            {1,1,1,1,1,1,1,1,1,1,1,0,2,0,1,1,1,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,1,1,0,0,3,0,0,1,1,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1},
            {0,1,1,1,1,1,1,1,0,0,2,0,3,0,2,0,0,1,1,1,1,1,1,1,0},
            {0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0},
            {0,0,0,2,2,0,0,0,0,0,0,3,0,3,0,0,0,0,0,0,2,2,0,0,0},
        },
    },
    // ========== 关卡3：蝴蝶 ==========
    {
        // Easy：实心蝴蝶
        {
            {0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0},
            {0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0},
            {0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0},
            {0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0},
            {0,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,0},
            {1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1},
            {0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0},
            {0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0},
            {0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0},
            {0,0,0,0,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,1,1,0,0,0},
        },
        // Normal：蝴蝶+障碍
        {
            {0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0},
            {0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0},
            {0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0},
            {0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0},
            {0,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,0},
            {1,1,1,0,0,0,0,0,0,0,1,2,1,2,1,0,0,0,0,0,0,0,1,1,1},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,1,1,1,0,2,0,1,1,1,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1},
            {0,1,1,1,1,1,1,1,1,0,0,2,0,2,0,0,1,1,1,1,1,1,1,1,0},
            {0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0},
            {0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0},
            {0,0,0,0,2,2,0,0,0,0,0,2,1,2,0,0,0,0,0,0,2,2,0,0,0},
        },
        // Hard：蝴蝶+障碍+恶魔
        {
            {0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0},
            {0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0},
            {0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0},
            {0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0},
            {0,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,0},
            {1,1,1,0,0,0,0,0,0,0,1,2,1,2,1,0,0,0,0,0,0,0,1,1,1},
            {1,1,1,1,1,1,1,1,1,1,1,3,1,3,1,1,1,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,1,1,1,0,2,0,1,1,1,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,1,1,0,0,3,0,0,1,1,1,1,1,1,1,1,1,1},
            {0,1,1,1,1,1,1,1,1,0,0,2,0,2,0,0,1,1,1,1,1,1,1,1,0},
            {0,0,1,1,1,1,1,1,0,0,0,0,3,0,0,0,0,1,1,1,1,1,1,0,0},
            {0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0},
            {0,0,0,0,2,2,0,0,0,0,0,3,1,3,0,0,0,0,0,0,2,2,0,0,0},
        },
    },
};
// 线段与矩形碰撞检测函数
bool LineRectCollision(Vector2 start, Vector2 end, Rectangle rect, Vector2& hitPoint) {
    // 检查矩形的四条边
    Vector2 edges[4][2] = {
        {{rect.x, rect.y}, {rect.x + rect.width, rect.y}},
        {{rect.x + rect.width, rect.y}, {rect.x + rect.width, rect.y + rect.height}},
        {{rect.x + rect.width, rect.y + rect.height}, {rect.x, rect.y + rect.height}},
        {{rect.x, rect.y + rect.height}, {rect.x, rect.y}}
    };
    
    for (int i = 0; i < 4; i++) {
        Vector2 p1 = edges[i][0];
        Vector2 p2 = edges[i][1];
        
        float denom = (p2.y - p1.y) * (end.x - start.x) - (p2.x - p1.x) * (end.y - start.y);
        if (fabs(denom) < 0.0001f) continue;
        
        float ua = ((p2.x - p1.x) * (start.y - p1.y) - (p2.y - p1.y) * (start.x - p1.x)) / denom;
        float ub = ((end.x - start.x) * (start.y - p1.y) - (end.y - start.y) * (start.x - p1.x)) / denom;
        
        if (ua >= 0 && ua <= 1 && ub >= 0 && ub <= 1) {
            hitPoint.x = start.x + ua * (end.x - start.x);
            hitPoint.y = start.y + ua * (end.y - start.y);
            return true;
        }
    }
    return false;
}

Game::Game(int width, int height)
    : screenWidth(width), screenHeight(height), paddle(width, height), bricks(width)
{
    isChinese = false;
    for (int i = 0; i < MAX_PARTICLES; i++) {
        particleActive[i] = false;
    }
    currentBrickType = 0;
    editMode = false;
    saveButton = { (float)screenWidth/2 - 100, (float)screenHeight/2 - 100, 200, 50 };
    freezeTimer = 0;
    isFrozen = false;
    loadedCount = 0;
    totalTasks = 20;
    isLoading.store(false);
    pendingLevel = 0;
    pendingDifficulty = 0;
    menuButton = { (float)screenWidth/2 - 100, (float)screenHeight/2 + 110, 200, 50 };
    backToMenu = false;
    config = Config::load("config.json");
    steps = config.steps;
    InitWindow(screenWidth, screenHeight, "打砖块");
    LoadFont();
    powerUpTexture = LoadTexture("assets/images/powerup.png");
    
    for (int i = 0; i < 6; i++) {
        powerUpTimer[i] = 0.0f;
    }
    
    originalPaddleWidth = paddle.GetSize().x;

    balls.clear();
    balls.push_back(Ball());
    Reset();
    background = LoadTexture("assets/images/t04bc12585fc9f34567(1).png");
    
    paused = false;
    pauseButton = { (float)screenWidth - 50, 10, 40, 40 };
    continueButton = { (float)screenWidth/2 - 100, (float)screenHeight/2 - 30, 200, 50 };
    quitButton = { (float)screenWidth/2 - 100, (float)screenHeight/2 + 40, 200, 50 };
    
    isNetworkGame = false;
    networkMode = NetworkMode::NONE;
}


Game::~Game() {
    UnloadFont(chineseFont);
    UnloadTexture(background);
    UnloadTexture(powerUpTexture);
    CloseWindow();
}

/*void Game::LoadFont() {
    int codepoints[] = { 
        0x5F97, 0x5206, 0x6309, 0x7A7A, 0x683C, 0x952E, 0x5F00, 0x59CB, 0x6E38, 0x620F,
        0x5DE6, 0x53F3, 0x79FB, 0x52A8, 0x677F, 0x5B50, 0x7ED3, 0x675F, 0x6700, 0x7EC8,
        0x91CD, 0x65B0, 0x80DC, 0x5229, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39
    };
    int codepointCount = sizeof(codepoints) / sizeof(codepoints[0]);
    chineseFont = LoadFontEx("/mnt/c/Windows/Fonts/simhei.ttf", 48, codepoints, codepointCount);
}*/
void Game::LoadFont() {
    // 计算需要加载的字符数量
    int asciiCount = 127 - 32;
    int hanziCount = 0x9FFF - 0x4E00 + 1;
    int totalCount = asciiCount + hanziCount;
    
    // 动态分配数组
    int* codepoints = new int[totalCount];
    int index = 0;
    
    // 加载 ASCII 字符
    for (int i = 32; i < 127; i++) {
        codepoints[index++] = i;
    }
    
    // 加载常用汉字
    for (int i = 0x4E00; i <= 0x9FFF; i++) {
        codepoints[index++] = i;
    }
    
    chineseFont = LoadFontEx("/mnt/c/Windows/Fonts/simhei.ttf", 48, codepoints, totalCount);
    
    // 释放内存
    delete[] codepoints;
}
void Game::Reset() {
    powerUps.clear();
    for (int i = 0; i < 6; i++) {
        powerUpTimer[i] = 0.0f;
    }
    
    balls.clear();
    balls.push_back(Ball());
    balls[0].Reset();
    
    paddle.Reset();
    paddle.SetWidth(originalPaddleWidth);
    
    state = GameState::WAITING;
    score = 0;
    lives = config.initialLives;
    int startX = (screenWidth - 12 * 43) / 2;
    state = GameState::WAITING;
    score = 0;
    lives = config.initialLives;
}
void Game::ProcessInput() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, pauseButton)) {
            paused = !paused;
        }
    }
    
    if (paused) return;
    
    switch (state) {
    case GameState::WAITING:
        if (!editMode && IsKeyPressed(KEY_SPACE)) {
            state = GameState::PLAYING;
            for (auto& b : balls) {
                b.Start(config.ballSpeedX, config.ballSpeedY);
            }
        }
        break;
        
    case GameState::PLAYING:
        if (isNetworkGame && networkMode == NetworkMode::CLIENT) {
            paddle.Update(GetFrameTime());
            ::GameState myState;
            memset(&myState, 0, sizeof(myState));
            myState.paddle2X = paddle.GetPosition().x;
            network.SendGameState(myState);
        }
        if (IsKeyPressed(KEY_E)) {
            editMode = !editMode;
            if (editMode) {
                balls.clear();
            }
        }
        if (editMode) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) EditClick();
            if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
                currentBrickType = (currentBrickType + 1) % 4;
            }
            if (IsKeyPressed(KEY_S)) SaveEditToJSON();
            return;
        }
        break;
        
    case GameState::GAME_OVER_MENU:
        if (IsKeyPressed(KEY_R)) {
            int startX = (screenWidth - 12 * 43) / 2;
            bricks.LoadPattern(g_levelPatterns[currentLevel][currentDifficulty], startX, 100);
            balls.clear();
            balls.push_back(Ball());
            balls[0].Reset();
            paddle.Reset();
            paddle.SetWidth(originalPaddleWidth);
            state = GameState::WAITING;
            score = 0;
            lives = config.initialLives;
        }
        if (IsKeyPressed(KEY_Q)) {
            SaveProgress();
            backToMenu = true;
        }
        break;
        
    case GameState::LEVEL_COMPLETE:
        if (IsKeyPressed(KEY_N)) {
            NextLevel();
            SaveProgress();
        }
        if (IsKeyPressed(KEY_Q)) {
            SaveProgress();
            backToMenu = true;
        }
        break;
        
    default:
        break;
    }
}
void Game::Update(float dt) {
    // 暂停菜单的按钮检测
    if (paused) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mousePos = GetMousePosition();
            if (CheckCollisionPointRec(mousePos, continueButton)) {
                paused = false;
            }
            if (CheckCollisionPointRec(mousePos, saveButton)) {
                SaveProgress();
            }
            if (CheckCollisionPointRec(mousePos, menuButton)) {
                paused = false;
                backToMenu = true;
            }
            if (CheckCollisionPointRec(mousePos, quitButton)) {
                paused = false;
                state = GameState::WAITING;
                Reset();
            }
        }
        return;
    }

    bricks.UpdateParticles(dt);

    if (flash.life > 0) {
        flash.life -= dt;
    }

    for (int i = floatingTexts.size() - 1; i >= 0; i--) {
        floatingTexts[i].pos.y -= 50 * dt;
        floatingTexts[i].life -= dt;
        if (floatingTexts[i].life <= 0) {
            floatingTexts.erase(floatingTexts.begin() + i);
        }
    }

    /*for (int i = effectParticles.size() - 1; i >= 0; i--) {
        effectParticles[i].pos.x += effectParticles[i].vel.x * dt;
        effectParticles[i].pos.y += effectParticles[i].vel.y * dt;
        effectParticles[i].life -= dt;
        if (effectParticles[i].life <= 0) {
            effectParticles.erase(effectParticles.begin() + i);
        }
    }*/
    //粒子更新改成对象池
    for (int i = 0; i < MAX_PARTICLES; i++) {
    if (particleActive[i]) {
        particlePool[i].pos.x += particlePool[i].vel.x * dt;
        particlePool[i].pos.y += particlePool[i].vel.y * dt;
        particlePool[i].life -= dt;
        if (particlePool[i].life <= 0) {
            particleActive[i] = false;
        }
    }
}

    UpdatePowerUps(dt);

    // 网络更新
    if (isNetworkGame) {
        network.Update();
        UpdateNetwork();
    }

    if (state == GameState::PLAYING) {
        if (editMode) return;
        paddle.Update(dt);

        if (isNetworkGame && networkMode == NetworkMode::CLIENT) {
            return;
        }

        float stepDt = dt / steps;

        for (auto& ball : balls) {
            // 冰冻：球停止运动
            if (isFrozen) {
                ball.SetSpeed({0, 0});
            }
            for (int step = 0; step < steps; step++) {
                Vector2 oldPos = ball.GetPosition();
                ball.Update(stepDt);
                Vector2 ballPos = ball.GetPosition();
                float ballRadius = ball.GetRadius();

                if (ballPos.x - ballRadius < 0) {
                    ball.SetPositionX(ballRadius);
                    ball.BounceX();
                }
                if (ballPos.x + ballRadius > screenWidth) {
                    ball.SetPositionX(screenWidth - ballRadius);
                    ball.BounceX();
                }
                if (ballPos.y - ballRadius < 0) {
                    ball.SetPositionY(ballRadius);
                    ball.BounceY();
                }

                Rectangle paddleRect = paddle.GetRect();
                Rectangle expandedRect = {
                    paddleRect.x - ballRadius,
                    paddleRect.y - ballRadius,
                    paddleRect.width + ballRadius * 2,
                    paddleRect.height + ballRadius * 2
                };
                Vector2 hitPoint;
                if (LineRectCollision(oldPos, ballPos, expandedRect, hitPoint)) {
                    if (ball.GetSpeed().y > 0) {
                        float hitPos = (hitPoint.x - paddle.GetPosition().x) / (paddle.GetSize().x / 2);
                        if (hitPos > 1) hitPos = 1;
                        if (hitPos < -1) hitPos = -1;
                        ball.BounceY();
                        ball.AddSpeedX(hitPos * 150);
                        ball.ClampSpeed(500);
                        ball.SetPositionY(paddleRect.y - ballRadius);
                    }
                }
                //加计时，优化


                Vector2 ballSpeed = ball.GetSpeed();
                bool dropPowerUp = false, hitEvil = false, hitExplosive = false;
                Vector2 dropPos;
                bool hit = bricks.CheckCollision(ballPos, ballRadius, ballSpeed, score,dropPowerUp, dropPos, hitEvil, hitExplosive);


                if (hitEvil) {
                    if (!ball.IsInvincible()) {
                        state = GameState::GAME_OVER_MENU;
                        ball.Stop();
                        return;
                    }
                }
                if (hit) ball.SetSpeed(ballSpeed);
                if (dropPowerUp) {
                    int type;
                    int rand = GetRandomValue(0, 100);
                    if (rand < 8) type = 5;
                    else if (rand < 48) type = 2;
                    else if (rand < 50) type = 0;
                    else if (rand < 55) type = 3;
                    else if (rand < 75) type = 4;
                    else type = 1;
                    SpawnPowerUp(dropPos, type);
                }
            }
        }

        for (int i = balls.size() - 1; i >= 0; i--) {
            if (balls[i].GetPosition().y + balls[i].GetRadius() > screenHeight) {
                if (balls[i].IsInvincible()) {
                    balls[i].SetPositionY(screenHeight - balls[i].GetRadius());
                    balls[i].BounceY();
                    flash.intensity = 0.5f;
                    flash.life = 0.2f;
                } else {
                    balls.erase(balls.begin() + i);
                }
            }
        }

        if (balls.empty()) {
            lives--;
            if (lives <= 0) {
                state = GameState::GAME_OVER_MENU;
            } else {
                balls.clear();
                balls.push_back(Ball());
                balls[0].Reset();
                paddle.Reset();
                state = GameState::WAITING;
            }
        }

        if (!editMode && bricks.AllCleared()) {
            state = GameState::LEVEL_COMPLETE;
        }
    }
}
/*void Game::Update(float dt) {
    // 暂停菜单的按钮检测
    if (paused) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mousePos = GetMousePosition();
            if (CheckCollisionPointRec(mousePos, continueButton)) {
                paused = false;
            }
            if (CheckCollisionPointRec(mousePos, quitButton)) {
                paused = false;
                state = GameState::WAITING;
                Reset();
            }
            if (CheckCollisionPointRec(mousePos, menuButton)) {
                paused = false;
                backToMenu = true;
            }
        }
        return;
    }

    bricks.UpdateParticles(dt);

    if (flash.life > 0) {
        flash.life -= dt;
    }

    for (int i = floatingTexts.size() - 1; i >= 0; i--) {
        floatingTexts[i].pos.y -= 50 * dt;
        floatingTexts[i].life -= dt;
        if (floatingTexts[i].life <= 0) {
            floatingTexts.erase(floatingTexts.begin() + i);
        }
    }

    for (int i = effectParticles.size() - 1; i >= 0; i--) {
        effectParticles[i].pos.x += effectParticles[i].vel.x * dt;
        effectParticles[i].pos.y += effectParticles[i].vel.y * dt;
        effectParticles[i].life -= dt;
        if (effectParticles[i].life <= 0) {
            effectParticles.erase(effectParticles.begin() + i);
        }
    }

    UpdatePowerUps(dt);

    if (isNetworkGame) {
        network.Update();
        UpdateNetwork();
    }

    if (state == GameState::PLAYING) {
        paddle.Update(dt);

        if (isNetworkGame && networkMode == NetworkMode::CLIENT) {
            return;
        }

        float stepDt = dt / steps;

        for (auto& ball : balls) {
            if (isFrozen) {
                ball.SetSpeed({0, 0});
            }
            for (int step = 0; step < steps; step++) {
                Vector2 oldPos = ball.GetPosition();
                ball.Update(stepDt);
                Vector2 ballPos = ball.GetPosition();
                float ballRadius = ball.GetRadius();

                if (ballPos.x - ballRadius < 0) {
                    ball.SetPositionX(ballRadius);
                    ball.BounceX();
                }
                if (ballPos.x + ballRadius > screenWidth) {
                    ball.SetPositionX(screenWidth - ballRadius);
                    ball.BounceX();
                }
                if (ballPos.y - ballRadius < 0) {
                    ball.SetPositionY(ballRadius);
                    ball.BounceY();
                }

                Rectangle paddleRect = paddle.GetRect();
                Rectangle expandedRect = {
                    paddleRect.x - ballRadius,
                    paddleRect.y - ballRadius,
                    paddleRect.width + ballRadius * 2,
                    paddleRect.height + ballRadius * 2
                };
                Vector2 hitPoint;
                if (LineRectCollision(oldPos, ballPos, expandedRect, hitPoint)) {
                    if (ball.GetSpeed().y > 0) {
                        float hitPos = (hitPoint.x - paddle.GetPosition().x) / (paddle.GetSize().x / 2);
                        if (hitPos > 1) hitPos = 1;
                        if (hitPos < -1) hitPos = -1;
                        ball.BounceY();
                        ball.AddSpeedX(hitPos * 150);
                        ball.ClampSpeed(500);
                        ball.SetPositionY(paddleRect.y - ballRadius);
                    }
                }

                Vector2 ballSpeed = ball.GetSpeed();
                bool dropPowerUp = false, hitEvil = false, hitExplosive = false;
                Vector2 dropPos;
                bool hit = bricks.CheckCollision(ballPos, ballRadius, ballSpeed, score,
                                                  dropPowerUp, dropPos, hitEvil, hitExplosive);
                if (hitEvil) {
                    if (!ball.IsInvincible()) {
                        state = GameState::GAME_OVER_MENU;
                        ball.Stop();
                        return;
                    }
                }
                if (hit) ball.SetSpeed(ballSpeed);
                if (dropPowerUp) {
                    int type = 2;  // 测试：强制火焰分裂
                    SpawnPowerUp(dropPos, type);
                }
            }
        }

        for (int i = balls.size() - 1; i >= 0; i--) {
            if (balls[i].GetPosition().y + balls[i].GetRadius() > screenHeight) {
                if (balls[i].IsInvincible()) {
                    balls[i].SetPositionY(screenHeight - balls[i].GetRadius());
                    balls[i].BounceY();
                    flash.intensity = 0.5f;
                    flash.life = 0.2f;
                } else {
                    balls.erase(balls.begin() + i);
                }
            }
        }

        if (balls.empty()) {
            lives--;
            if (lives <= 0) {
                state = GameState::GAME_OVER_MENU;
            } else {
                balls.clear();
                balls.push_back(Ball());
                balls[0].Reset();
                paddle.Reset();
                state = GameState::WAITING;
            }
        }

        if (bricks.AllCleared()) {
            state = GameState::LEVEL_COMPLETE;
        }
    }
}测试代码*/
void Game::DrawChineseText(const char* text, int x, int y, int fontSize, Color color) {
    DrawTextEx(chineseFont, text, (Vector2){ (float)x, (float)y }, fontSize, 2, color);
}
void Game::Draw() {
    BeginDrawing();
    
    float scaleX = (float)screenWidth / background.width;
    float scaleY = (float)screenHeight / background.height;
    float scale = (scaleX > scaleY) ? scaleX : scaleY;
    float drawX = (screenWidth - background.width * scale) / 2;
    float drawY = (screenHeight - background.height * scale) / 2;
    DrawTextureEx(background, (Vector2){drawX, drawY}, 0, scale, WHITE);
    
    bricks.Draw();
    
    for (auto& b : balls) {
        b.Draw();
    }
    
    if (isNetworkGame) {
        Color myColor = (networkMode == NetworkMode::HOST) ? BLUE : RED;
        DrawRectangle(paddle.GetPosition().x - paddle.GetSize().x/2,
                      paddle.GetPosition().y,
                      paddle.GetSize().x, paddle.GetSize().y,
                      myColor);
    } else {
        paddle.Draw();
    }
    
    if (isNetworkGame && network.Connected()) {
        float opponentX = (networkMode == NetworkMode::HOST) ? clientPaddleX : hostPaddleX;
        Color oppColor = (networkMode == NetworkMode::HOST) ? RED : BLUE;
        DrawRectangle(opponentX - paddle.GetSize().x/2,
                      paddle.GetPosition().y,
                      paddle.GetSize().x, paddle.GetSize().y,
                      oppColor);
    }
    
    DrawPowerUps();
    
    char scoreText[50] = {0};
    sprintf(scoreText, "得分: %d", score);
    DrawChineseText(scoreText, 10, 10, 28, WHITE);
    DrawHearts();
    
    if (state == GameState::WAITING) {
        DrawChineseText("按空格键开始游戏", screenWidth/2 - 130, screenHeight/2, 28, YELLOW);
        DrawChineseText("左右键移动板子", screenWidth/2 - 100, screenHeight/2 + 50, 24, WHITE);
    }
    
    if (state == GameState::GAME_OVER_MENU) {
        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.7f));
        DrawChineseText("游戏结束", screenWidth/2 - 70, screenHeight/2 - 80, 48, RED);
        char finalScore[50] = {0};
        sprintf(finalScore, "最终得分: %d", score);
        DrawChineseText(finalScore, screenWidth/2 - 80, screenHeight/2 - 30, 28, WHITE);
        DrawChineseText("按 R 重新开始本关", screenWidth/2 - 110, screenHeight/2 + 20, 24, GOLD);
        DrawChineseText("按 Q 返回主菜单", screenWidth/2 - 110, screenHeight/2 + 55, 24, GOLD);
    }
    
    if (state == GameState::LEVEL_COMPLETE) {
        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.7f));
        DrawChineseText("过关!", screenWidth/2 - 40, screenHeight/2 - 80, 56, GREEN);
        char finalScore[50] = {0};
        sprintf(finalScore, "得分: %d", score);
        DrawChineseText(finalScore, screenWidth/2 - 60, screenHeight/2 - 30, 28, WHITE);
        DrawChineseText("按 N 进入下一关", screenWidth/2 - 90, screenHeight/2 + 20, 24, GOLD);
        DrawChineseText("按 Q 返回主菜单", screenWidth/2 - 90, screenHeight/2 + 55, 24, GOLD);
    }
    
    if (flash.life > 0) {
        float alpha = flash.intensity * flash.life * 2;
        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(WHITE, alpha));
    }
    
    for (const auto& ft : floatingTexts) {
        int fontSize = 28 * (1.0f + (1.0f - ft.life));
        DrawTextEx(chineseFont, ft.text, ft.pos, fontSize, 2, Fade(ft.color, ft.life));
    }
    
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particleActive[i]) {
            float size = particlePool[i].size * (1.0f + (1.0f - particlePool[i].life));
            DrawCircleV(particlePool[i].pos, size, Fade(particlePool[i].color, particlePool[i].life * 1.5f));
        }
    }
    
    if (isFrozen) {
        float time = GetTime();
        float progress = 1.0f - (freezeTimer / 4.0f);
        float slowTime = time * 0.6f;
        DrawRectangle(0, 0, screenWidth, screenHeight, Fade((Color){100, 175, 230, 255}, 0.04f + progress * 0.07f));
        for (int i = 0; i < 24; i++) {
            float cx, cy;
            if (i < 8) { cx = screenWidth * (i / 7.0f); cy = -15; }
            else if (i < 13) { cx = screenWidth + 15; cy = screenHeight * ((i - 8) / 4.0f); }
            else if (i < 19) { cx = screenWidth * (1.0f - (i - 13) / 5.0f); cy = screenHeight + 15; }
            else { cx = -15; cy = screenHeight * (1.0f - (i - 19) / 4.0f); }
            float breathe = sin(slowTime * 1.2f + i * 0.5f) * 0.3f + 0.7f;
            float sz = 35 + progress * 65 * breathe;
            float alpha = 0.55f + progress * 0.45f * breathe;
            Vector2 center = {cx, cy};
            for (int petal = 0; petal < 10; petal++) {
                float baseAngle = petal * 36.0f * DEG2RAD + slowTime * 5.0f * DEG2RAD;
                float len = sz * (0.55f + 0.45f * sin(slowTime * 1.5f + petal * 0.8f));
                float width = sz * 0.25f;
                Vector2 tip = { cx + cos(baseAngle) * len, cy + sin(baseAngle) * len };
                Vector2 mid = { cx + cos(baseAngle) * len * 0.55f, cy + sin(baseAngle) * len * 0.55f };
                Vector2 s1 = { mid.x + cos(baseAngle + 90*DEG2RAD) * width, mid.y + sin(baseAngle + 90*DEG2RAD) * width };
                Vector2 s2 = { mid.x + cos(baseAngle - 90*DEG2RAD) * width, mid.y + sin(baseAngle - 90*DEG2RAD) * width };
                DrawTriangle(center, s1, s2, Fade((Color){195, 225, 250, 255}, alpha * 0.55f));
                DrawTriangle(center, tip, s1, Fade((Color){230, 245, 255, 255}, alpha * 0.7f));
                DrawTriangle(center, tip, s2, Fade((Color){170, 210, 245, 255}, alpha * 0.4f));
                DrawLineEx(center, tip, 2.0f, Fade(WHITE, alpha * 0.6f));
                float sideLen = len * 0.55f;
                for (int side = -1; side <= 1; side += 2) {
                    Vector2 branchBase = { cx + cos(baseAngle) * len * 0.5f, cy + sin(baseAngle) * len * 0.5f };
                    float branchAngle = baseAngle + side * 58.0f * DEG2RAD;
                    Vector2 branchTip = { branchBase.x + cos(branchAngle) * sideLen, branchBase.y + sin(branchAngle) * sideLen };
                    DrawLineEx(branchBase, branchTip, 1.5f, Fade(WHITE, alpha * 0.5f));
                    float subLen = sideLen * 0.5f;
                    Vector2 subTip = { branchTip.x + cos(branchAngle + side * 30*DEG2RAD) * subLen, branchTip.y + sin(branchAngle + side * 30*DEG2RAD) * subLen };
                    DrawLineEx(branchTip, subTip, 1.0f, Fade(WHITE, alpha * 0.35f));
                }
            }
            DrawCircleV(center, sz * 0.25f, Fade(WHITE, alpha * 0.95f));
            DrawCircleV(center, sz * 0.5f, Fade((Color){220, 240, 255, 255}, alpha * 0.4f));
        }
        Vector2 cnrs[] = {{50,50}, {(float)screenWidth-50, 50}, {(float)screenWidth-50, (float)screenHeight-50}, {50, (float)screenHeight-50}};
        for (int c = 0; c < 4; c++) {
            float breathe = sin(slowTime * 1.0f + c) * 0.25f + 0.75f;
            float sz = 55 + progress * 70 * breathe;
            float alpha = 0.65f + progress * 0.35f * breathe;
            for (int petal = 0; petal < 12; petal++) {
                float angle = petal * 30.0f * DEG2RAD;
                float len = sz * (0.6f + 0.4f * sin(slowTime * 1.8f + petal * 0.7f));
                Vector2 tip = { cnrs[c].x + cos(angle) * len, cnrs[c].y + sin(angle) * len };
                Vector2 mid = { cnrs[c].x + cos(angle) * len * 0.5f, cnrs[c].y + sin(angle) * len * 0.5f };
                float w = sz * 0.18f;
                Vector2 s1 = { mid.x + cos(angle + 90*DEG2RAD) * w, mid.y + sin(angle + 90*DEG2RAD) * w };
                Vector2 s2 = { mid.x + cos(angle - 90*DEG2RAD) * w, mid.y + sin(angle - 90*DEG2RAD) * w };
                DrawTriangle(cnrs[c], s1, s2, Fade((Color){200, 230, 250, 255}, alpha * 0.55f));
                DrawTriangle(cnrs[c], tip, s1, Fade((Color){235, 248, 255, 255}, alpha * 0.65f));
                DrawTriangle(cnrs[c], tip, s2, Fade((Color){175, 215, 245, 255}, alpha * 0.4f));
                DrawLineEx(cnrs[c], tip, 2.0f, Fade(WHITE, alpha * 0.55f));
            }
            DrawCircleV(cnrs[c], sz * 0.2f, Fade(WHITE, alpha * 0.95f));
            DrawCircleV(cnrs[c], sz * 0.45f, Fade((Color){225, 245, 255, 255}, alpha * 0.35f));
        }
        for (const auto& pos : iceCrystals) {
            float sy = fmod(pos.y + time * 12.0f, screenHeight);
            float sx = pos.x + sin(time * 0.8f + pos.x) * 15;
            float sz = 3.0f + sin(time * 3 + pos.x) * 1.5f;
            DrawCircleV((Vector2){sx, sy}, sz * 4.0f, Fade((Color){160, 205, 240, 255}, 0.15f));
            DrawCircleV((Vector2){sx, sy}, sz * 2.5f, Fade((Color){195, 225, 250, 255}, 0.3f));
            DrawCircleV((Vector2){sx, sy}, sz, Fade(WHITE, 0.6f));
        }
        DrawRectangleLinesEx((Rectangle){4, 4, screenWidth-8, screenHeight-8}, 4, Fade((Color){130, 185, 230, 255}, 0.18f + progress * 0.3f));
    }
    
    if (isLoading.load()) {
        DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
        float boxX = screenWidth - 260;
        float boxY = screenHeight - 120;
        float time = GetTime();
        float cx = boxX + 35;
        float cy = boxY + 40;
        float radius = 18;
        DrawRing((Vector2){cx, cy}, radius - 4, radius + 4, 0, 360, 48, Fade(WHITE, 0.15f));
        for (int i = 0; i < 4; i++) {
            float startAngle = fmod(time * 200.0f + i * 90.0f, 360.0f);
            Color arcColor;
            if (i == 0) arcColor = (Color){255, 100, 100, 255};
            else if (i == 1) arcColor = (Color){100, 255, 100, 255};
            else if (i == 2) arcColor = (Color){100, 100, 255, 255};
            else arcColor = (Color){255, 255, 100, 255};
            DrawRing((Vector2){cx, cy}, radius - 3, radius + 3, startAngle, startAngle + 60, 36, Fade(arcColor, 0.9f));
        }
        int percent = (totalTasks > 0) ? (loadedCount * 100 / totalTasks) : 0;
        char progressText[16];
        sprintf(progressText, "Loading %d%%", percent);
        DrawText(progressText, boxX + 60, boxY + 8, 22, (Color){220, 220, 255, 255});
        float barX = boxX + 60;
        float barY = boxY + 36;
        float barW = 150;
        float barH = 14;
        DrawRectangle(barX, barY, barW, barH, Fade((Color){40, 40, 40, 255}, 0.6f));
        float fillW = barW * percent / 100.0f;
        for (int px = 0; px < (int)fillW; px++) {
            float t = (float)px / barW;
            Color barColor;
            barColor.r = (unsigned char)(255 * (1.0f - t) + 100 * t);
            barColor.g = (unsigned char)(100 * (1.0f - t) + 255 * t);
            barColor.b = (unsigned char)(100 * (1.0f - t) + 200 * t);
            barColor.a = 255;
            DrawRectangle(barX + px, barY, 1, barH, barColor);
        }
        DrawRectangle(barX, barY, fillW, barH / 3, Fade(WHITE, 0.15f));
        float dotX = barX + fillW;
        float dotY = barY + barH / 2;
        float pulse = sin(time * 6.0f) * 3.0f + 5.0f;
        DrawCircleV((Vector2){dotX, dotY}, pulse, Fade(WHITE, 0.8f));
        DrawCircleV((Vector2){dotX, dotY}, pulse * 0.5f, WHITE);
    }
    
    DrawRectangleRec(pauseButton, Fade(GRAY, 0.5f));
    DrawRectangle(pauseButton.x + 12, pauseButton.y + 10, 6, 20, WHITE);
    DrawRectangle(pauseButton.x + 22, pauseButton.y + 10, 6, 20, WHITE);
    
    if (paused) {
        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.7f));
        DrawChineseText(isChinese ? "暂停" : "PAUSED", screenWidth/2 - 40, screenHeight/2 - 160, 40, WHITE);
        
        DrawRectangleRec(continueButton, Fade(GREEN, 0.8f));
        const char* contText = isChinese ? "继续" : "Continue";
        DrawChineseText(contText, continueButton.x + (continueButton.width - MeasureText(contText, 24))/2, continueButton.y + 12, 24, WHITE);
        
        DrawRectangleRec(saveButton, Fade(BLUE, 0.8f));
        const char* saveText = isChinese ? "保存" : "Save";
        DrawChineseText(saveText, saveButton.x + (saveButton.width - MeasureText(saveText, 24))/2, saveButton.y + 12, 24, WHITE);
        
        DrawRectangleRec(menuButton, Fade(YELLOW, 0.8f));
        const char* menuText = isChinese ? "主菜单" : "Main Menu";
        DrawChineseText(menuText, menuButton.x + (menuButton.width - MeasureText(menuText, 24))/2, menuButton.y + 12, 24, WHITE);
        
        DrawRectangleRec(quitButton, Fade(RED, 0.8f));
        const char* quitText = isChinese ? "返回" : "Quit";
        DrawChineseText(quitText, quitButton.x + (quitButton.width - MeasureText(quitText, 24))/2, quitButton.y + 12, 24, WHITE);
    }
    if (editMode) {
        const char* typeNames[] = {"普通", "爆炸", "恶魔", "障碍"};
        char buf[64];
        sprintf(buf, "编辑 - %s (右键切换) S保存 E退出", typeNames[currentBrickType]);
        DrawChineseText(buf, 10, screenHeight - 30, 18, RED);
    }
    
    EndDrawing();
}

void Game::Run() {
    SetTargetFPS(60);
    currentLevel = 0;
    currentDifficulty = 0;
    
    bool hasSave = false;
    {
        std::ifstream file("save.json");
        if (file.is_open()) {
            json save;
            file >> save;
            currentLevel = save["level"];
            currentDifficulty = save["diff"];
            score = save["score"];
            lives = save["lives"];
            hasSave = true;
        }
    }
    
    bool running = true;
    while (running && !WindowShouldClose()) {
        Menu menu;
        menu.Init();
        if (hasSave) {
            menu.hasSave = true;
            menu.savedLevel = currentLevel;
            menu.savedScore = score;
            menu.savedLives = lives;
        }
        
        while (menu.inMenu && !WindowShouldClose()) {
            menu.Update();
            BeginDrawing();
            ClearBackground(BLACK);
            menu.Draw(chineseFont);
            EndDrawing();
        }
        
        if (WindowShouldClose()) break;
        isChinese = menu.isChinese;
        
        if (menu.loadSave) {
            hasSave = false;
        } else {
            currentLevel = menu.selectedLevel;
            currentDifficulty = menu.selectedDiff;
            score = 0;
            lives = config.initialLives;
        }
        
        switch (menu.mode) {
            case GameMode::SINGLE_PLAYER:
                isNetworkGame = false;
                networkMode = NetworkMode::NONE;
                network.Disconnect();
                break;
            case GameMode::HOST:
                isNetworkGame = true;
                networkMode = NetworkMode::HOST;
                network.InitAsHost();
                break;
            case GameMode::CLIENT:
                isNetworkGame = true;
                networkMode = NetworkMode::CLIENT;
                network.InitAsClient("127.0.0.1");
                break;
        }
        int startX = (screenWidth - 12 * 43) / 2;
        
        if (menu.selectedLevel == 3 && !menu.loadSave) {
            editMode = true;
            currentLevel = 3;
            bricks.clear();
            state = GameState::PLAYING;
            balls.clear();
            score = 0;
            lives = config.initialLives;
            powerUps.clear();
        } else {
            std::string levelPath = "levels/level" + std::to_string(currentLevel + 1) + ".json";
            std::vector<std::vector<int>> layout = LoadLevelFromJSON(levelPath);
            if (!layout.empty()) {
                bricks.LoadPattern(layout, startX, 100);
            } else {
                bricks.LoadPattern(g_levelPatterns[currentLevel][currentDifficulty], startX, 100);
            }
            
            if (menu.loadSave) {
                std::ifstream file("save.json");
                if (file.is_open()) {
                    json save;
                    file >> save;
                    if (save.contains("bricks")) {
                        int i = 0;
                        for (auto& act : save["bricks"]) {
                            bricks.SetBrickType(i, act.get<int>());
                            i++;
                        }
                    }
                    if (save.contains("paddle_x")) {
                        paddle.SetPosition({save["paddle_x"], paddle.GetPosition().y});
                        paddle.SetWidth(save["paddle_w"]);
                    }
                }
                state = GameState::WAITING;
            } else {
                Reset();
            }
        }
        
        backToMenu = false;
        
        while (!WindowShouldClose() && !backToMenu) {
            float dt = GetFrameTime();
            
            if (isNetworkGame) {
                network.Update();
                UpdateNetwork();
            }
            if (isLoading.load()) {
                CheckAsyncLoad();
                Draw();
                continue;
            }
            
            ProcessInput();
            Update(dt);
            Draw();
        }
    }
}
void Game::SpawnPowerUp(Vector2 pos, int type) {
    PowerUp p;
    p.position = pos;
    p.active = true;
    p.type = type;
    p.speedY = 100.0f;
    powerUps.push_back(p);
}

void Game::ApplyPowerUp(int type) {
    Vector2 center = paddle.GetPosition();
    center.y -= 20;
    
    switch (type) {
        case 0: // 加长板子
        {
            originalPaddleWidth = paddle.GetSize().x;
            paddle.SetWidth(originalPaddleWidth * 2.0f);
            powerUpTimer[0] = 6.0f;
            for (int i = 0; i < 80; i++) {
                EffectParticle p;
                p.pos = center;
                float angle = (float)GetRandomValue(0, 360) * DEG2RAD;
                float speed = (float)GetRandomValue(200, 600);
                p.vel = { (float)(cos(angle) * speed), (float)(sin(angle) * speed) };
                p.life = 0.8f;
                p.size = 4;
                p.color = GOLD;
                SpawnParticle(p.pos, p.vel, p.life, p.size, p.color);
            }
            flash.intensity = 0.8f;
            flash.life = 0.3f;
            break;
        }
            
        case 1: // 缩短板子
        {
            originalPaddleWidth = paddle.GetSize().x;
            paddle.SetWidth(originalPaddleWidth * 0.5f);
            powerUpTimer[1] = 5.0f;
            for (int i = 0; i < 60; i++) {
                EffectParticle p;
                p.pos = center;
                float angle = (float)GetRandomValue(0, 360) * DEG2RAD;
                float speed = (float)GetRandomValue(150, 450);
                p.vel = { (float)(cos(angle) * speed), (float)(sin(angle) * speed) };
                p.life = 0.6f;
                p.size = 5;
                p.color = DARKPURPLE;
                SpawnParticle(p.pos, p.vel, p.life, p.size, p.color);
            }
            break;
        }
            
        case 2: // 多球
        {
            for (int r = 0; r < 3; r++) {
                for (int i = 0; i < 120; i++) {
                    EffectParticle p;
                    p.pos = center;
                    float angle = (float)GetRandomValue(0, 360) * DEG2RAD;
                    float speed = (float)GetRandomValue(300, 800);
                    p.vel = { (float)(cos(angle) * speed), (float)(sin(angle) * speed) };
                    p.life = 0.7f - r * 0.1f;
                    p.size = (r == 0) ? 6 : 3;
                    if (r == 0) p.color = ORANGE;
                    else if (r == 1) p.color = YELLOW;
                    else p.color = RED;
                    SpawnParticle(p.pos, p.vel, p.life, p.size, p.color);
                }
            }
            
            int newBallsCount = GetRandomValue(8, 12);
            for (int i = 0; i < newBallsCount; i++) {
                Ball newBall = balls[0];
                
                float angle;
                if (i % 2 == 0) {
                    angle = (30.0f + (float)(i/2) * (120.0f / (newBallsCount/2))) * DEG2RAD;
                } else {
                    angle = (210.0f + (float)(i/2) * (120.0f / (newBallsCount/2))) * DEG2RAD;
                }
                
                float radius = 60.0f;
                Vector2 offset = { (float)(cos(angle) * radius), (float)(sin(angle) * radius) };
                newBall.SetPosition({ center.x + offset.x, center.y + offset.y });
                
                float speedX = (float)(cos(angle) * 350);
                float speedY = (float)(sin(angle) * 350);
                
                float minVerticalSpeed = 200.0f;
                if (fabs(speedY) < minVerticalSpeed) {
                    speedY = (speedY >= 0) ? minVerticalSpeed : -minVerticalSpeed;
                }
                
                Vector2 speed = { speedX, speedY };
                newBall.SetSpeed(speed);
                newBall.Start(speed.x, speed.y);
                balls.push_back(newBall);
            }
            
            shakeTime = 0.5f;
            flash.intensity = 1.0f;
            flash.life = 0.4f;
            
            FloatingText ft2;
        memset(ft2.text, 0, sizeof(ft2.text));
            ft2.pos = center;
            sprintf(ft2.text, "+%d 个球!", newBallsCount);
            ft2.life = 1.0f;
            ft2.color = YELLOW;
            floatingTexts.push_back(ft2);
            break;
        }
            
        case 3: // 冰冻效果
        {
            isFrozen = true;
            freezeTimer = 4.0f;
            powerUpTimer[3] = 4.0f;
            
            // 生成随机冰晶位置
            iceCrystals.clear();
            for (int i = 0; i < 12; i++) {
                float cx = GetRandomValue(20, screenWidth - 20);
                float cy = GetRandomValue(20, screenHeight - 20);
                iceCrystals.push_back({(float)cx, (float)cy});
            }
            
            // 冰晶粒子特效
            for (int i = 0; i < 40; i++) {
                EffectParticle p;
                p.pos = { (float)GetRandomValue(0, screenWidth), (float)GetRandomValue(0, screenHeight) };
                p.vel = { 0, 0 };
                p.life = 1.5f;
                p.size = (float)GetRandomValue(3, 8);
                p.color = (Color){ (unsigned char)GetRandomValue(100, 200), 
                                   (unsigned char)GetRandomValue(180, 255), 255, 200 };
                SpawnParticle(p.pos, p.vel, p.life, p.size, p.color);
            }
            break;
        }
        case 4: // 加分
        {
            score += 100;
            for (int i = 0; i < 100; i++) {
                EffectParticle p;
                p.pos = center;
                float angle = (float)GetRandomValue(0, 360) * DEG2RAD;
                float speed = (float)GetRandomValue(200, 500);
                p.vel = { (float)(cos(angle) * speed), (float)(sin(angle) * speed) };
                p.life = 0.6f;
                p.size = (i % 2 == 0) ? 5 : 3;
                p.color = GOLD;
                SpawnParticle(p.pos, p.vel, p.life, p.size, p.color);
            }
            FloatingText ft4;
        memset(ft4.text, 0, sizeof(ft4.text));
            ft4.pos = center;
            sprintf(ft4.text, "+100 分");
            ft4.life = 1.0f;
            ft4.color = GOLD;
            floatingTexts.push_back(ft4);
            break;
        }
            
        case 5: // 无敌球
        {
            for (auto& b : balls) {
                b.SetInvincible(true, 8.0f);
            }
            powerUpTimer[5] = 8.0f;
            
            for (int i = 0; i < 80; i++) {
                EffectParticle p;
                p.pos = center;
                float angle = (float)GetRandomValue(0, 360) * DEG2RAD;
                float speed = (float)GetRandomValue(200, 600);
                p.vel = { (float)(cos(angle) * speed), (float)(sin(angle) * speed) };
                p.life = 0.8f;
                p.size = 5;
                p.color = (Color){ 
                    (unsigned char)GetRandomValue(100, 255), 
                    (unsigned char)GetRandomValue(100, 255), 
                    (unsigned char)GetRandomValue(100, 255), 
                    255 
                };
                SpawnParticle(p.pos, p.vel, p.life, p.size, p.color);
            }
            
            flash.intensity = 1.0f;
            flash.life = 0.5f;
            
            FloatingText ft5;
        memset(ft5.text, 0, sizeof(ft5.text));
            ft5.pos = center;
            sprintf(ft5.text, "无敌!");
            ft5.life = 1.5f;
            ft5.color = (Color){ 255, 215, 0, 255 };
            floatingTexts.push_back(ft5);
            break;
        }
    }
}

void Game::UpdatePowerUps(float dt) {
    // 更新计时器
    for (int i = 0; i < 6; i++) {
        if (powerUpTimer[i] > 0) {
            powerUpTimer[i] -= dt;
            if (powerUpTimer[i] <= 0) {
                if (i == 0 || i == 1) {
                    paddle.SetWidth(originalPaddleWidth);
                }
                if (i == 3) {
                    isFrozen = false;
                    iceCrystals.clear();
                    for (auto& b : balls) {
                        b.SetSpeed(b.GetOriginalSpeed());
                    }
                }
                if (i == 5) {
                    for (auto& b : balls) {
                        b.SetInvincible(false, 0);
                    }
                }
            }
        }
    }
    
    // 冰冻计时
    if (isFrozen) {
        freezeTimer -= dt;
        if (freezeTimer <= 0) {
            isFrozen = false;
            iceCrystals.clear();
        }
    }
    
    // 更新每个球的无敌状态
    for (auto& b : balls) {
        b.UpdateInvincible(dt);
    }
    
    // 更新道具位置和碰撞
    for (auto& p : powerUps) {
        if (!p.active) continue;
        
        p.position.y += p.speedY * dt;
        
        if (p.position.y > screenHeight) {
            p.active = false;
            continue;
        }
        
        Rectangle paddleRect = paddle.GetRect();
        if (CheckCollisionCircleRec(p.position, 20, paddleRect)) {
            ApplyPowerUp(p.type);
            p.active = false;
        }
    }
    
    // 删除失效道具
    powerUps.erase(remove_if(powerUps.begin(), powerUps.end(),
        [](const PowerUp& p) { return !p.active; }), powerUps.end());
}

void Game::DrawPowerUps() {
    for (const auto& p : powerUps) {
        if (p.active) {
            if (p.type == 3) {  // 冰冻冰块
                float time = GetTime();
                float breathe = sin(time * 2.0f) * 0.15f + 0.85f;
                float x = p.position.x;
                float y = p.position.y;
                float w = 24;
                float h = 28;
                
                // 冰块主体——浅蓝白半透明
                DrawRectangleRounded(
                    (Rectangle){x - w/2, y - h/2, w, h},
                    0.25f, 12, Fade((Color){180, 220, 250, 255}, 0.7f * breathe)
                );
                
                // 内部透亮
                DrawRectangleRounded(
                    (Rectangle){x - w/2 + 3, y - h/2 + 2, w - 6, h - 8},
                    0.2f, 12, Fade(WHITE, 0.25f)
                );
                
                // 顶面高光（模拟冰块反光）
                DrawRectangleRounded(
                    (Rectangle){x - w/2 + 4, y - h/2 + 1, w - 8, h * 0.35f},
                    0.15f, 8, Fade(WHITE, 0.5f)
                );
                
                // 侧棱高光
                DrawLineEx((Vector2){x - w/2 + 5, y - h/2 + 3}, (Vector2){x - w/2 + 5, y + h/2 - 5}, 2.5f, Fade(WHITE, 0.4f));
                DrawLineEx((Vector2){x + w/2 - 5, y - h/2 + 3}, (Vector2){x + w/2 - 5, y + h/2 - 5}, 2.0f, Fade(WHITE, 0.25f));
                
                // 底部暗面
                DrawRectangleRounded(
                    (Rectangle){x - w/2 + 3, y + h/2 - 5, w - 6, 4},
                    0.1f, 6, Fade((Color){120, 180, 220, 255}, 0.3f)
                );
                
                // 冰块边框
                DrawRectangleLinesEx((Rectangle){x - w/2, y - h/2, w, h}, 1.5f, Fade(WHITE, 0.6f));
                
                // 内部六边形冰晶纹理
                DrawPoly((Vector2){x, y}, 6, 6, time * 20.0f, Fade(WHITE, 0.3f));
                DrawPoly((Vector2){x, y}, 6, 3, -time * 30.0f, Fade(WHITE, 0.5f));
                
                // 微光点
                DrawCircleV((Vector2){x - w/4, y - h/4}, 2.5f, Fade(WHITE, 0.7f));
                DrawCircleV((Vector2){x + w/5, y}, 2.0f, Fade(WHITE, 0.5f));
            }
            else if (p.type == 2) {  // 火焰爆炸
                float time = GetTime();
                float x = p.position.x;
                float y = p.position.y;
                float sz = 13;
                
                // 外层火焰光晕——多层柔光
                DrawCircleV(p.position, sz + 10, Fade((Color){255, 80, 0, 255}, 0.1f));
                DrawCircleV(p.position, sz + 6, Fade((Color){255, 130, 20, 255}, 0.15f));
                DrawCircleV(p.position, sz + 3, Fade((Color){255, 180, 40, 255}, 0.25f));
                
                // 火焰舌——细长三角，颜色从白到红
                for (int i = 0; i < 9; i++) {
                    float angle = i * 40.0f * DEG2RAD + time * 2.5f;
                    float flicker = sin(time * 10.0f + i * 1.7f) * 0.3f + 0.7f;
                    float len = sz * (1.0f + 0.6f * flicker);
                    float tipX = x + cos(angle) * len;
                    float tipY = y + sin(angle) * len;
                    
                    // 内侧亮白
                    float innerLen = len * 0.6f;
                    float ix = x + cos(angle) * innerLen;
                    float iy = y + sin(angle) * innerLen;
                    
                    // 火焰主体——从白到橙到红
                    DrawTriangle(
                        p.position,
                        (Vector2){x + cos(angle + 0.35f) * sz * 0.5f, y + sin(angle + 0.35f) * sz * 0.5f},
                        (Vector2){tipX, tipY},
                        Fade((Color){255, 180, 10, 255}, 0.75f)
                    );
                    DrawTriangle(
                        p.position,
                        (Vector2){x + cos(angle - 0.35f) * sz * 0.5f, y + sin(angle - 0.35f) * sz * 0.5f},
                        (Vector2){tipX, tipY},
                        Fade((Color){255, 100, 0, 255}, 0.55f)
                    );
                    
                    // 内焰白亮
                    DrawLineEx(p.position, (Vector2){ix, iy}, 2.0f + flicker, Fade(WHITE, 0.6f));
                }
                
                // 内核——炽白球体
                DrawCircleV(p.position, sz * 0.55f, Fade(WHITE, 0.95f));
                DrawCircleV(p.position, sz * 0.75f, Fade((Color){255, 255, 180, 255}, 0.7f));
                DrawCircleV(p.position, sz * 0.35f, WHITE);
                
                // 火星飞溅
                for (int s = 0; s < 5; s++) {
                    float angle = time * 8.0f + s * 72.0f * DEG2RAD;
                    float dist = sz * 1.4f + sin(time * 12.0f + s) * 5;
                    Vector2 spark = { x + cos(angle) * dist, y + sin(angle) * dist };
                    DrawCircleV(spark, 1.5f, Fade((Color){255, 255, 150, 255}, 0.9f));
                    DrawCircleV(spark, 3.0f, Fade((Color){255, 180, 30, 255}, 0.3f));
                }
            }
            else if (p.type == 5) {
                // 无敌星星...
                float time = GetTime();
                DrawCircleV(p.position, 15, Fade(YELLOW, 0.5f));
                DrawCircleV(p.position, 10, Fade(WHITE, 0.8f));
                DrawText("★", p.position.x - 10, p.position.y - 10, 22, YELLOW);
            } else {
                DrawTextureEx(powerUpTexture, p.position, 0, 0.01f, WHITE);
            }
        }
    }
}

void Game::DrawHearts() {
    int startX = screenWidth - 70;
    int startY = 20;
    int spacing = 50;
    
    for (int i = 0; i < lives; i++) {
        int x = startX - i * spacing;
        
        int heartPixels[24][24] = {
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0},
            {0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0},
            {0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0},
            {0,0,0,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,0,0,0},
            {0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0},
            {0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0},
            {0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
            {0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0},
            {0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
        };
        
        for (int row = 0; row < 24; row++) {
            for (int col = 0; col < 24; col++) {
                if (heartPixels[row][col] == 1) {
                    DrawPixel(x + col - 12, startY + row - 12, RED);
                }
            }
        }
    }
}
void Game::UpdateNetwork() {
    if (!isNetworkGame) return;
    network.Update();
    
    if (networkMode == NetworkMode::HOST) {
        // 主机发送游戏状态
        SendGameState();
        // 主机接收客户端板位置
        ::GameState clientState;
        memset(&clientState, 0, sizeof(clientState));
        if (network.ReceiveGameState(clientState)) {
            clientPaddleX = clientState.paddle2X;
        }
    } else if (networkMode == NetworkMode::CLIENT) {
        // 客户端发送自己的板位置
        ::GameState myState;
        memset(&myState, 0, sizeof(myState));
        myState.paddle2X = paddle.GetPosition().x;
        network.SendGameState(myState);
        
                // 客户端接收主机发来的游戏状态
        ::GameState hostState;
        memset(&hostState, 0, sizeof(hostState));
        if (network.ReceiveGameState(hostState)) {
            // 同步多球
            int ballCount = hostState.ballCount;
            if (ballCount > 0) {
                balls.clear();
                for (int i = 0; i < ballCount && i < 12; i++) {
                    Ball b;
                    b.SetPosition({hostState.ballX2[i], hostState.ballY2[i]});
                    b.SetSpeed({hostState.ballSpeedX2[i], hostState.ballSpeedY2[i]});
                    b.Start(hostState.ballSpeedX2[i], hostState.ballSpeedY2[i]);
                    if (hostState.isInvincible) {
                        b.SetInvincible(true, hostState.invincibleTimer);
                    }
                    balls.push_back(b);
                }
            }
            
            hostPaddleX = hostState.paddle1X;
            paddle.SetWidth(hostState.paddle1Width);
            score = hostState.score1;
            lives = hostState.lives1;
            
            // 同步道具计时器
            for (int i = 0; i < 3 && i < 5; i++) {
                powerUpTimer[i] = hostState.powerUpTimers[i];
            }
            
            // 同步砖块
            int count = bricks.GetBrickCount();
            for (int i = 0; i < count && i < 50; i++) {
                bricks.SetBrickType(i, hostState.brickType[i]);
                bricks.SetBrickColor(i, hostState.brickColor[i]);
            }
        }
    }
}

void Game::SendGameState() {
    if (!network.Connected()) return;
    
    ::GameState state;
    memset(&state, 0, sizeof(state));
    
    // 球的位置
    state.ballCount = (int)balls.size();
    for (int i = 0; i < state.ballCount && i < 12; i++) {
        state.ballX2[i] = balls[i].GetPosition().x;
        state.ballY2[i] = balls[i].GetPosition().y;
        state.ballSpeedX2[i] = balls[i].GetSpeed().x;
        state.ballSpeedY2[i] = balls[i].GetSpeed().y;
    }
    // 兼容单球
    if (!balls.empty()) {
        state.ballX = balls[0].GetPosition().x;
        state.ballY = balls[0].GetPosition().y;
        state.ballSpeedX = balls[0].GetSpeed().x;
        state.ballSpeedY = balls[0].GetSpeed().y;
        state.isInvincible = balls[0].IsInvincible();
    }
    
    state.paddle1X = paddle.GetPosition().x;
    state.paddle2X = clientPaddleX;
    state.paddle1Width = paddle.GetSize().x;
    state.paddle2Width = 0;
    state.score1 = score;
    state.lives1 = lives;
    
    // 砖块状态
    int count = bricks.GetBrickCount();
    for (int i = 0; i < count && i < 50; i++) {
        if (!bricks.IsBrickActive(i)) {
            state.brickType[i] = 0;
        } else if (bricks.IsBrickExplosive(i)) {
            state.brickType[i] = 2;
        } else if (bricks.IsBrickEvil(i)) {
            state.brickType[i] = 3;
        } else {
            state.brickType[i] = 1;
        }
        state.brickColor[i] = bricks.GetBrickColor(i);
    }
    
    // 道具计时器
    for (int i = 0; i < 3 && i < 5; i++) {
        state.activePowerUps[i] = (powerUpTimer[i] > 0) ? i : -1;
        state.powerUpTimers[i] = powerUpTimer[i];
    }
    
    network.SendGameState(state);
}
/*void Game::NextLevel() {
    currentLevel++;
    if (currentLevel >= 3) currentLevel = 0;
    
    int startX = (screenWidth - 12 * 43) / 2;
    int startY = 100;
    bricks.LoadPattern(g_levelPatterns[currentLevel][currentDifficulty], startX, startY);
    
    state = GameState::WAITING;
    balls.clear();
    balls.push_back(Ball());
    balls[0].Reset();
    paddle.Reset();
    paddle.SetWidth(originalPaddleWidth);
}直接加载改成异步加载*/
void Game::NextLevel() {
    int nextLevel = currentLevel + 1;
    if (nextLevel >= 3) nextLevel = 0;
    
    StartAsyncLoad(nextLevel, currentDifficulty);
}
void Game::StartAsyncLoad(int level, int difficulty) {
    if (isLoading.load()) return;
    isLoading.store(true);
        // 清空上一关特效
    for (int i = 0; i < MAX_PARTICLES; i++) particleActive[i] = false;
    floatingTexts.clear();
    powerUps.clear();
    for (int i = 0; i < 6; i++) {
        powerUpTimer[i] = 0.0f;
    }
    pendingLevel = level;
    pendingDifficulty = difficulty;
    loadedCount = 0;
    
    // 异步加载
    /*loadFuture = std::async(std::launch::async, [this, level, difficulty]() {
        // 模拟耗时加载
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    });*/
    // 同时启动 3 个异步任务
    /*for (int i = 0; i < totalTasks; i++) {
        std::thread([this](int taskId) {
            std::this_thread::sleep_for(std::chrono::milliseconds(400));
            /*std::this_thread::sleep_for(std::chrono::milliseconds(50 + taskId * 15));*/
           /*std::string msg = "资源" + std::to_string(taskId + 1) + " 加载完成";
            loadQueue.push(msg);
        }, i).detach();
    }*/
    // 一个后台线程，逐步完成 20 个任务，测试用
    std::thread([this]() {
        for (int i = 0; i < totalTasks; i++) {
            std::this_thread::sleep_for(std::chrono::milliseconds(400));
            loadQueue.push("task");
        }
    }).detach();
}

/*void Game::CheckAsyncLoad() {
    if (!isLoading) return;
    
    // 检查是否加载完成
    auto status = loadFuture.wait_for(std::chrono::seconds(0));
    if (status == std::future_status::ready) {
        // 应用新关卡
        currentLevel = pendingLevel;
        currentDifficulty = pendingDifficulty;
        
        int startX = (screenWidth - 12 * 43) / 2;
        bricks.LoadPattern(g_levelPatterns[currentLevel][currentDifficulty], startX, 100);
        
        balls.clear();
        balls.push_back(Ball());
        balls[0].Reset();
        paddle.Reset();
        originalPaddleWidth = config.paddleWidth;
        paddle.SetWidth(originalPaddleWidth);
        state = GameState::WAITING;
        isLoading.store(false);
    }
}*/
void Game::CheckAsyncLoad() {
    if (!isLoading.load()) return;
    
    std::string msg;
    while (loadQueue.pop(msg)) {
        loadedCount++;
    }
    
    if (loadedCount >= totalTasks) {
        currentLevel = pendingLevel;
        currentDifficulty = pendingDifficulty;
        
        int startX = (screenWidth - 12 * 43) / 2;
        bricks.LoadPattern(g_levelPatterns[currentLevel][currentDifficulty], startX, 100);
        
        balls.clear();
        balls.push_back(Ball());
        balls[0].Reset();
        paddle.Reset();
        originalPaddleWidth = config.paddleWidth;
        paddle.SetWidth(originalPaddleWidth);
        state = GameState::WAITING;
        isLoading.store(false);
        loadedCount = 0;
    }
}
void Game::SpawnParticle(Vector2 pos, Vector2 vel, float life, float size, Color color) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (!particleActive[i]) {
            particlePool[i].pos = pos;
            particlePool[i].vel = vel;
            particlePool[i].life = life;
            particlePool[i].size = size;
            particlePool[i].color = color;
            particleActive[i] = true;
            return;
        }
    }
}//创建一个新粒子
//存档
void Game::SaveProgress() {
    TraceLog(LOG_INFO, "开始保存...");
    json save;
    save["v"] = 1;
    save["level"] = currentLevel;
    save["diff"] = currentDifficulty;
    save["score"] = score;
    save["lives"] = lives;
    save["paddle_x"] = paddle.GetPosition().x;
    save["paddle_w"] = paddle.GetSize().x;
    
    json blist = json::array();
    int n = bricks.GetBrickCount();
    for (int i = 0; i < n; i++) {
        int t = 1;
        if (!bricks.IsBrickActive(i)) t = 0;
        else if (bricks.IsBrickExplosive(i)) t = 2;
        else if (bricks.IsBrickEvil(i)) t = 3;
        else if (bricks.IsBrickIndestructible(i)) t = 4;
        blist.push_back(t);
    }
    save["bricks"] = blist;
    
    std::ofstream file("save.json");
    file << save.dump(4);
    TraceLog(LOG_INFO, "存档已保存");
}
bool Game::LoadProgress() {
    std::ifstream file("save.json");
    if (!file.is_open()) return false;
    
    json save;
    file >> save;
    
    int version = save.value("v", 0);
    
    currentLevel = save["level"];
    currentDifficulty = save["diff"];
    score = save["score"];
    lives = save["lives"];
    
    if (save.contains("paddle_x")) {
        paddle.SetPosition({save["paddle_x"], paddle.GetPosition().y});
        paddle.SetWidth(save["paddle_w"]);
    }
    
    return true;
}
void Game::ToggleEditMode() {
    editMode = !editMode;
    if (editMode) {
        state = GameState::PLAYING;
        balls.clear();
    }
}

void Game::EditClick() {
    TraceLog(LOG_INFO, "EditClick called");
    Vector2 mouse = GetMousePosition();
    bricks.ToggleBrickAt(mouse.x, mouse.y, 22.0f, 14.0f, 2.0f, 100.0f, currentBrickType);
}
void Game::SaveEditToJSON() {
    bricks.SaveLayoutToJSON("levels/custom.json", 22.0f, 14.0f, 2.0f, 100.0f);
}