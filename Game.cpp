#include <iostream>
#include <stdio.h>
#include <termios.h>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <list>
#include <mutex>
#include <cstdlib>
#include <ctime>

#include "Player.h"
#include "Game.h"
#include "Snake.h"

// 清除视图缓冲区
extern void ClearViewBuf();
// 清除旧视图
extern void ClearView();

// 定义窗口大小
extern struct winsize window_size;
// 视图数据缓冲区
extern std::string* view_buf;
extern std::recursive_mutex mtx;
// 定义玩家
extern struct Player player;

Game::Game() {
    // 初始化菜单1
    menu1.push_back("START");
    // menu1.push_back("HELP");
    menu1.push_back("EXIT");
    // 初始化菜单2
    // menu2.push_back("LEVEL-1");
    // menu2.push_back("LEVEL-2");
    // menu2.push_back("LEVEL-3");
    // 初始化菜单3
    menu3.push_back("EXIT");
    // menu3.push_back("RESTART");
    // menu3.push_back("MAIN");
    // 初始化集合菜单数组
    menu.push_back(menu1);
    menu.push_back(menu2);
    menu.push_back(menu3);
    // 初始等级默认为1
    level = 400000;
    // 生成围墙
    MakeWall();
    // 初始化snake，为了好管理内存，构造中初始化
    snake = new Snake;
    // 初始化food，为了好管理内存，构造中初始化
    food = new Food;
    food->shape = 0;
    // 初始化得分
    score = 0;
}

// 游戏启动
void Game::Start() {
    Draw();
}

// 开启指令读取线程
void Game::InstructionOut() {
    pthread_t instruction_thread_id;
    if (pthread_create(&instruction_thread_id, NULL, ReadInstruction, (void*)this) != 0) {
        std::cout << "指令读取线程启动失败！" << std::endl;
    }
}

// 解析用户指令
void* Game::ReadInstruction(void* ptr) {
    // 按进入队列顺序取出输入内容
    while(1) {
        int input = 0;
        // 判断list是否为空
        if (!player.instruction.empty()) {
            // 读取list第一个指令
            input = player.instruction.front();
            mtx.lock();
            // 删除刚刚读取的指令
            player.instruction.pop_front();
            mtx.unlock();
        }
        Game* g = (Game*)ptr;
        if (g->current_status == g->e_play_snake) {
            // 如果在游戏中，就算没有输入，也持续执行指令
            g->Draw(input);
        } else if (input != 0) {
            //*view_buf += "\033[" + std::to_string(5) + ";" + std::to_string(5) + "H" + std::to_string(input);     // 测试用
            // 解析输入指令，如果不在游戏中，且没有输入的情况下不执行指令
            g->Draw(input);
        }
    }
    return NULL;
}

// 解析输入指令
void Game::Draw(int input) {
    //*view_buf += "\033[" + std::to_string(5) + ";" + std::to_string(5) + "H" + std::to_string(input);     // 测试用
    switch(current_status) {
        case e_menu_1:
            ClearViewBuf();
            DrawWall();
            OperatingMenu((int)e_menu_1, input);
            DrawMenu((int)e_menu_1);
            break;
        case e_menu_2:
            break;
        case e_menu_3:
            ClearViewBuf();
            DrawWall();
            OperatingMenu((int)e_menu_3, input);
            DrawMenu((int)e_menu_3);
            break;
        case e_play_snake:
            if ( (input == key_config["left"]) || (input == key_config["right"]) || (input == key_config["up"]) || (input == key_config["down"]) )
                snake->AddTurn(input);
            Play();
            break;
        default:
            break;
    }
}

// 选择菜单动作
void Game::OperatingMenu(int menu_id, int input) {
    if (input == key_config["up"]) {
        // 向上滑动菜单
        if ( (selected_menu_id - 1) >= 0 ) {
            selected_menu_id -= 1;
        }
    } else if (input == key_config["down"]) {
        // 向下滑动菜单
        if ((selected_menu_id + 1) < menu[menu_id].size()) {
            selected_menu_id += 1;
        }
    } else if (input == key_config["action"]) {
        // 执行菜单
        switch (menu_id) {
            case 0:
                switch (selected_menu_id) {
                    case 0:
                        // 切换游戏状态，开始游戏
                        current_status = e_play_snake;
                        break;
                    case 1:

                        break;
                    case 2:
                        exit(0);
                        break;
                    default:
                        break;
                }
                break;
            case 1:
                break;
            case 2:
                switch (selected_menu_id) {
                    case 0:
                        // EXIT
                        exit(0);
                        break;
                    case 1:
                        // RESTART
                        //current_status = e_play_snake;
                        break;
                    case 2:
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }
}

// 绘制菜单
void Game::DrawMenu(int menu_id) {
    // 选中效果
    std::string menu_selected_left = ">>>>     ";
    std::string menu_selected_right = "     <<<<";
    // 计算行分割点
    int row_split_point = window_size.ws_row / (menu[menu_id].size() + 1);
    std::string menu_content;
    // 循环输出菜单
    for (int i = 0; i < menu[menu_id].size(); i++) {
        // 复制菜单内容，防止被篡改
        menu_content = menu[menu_id][i];
        // 如果菜单被选中则绘制选中效果
        if (i == selected_menu_id)
            menu_content = menu_selected_left + menu_content + menu_selected_right;
        // 计算列分割点，+1为了视觉居中美观
        int col_split_point = window_size.ws_col / 2 - menu_content.length() / 2 + 1;
        // 指定点输出菜单内容
        *view_buf = *view_buf + "\033[" + std::to_string((i + 1) * row_split_point) + ";" + std::to_string(col_split_point) + "H" + menu_content;
    }
}

// 绘制帮助
// void Game::DrawHelp() {
//     std::string h1 = "小写字母 w : 上";
//     std::string h2 = "小写字母 d : 下";
//     std::string h3 = "小写字母 a : 左";
//     std::string h4 = "小写字母 d : 右";
// }

// 绘制围墙
void Game::DrawWall() {
    for (int i = 0; i < *Wall->ele_num; i++) {
        *view_buf += "\033[" + std::to_string((*Wall)[i]->val->y) + ";" + std::to_string((*Wall)[i]->val->x) + "H" + (*Wall)[i]->val->shape;
    }
}

// 生成围墙
void Game::MakeWall() {
    Wall = new SmartArr<Brick*>;
    Brick* brick;
    // j为y轴，i为x轴
    for (int j = 0; j <= window_size.ws_row; j++) {
        if ( (j == 0) || (j == window_size.ws_row) ) {
            for (int i = 0; i <= window_size.ws_col; i++) {
                brick = new Brick;
                brick->x = i;
                brick->y = j;
                brick->shape = '#';
                Wall->Push(brick);
                // *view_buf = *view_buf + "\033[" + std::to_string(j) + ";" + std::to_string(i) + "H" + "#";   // 旧版直接绘制时使用
            }
        } else {
            // *view_buf = *view_buf + "\033[" + std::to_string(j) + ";" + std::to_string(0) + "H" + "#";   // 旧版直接绘制时使用
            brick = new Brick;
            brick->x = 0;
            brick->y = j;
            brick->shape = '#';
            Wall->Push(brick);
            
            // *view_buf = *view_buf + "\033[" + std::to_string(j) + ";" + std::to_string( window_size.ws_col ) + "H" + "#";    // 旧版直接绘制时使用
            brick = new Brick;
            brick->x = window_size.ws_col;
            brick->y = j;
            brick->shape = '#';
            Wall->Push(brick);
        }
    }
}

// 绘制背景，用于图像叠加【暂时不用】
void Game::DrawBackground() {
    for (int j = 0; j <= window_size.ws_row; j++) {
        for (int i = 0; i <= window_size.ws_col; i++) {
            *view_buf = *view_buf + "\033[" + std::to_string(j) + ";" + std::to_string(i) + "H" + " ";
        }
    }
}

// 开始游戏
void Game::Play() {
    if (current_status == e_play_snake) {
        // 清空视图缓冲
        ClearViewBuf();
        // 绘制墙体
        DrawWall();
        // 绘制食物
        DrawFood();
        // 移动
        snake->Move();
        // 检查碰撞
        CheckCollision();
        // 绘制snake
        snake->Draw();
        *view_buf += snake->bodyStr;
        // 游戏执行帧数
        usleep(level);
    }
}

// 检查碰撞
void Game::CheckCollision() {
    // snake头与身体碰撞，碰撞后游戏状态设置为失败
    for (int i = 1; i < *snake->body->ele_num; i++) {
        // 当头节点与身体节点重合时为碰撞，游戏结束
        if ( ((*snake->body)[i]->val->x == snake->body->head->val->x) && ((*snake->body)[i]->val->y == snake->body->head->val->y) ) {
            // 游戏结束
            GameOver();
        }
    }
    // snake与墙体碰撞，碰撞后游戏状态设置为失败
    for (int i = 0; i < *Wall->ele_num; i++) {
        if ( ((*Wall)[i]->val->x == snake->body->head->val->x) && ((*Wall)[i]->val->y == snake->body->head->val->y) ) {
            // 游戏结束
            GameOver();
        }
    }
    // snake头与食物碰撞，碰撞后食物消失，触发eat()方法，吃掉食物后，更新食物坐标点
    if ( (food->x == snake->body->head->val->x) && (food->y == snake->body->head->val->y) ) {
        // 吃掉食物
        snake->Eat();
        // 设置图形为0，用来重新生成食物
        food->shape = 0;
        // 得分
        score++;
        // 判断得分，如果得分到一定程度则，提升游戏速度
        switch (score) {
            case 3:
                level = level / 2;
                break;
            case 8:
                level = level / 2;
                break;
            case 12:
                level = level / 2;
                break;
            case 15:
                level = level / 2;
                break;
        }
    }
}

// 生成食物
void Game::MakeFood() {
    food->shape = '@';
    // 随即x坐标
    srand(unsigned(time(0)));
    size_t y_start = 2;
    size_t y_end = window_size.ws_row - 2;
    food->y = Random(y_start, y_end);
    // 随即y坐标
    srand(unsigned(time(0)));
    size_t x_start = 2;
    size_t x_end = window_size.ws_col - 2;
    food->x = Random(x_start, x_end);
}

// 食物坐标不能与snake的body节点重合
void Game::CheckFood() {
    for (int i = 0; i < *snake->body->ele_num; i++) {
        if ( ((*snake->body)[i]->val->x == food->x) && ((*snake->body)[i]->val->y == food->y) ) {
            MakeFood();
        }
    }
}

// 绘制食物
void Game::DrawFood() {
    if (food->shape == 0) {
        MakeFood();
        CheckFood();
    }
    *view_buf = *view_buf + "\033[" + std::to_string(food->y) + ";" + std::to_string(food->x) + "H" + food->shape;
}

// 生成区间随机数
size_t Game::Random(size_t start, size_t end) {
    return start+(end-start)*rand()/(RAND_MAX + 1.0);
}


// 游戏结束
void Game::GameOver() {
    // 切换游戏状态，开始游戏
    current_status = e_menu_3;
    // 清空视图缓冲
    ClearViewBuf();
    // 计算列分割点，+1为了视觉居中美观
    std::string contnet = "GameOver---Prss Any Key To Menu";
    int col_split_point = window_size.ws_col / 2 - contnet.length() / 2 + 1;
    *view_buf = "\033[" + std::to_string(window_size.ws_row / 2) + ";" + std::to_string(col_split_point) + "H" + "GameOver---Prss Any Key To Menu";
}

Game::~Game() {
    delete Wall;
    delete snake;
    delete food;
}