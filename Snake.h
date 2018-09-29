#ifndef SNAKE_H
#define SNAKE_H

#include <string>
#include <map>

#include "SmartArr.h"
#include "SmartArrNode.h"

extern std::map<std::string, char> key_config;

class Snake {
public:
    /**
     * 身体节点
     * x：横坐标
     * y：纵坐标
     * shape：字符图形
     * direction：移动方向，配置参考 key_config
    */
    struct part {
        int x;
        int y;
        char shape;
        char direction;
    };
    typedef struct part Part;

    // 转向点
    struct turn {
        SmartArrNode<Part*>* current;
        char direction;
    };
    typedef struct turn Turn;
    SmartArr<Turn*>* turnSet;

    // 身体
    SmartArr<Part*>* body;
    std::string bodyStr;
    // 头
    Part* head;
    // 尾
    Part* tail;
    // 节点数量
    size_t num;

    Snake();
    void InitBody();
    void Eat();
    void Move();
    void Food();
    void AddTurn(char);
    void DoTurn();
    void Draw();
    ~Snake();
};

#endif