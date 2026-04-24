#include "network_manager.h"
#include <cstdio>
#include <cstring>

#define PIPE_H2C "/tmp/brick_h2c"
#define PIPE_C2H "/tmp/brick_c2h"

NetworkManager::NetworkManager() {
    host = nullptr; peer = nullptr;
    mode = NetworkMode::NONE;
    isConnected = false; sendTimer = 0.0f;
}
NetworkManager::~NetworkManager() { Disconnect(); }

bool NetworkManager::InitAsHost(int port) {
    FILE* f = fopen(PIPE_H2C, "wb"); if (f) fclose(f);
    f = fopen(PIPE_C2H, "wb"); if (f) fclose(f);
    mode = NetworkMode::HOST; isConnected = true;
    TraceLog(LOG_INFO, "Pipe: 主机启动"); return true;
}
bool NetworkManager::InitAsClient(const char* ip, int port) {
    mode = NetworkMode::CLIENT; isConnected = true;
    TraceLog(LOG_INFO, "Pipe: 客户端启动，已连接"); return true;
}
void NetworkManager::Update() {}

void NetworkManager::SendGameState(const GameState& state) {
    if (!isConnected) return;
    const char* p = (mode == NetworkMode::HOST) ? PIPE_H2C : PIPE_C2H;
    FILE* f = fopen(p, "wb");
    if (f) {
        fwrite(&state, sizeof(GameState), 1, f);
        fflush(f);  // 强制写入
        fclose(f);
    }
}
bool NetworkManager::ReceiveGameState(GameState& state) {
    if (!isConnected) return false;
    const char* p = (mode == NetworkMode::HOST) ? PIPE_C2H : PIPE_H2C;
    FILE* f = fopen(p, "rb");
    if (f) {
        fseek(f, 0, SEEK_END);
        long size = ftell(f);
        fseek(f, 0, SEEK_SET);
        if (size == sizeof(GameState)) {
            size_t n = fread(&state, sizeof(GameState), 1, f);
            fclose(f);
            return n == 1;
        }
        fclose(f);
    }
    return false;
}
void NetworkManager::Disconnect() {
    isConnected = false; mode = NetworkMode::NONE;
    remove(PIPE_H2C); remove(PIPE_C2H);
}