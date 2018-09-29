#include <iostream>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "Snake.h"
#include "SmartArr.h"
#include "SmartArrNode.h"

// 定义窗口大小
extern struct winsize window_size;

Snake::Snake():turnSet(new SmartArr<Turn*>) {
    InitBody();
}

// 初始化 body
void Snake::InitBody() {
    // 创建body
    body = new SmartArr<Part*>;
    // 创建头部
    Part* h = new Part;
    h->x = window_size.ws_col/2;
    h->y = window_size.ws_row/2;
    // h->shape = '<';
    h->shape = 'o';
    // 初始方向默认向左
    h->direction = key_config["left"];
    // 设置头、尾
    head = h;
    tail = h;
    // 加入body
    body->Push(h);
    // 节点数量 +1
    num++;
    // 默认初始body为头节点加5个节点
    for (int i = 0; i < 5; i++) {
        Eat();
    }
}

// 增加节点
void Snake::Eat() {
    // 创建节点
    Part* p = new Part;
    // 填充接待数据
    if (*body->ele_num == 1) {
        // 眼睛 (:
        p->shape = 'o';
    } else {
        p->shape = 'o';
    }
    // 需要根据尾部方向和位置计算增加的节点位置
    if (tail->direction == key_config["left"]) {
        p->x = tail->x + 1;
        p->y = tail->y;
    } else if (tail->direction == key_config["right"]) {
        p->x = tail->x - 1;
        p->y = tail->y;
    } else if (tail->direction == key_config["up"]) {
        p->x = tail->x;
        p->y = tail->y + 1;
    } else if (tail->direction == key_config["down"]) {
        p->x = tail->x;
        p->y = tail->y - 1;
    }
    // switch(tail->direction) {
    //     case 'l':
    //         p->x = tail->x + 1;
    //         p->y = tail->y;
    //         break;
    //     case 'r':
    //         p->x = tail->x - 1;
    //         p->y = tail->y;
    //         break;
    //     case 'u':
    //         p->x = tail->x;
    //         p->y = tail->y + 1;
    //         break;
    //     case 'd':
    //         p->x = tail->x;
    //         p->y = tail->y - 1;
    //         break;
    // }
    // 方向为尾节点方向
    p->direction = tail->direction;
    // 加入body
    body->Push(p);
    // 更新尾节点
    tail = p;
    // 节点数量 +1
    num++;
}

// 添加方向改变
void Snake::AddTurn(char direction) {
    if ( 
        ( (body->head->val->direction + direction) != (key_config["left"] + key_config["right"]) ) 
        &&
        ( (body->head->val->direction + direction) != (key_config["up"] + key_config["down"]) )
     ) {
        Turn* t = new Turn;
        t->direction = direction;
        t->current = body->head;
        turnSet->Push(t);
    }
    
}

// 对每个转向节点执行操作，每个节点每次操作body内的一个节点
void Snake::DoTurn() {
    for (size_t i = 0; i < *turnSet->ele_num; i++) {
        // 将改变的方向赋值给part节点
        (*turnSet)[i]->val->current->val->direction = (*turnSet)[i]->val->direction;
        if ((*turnSet)[i]->val->current->next != 0) {
            // 将current节点移动到下一个part节点
            (*turnSet)[i]->val->current = (*turnSet)[i]->val->current->next;
        } else {
            // 如果处理完了body最后一个节点，转向节点任务完成，可以删除
            //turnSet->Shift();     // 放开此代码后会造成段错误
        }
    }
}

// 移动
void Snake::Move() {
    // 首先进行转向
    DoTurn();
    // 每次移动按照当前body节点的方向进行移动
    for (size_t i = 0; i < *body->ele_num; i++) {
        // 计算每个节点的新位置
        if ((*body)[i]->val->direction == key_config["left"]) {
            (*body)[i]->val->x -= 1;
        } else if ((*body)[i]->val->direction == key_config["right"]) {
            (*body)[i]->val->x += 1;
        } else if ((*body)[i]->val->direction == key_config["up"]) {
            (*body)[i]->val->y -= 1;
        } else if ((*body)[i]->val->direction == key_config["down"]) {
            (*body)[i]->val->y += 1;
        }
        // switch ((*body)[i]->val->direction) {
        //     case 'l':
        //         (*body)[i]->val->x -= 1;
        //         break;
        //     case 'r':
        //         (*body)[i]->val->x += 1;
        //         break;
        //     case 'u':
        //         (*body)[i]->val->y += 1;
        //         break;
        //     case 'd':
        //         (*body)[i]->val->y -= 1;
        //         break;
        // }
    }
}

// 绘制snake
void Snake::Draw() {
    bodyStr = "";
    for (size_t i = 0; i < *body->ele_num; i++) {
        bodyStr += "\033[" + std::to_string((*body)[i]->val->y) + ";" + std::to_string((*body)[i]->val->x) + "H" + (*body)[i]->val->shape;
    }
    // return bodyStr;
}

Snake::~Snake() {

}