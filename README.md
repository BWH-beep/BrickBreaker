# Brick Breaker - 打砖块游戏

## 简介
基于 Raylib 开发的打砖块游戏，支持道具系统、多关卡、双人联机、存档读档、关卡编辑器。

## 环境要求
- g++ 支持 C++14
- Raylib >= 4.0
- nlohmann/json
- ENet（网络模块）
- Linux/WSL 环境

## 编译运行
```bash
cd BrickBreaker
g++ -o brick_game src/main.cpp src/ball.cpp src/paddle.cpp src/brick.cpp src/game.cpp src/network_manager.cpp -I. -lraylib -lX11 -lGL -lm -lpthread -ldl -lrt -lenet -std=c++14 -w -lpthread
./brick_game
操作说明
按键	功能
空格	发球 / 确认
左右方向键	移动板子
鼠标	暂停按钮 / 编辑模式放置砖块
E	进入/退出编辑模式
S	编辑模式保存关卡
N	过关后进入下一关
Q	返回主菜单
R	死亡后重试本关
L	切换中英文
鼠标右键	编辑模式切换砖块类型
功能列表
基础打砖块玩法
道具系统（加长板子、分裂球、无敌球、冰冻效果、加减分）
三种关卡图案（爱心、猫咪、蝴蝶），每种三个难度
双人联机（主机+客户端模式）
存档/读档（保存分数、生命、砖块状态）
自定义关卡编辑器
果冻风格渲染
多线程异步加载 + Loading 动画
粒子对象池性能优化
空间划分网格碰撞优化
中英文切换
