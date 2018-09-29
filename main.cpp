#include <iostream>
#include <stdio.h>
#include <termios.h>
#include <string>
#include <pthread.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <list>
#include <mutex>

#include "Player.h"
#include "Game.h"

void* View(void*);
void* InstructionIn(void*);
void ClearView();
void ClearViewBuf();

// 定义窗口大小
struct winsize window_size;
// 视图数据缓冲区
std::string* view_buf = new std::string;
// 游戏刷新率
const float FPS = 16700;
std::recursive_mutex mtx;
// 定义玩家
struct Player player = {"nameless"};
// 按键配置
std::map<std::string, char> key_config = {
    {"up", 'w'},
    {"down", 's'},
    {"left", 'a'},
    {"right", 'd'},
    {"action", '\n'},
    {"pause", ' '}
};

/**
 * 需要开启三个线程
 * 1.接收用户指令线程 
 * 2.游戏逻辑处理线程
 * 3.视图输出线程
*/
int main(int argc, char* argv[]) {
    // 获取窗口大小
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &window_size);

    // 接收用户指令线程
    pthread_t input_thread_id;
    if ( pthread_create(&input_thread_id, NULL, InstructionIn, NULL) )
        std::cout << "用户指令线程创建失败！";

    // 初始化游戏实例
    Game* game = new Game;
    // 启动start菜单
    game->Start();
    // 启动游戏指令读取线程，处理游戏逻辑
    game->InstructionOut();

    // 视图线程
    pthread_t view_thread_id;
    if ( pthread_create(&view_thread_id, NULL, View, NULL) )
        std::cout << "视图线程创建失败！";
    pthread_join(view_thread_id, NULL);

    // 释放资源
    delete game;
    delete view_buf;

    return 0;
}

// 监听指令输入
void* InstructionIn(void* ph) {
    struct termios old_tio, new_tio;
    unsigned char input = 0;
    // get the terminal settings for stdin, 获取stdin的终端设置
    tcgetattr(STDIN_FILENO, &old_tio);
    // we want to keep the old setting to restore them a the end, 我们希望保留旧设置以将其恢复到最后
    new_tio = old_tio;
    // disable canonical mode (buffered i/o) and local echo, 禁用规范模式（缓冲的i / o）和本地回声
    new_tio.c_lflag &=(~ICANON & ~ECHO);
    // set the new settings immediately, 立即设置新设置
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
    while(1) {
        input = 0;
        input = getchar();
        if (input != 0) {
            //*view_buf += "\033[" + std::to_string(5) + ";" + std::to_string(5) + "H" + std::to_string(c);     // 测试用
            mtx.lock();
            // 把输入指令写入输入队列
            player.instruction.push_back(input);
            mtx.unlock();
        }
    }
    // restore the former settings, 恢复以前的设置
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);

    return NULL;
}

// 输出视图
void* View(void* ph) {
    // 隐藏光标
    std::cout << "\033[?25l" << std::flush;
    // 清屏
    std::cout << "\033[2J" << std::flush;
    while(1) {
        // 清除视图和缓冲区【理论上 "\ec" 或 "\x1bc" 也可以】
        // std::cout << "\033c" << std::flush;      // 此方法部分操作系统不支持
        ClearView();
        // 输出视图缓冲区数据
        std::cout << *view_buf << "\033[0;0H" << std::flush;
        // 按照游戏预设FPS刷新输出
        usleep(FPS);
    }

    return NULL;
}

// 逐行清除显示内容 【"\033c"部分系统可替代，某些系统可能无法代替】
void ClearView() {
    for (int i = 0; i <= window_size.ws_row; i++) {
        std::cout << "\033[" + std::to_string( i ) + ";" + std::to_string( 0 ) + "H";
        std::cout << "\033[K" << std::flush;
    }
}

// 清除视图缓冲区
void ClearViewBuf() {
    *view_buf = "";
}