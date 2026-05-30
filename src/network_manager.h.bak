#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <enet/enet.h>
#include "network.h"
#include "raylib.h"

// 网络模式
enum class NetworkMode {
    NONE,       // 单机模式
    HOST,       // 主机（服务器）
    CLIENT      // 客户端
};

class NetworkManager {
private:
    ENetHost* host;              // ENet主机
    ENetPeer* peer;              // 连接的对等端
    NetworkMode mode;
    bool isConnected;
    float sendTimer;             // 发送计时器（限制发送频率）
    
public:
    NetworkManager();
    ~NetworkManager();
    
    // 初始化为主机
    bool InitAsHost(int port = 12345);
    
    // 初始化为客户端
    bool InitAsClient(const char* serverIP, int port = 12345);
    
    // 发送游戏状态
    void SendGameState(const GameState& state);
    
    // 接收游戏状态（返回true表示收到新数据）
    bool ReceiveGameState(GameState& state);
    
    // 断开连接
    void Disconnect();
    
    // 获取当前模式
    NetworkMode GetMode() const { return mode; }
    
    // 是否已连接
    bool Connected() const { return isConnected; }
    
    // 每帧更新（处理网络事件）
    void Update();
};

#endif
