#include <iostream>
using namespace std;

class ScoreCalculator {
public:
    // 重载1：根据砖块类型返回得分
    int CalculateScore(int brickType) {
        if (brickType == 1) return 10;      // 普通砖块
        if (brickType == 2) return 20;      // 金色砖块
        if (brickType == 3) return -5;      // 炸弹砖块
        return 0;
    }
    
    // 重载2：根据砖块类型和连击数返回得分
    int CalculateScore(int brickType, int combo) {
        int baseScore = CalculateScore(brickType);
        return baseScore + combo * 2;
    }
};

int main() {
    ScoreCalculator calc;
    
    cout << "普通砖块得分: " << calc.CalculateScore(1) << endl;
    cout << "金色砖块得分: " << calc.CalculateScore(2) << endl;
    cout << "炸弹砖块得分: " << calc.CalculateScore(3) << endl;
    cout << "金色砖块 + 3连击得分: " << calc.CalculateScore(2, 3) << endl;
    
    return 0;
}