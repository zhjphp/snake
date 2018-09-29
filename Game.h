#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>
#include <map>
#include <deque>

#include "Snake.h"

extern std::map<std::string, char> key_config;

class Game {
public:
    // 游戏菜单
    std::vector<std::string> menu1;
    std::vector<std::string> menu2;
    std::vector<std::string> menu3;
    // 菜单集合
    std::vector<std::vector<std::string>> menu;
    // 游戏状态码
    enum GameStatusCode {
        e_menu_1,
        e_menu_2,
        e_menu_3,
        e_menu_help,
        e_play_snake,
    };
    // 当前游戏状态
    enum GameStatusCode current_status = e_menu_1;
    // 被选择的菜单ID
    int selected_menu_id = 0;
    // 等级，难度
    size_t level;
    // Snake
    Snake* snake;
    // 砖(墙体节点)
    struct brick {
        int x;
        int y;
        char shape;
    };
    typedef struct brick Brick;
    // 墙体
    SmartArr<Brick*>* Wall;
    // 食物
    typedef struct brick Food;
    Food* food;
    // 得分
    size_t score;

    Game();
    void Start();
    void GameOver();
    void DrawMenu(int);
    void DrawHelp();
    void MakeWall();
    void DrawWall();
    void InstructionOut();
    static void* ReadInstruction(void*);
    void Draw(int = 0);
    void OperatingMenu(int, int = 0);
    void DrawBackground();
    void Play();
    void CheckCollision();
    void MakeFood();
    void CheckFood();
    size_t Random(size_t, size_t);
    void DrawFood();
    ~Game();
};

#endif