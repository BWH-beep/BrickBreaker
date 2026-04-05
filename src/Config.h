#ifndef CONFIG_H
#define CONFIG_H

#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

struct Config {
    int screenWidth;
    int screenHeight;
    float ballRadius;
    float ballSpeedX;
    float ballSpeedY;
    float paddleWidth;
    float paddleHeight;
    float paddleSpeed;
    float brickWidth;
    float brickHeight;
    float brickSpacing;
    int brickRows;
    int brickCols;
    int initialLives;
    int steps;
    
    static Config load(const std::string& filename) {
        Config cfg;
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "无法打开配置文件: " << filename << std::endl;
            return cfg;
        }
        
        json j;
        file >> j;
        
        cfg.screenWidth = j["screenWidth"];
        cfg.screenHeight = j["screenHeight"];
        cfg.ballRadius = j["ballRadius"];
        cfg.ballSpeedX = j["ballSpeedX"];
        cfg.ballSpeedY = j["ballSpeedY"];
        cfg.paddleWidth = j["paddleWidth"];
        cfg.paddleHeight = j["paddleHeight"];
        cfg.paddleSpeed = j["paddleSpeed"];
        cfg.brickWidth = j["brickWidth"];
        cfg.brickHeight = j["brickHeight"];
        cfg.brickSpacing = j["brickSpacing"];
        cfg.brickRows = j["brickRows"];
        cfg.brickCols = j["brickCols"];
        cfg.initialLives = j["initialLives"];
        cfg.steps = j["steps"];
        
        return cfg;
    }
};

#endif