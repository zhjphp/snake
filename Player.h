#include <string>
#include <list>

struct Player {
    // 用户名
    std::string name;
    // 得分
    int score;
    // 输入指令集
    std::list<int> instruction;
};