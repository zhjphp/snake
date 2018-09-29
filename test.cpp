#include <iostream>
#include <stdio.h>
#include <termios.h>
#include <string>
#include <pthread.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <list>
#include <mutex>

#include "SmartArr.h"
#include "SmartArrNode.h"

int main(int argc, char* argv[]) {
    SmartArr<int>* x = new SmartArr<int>();
    int b = 100;
    int d = 200;
    int e = 300;
    int f = 400;
    int g = 500;

    x->Push(b);
    x->Push(d);
    x->Unshift(f);
    x->Push(e);

    x->Pop();
    x->Shift();

    x->Insert(1,g);

    std::cout << (*x)[0]->val << std::endl;
    std::cout << (*x)[1]->val << std::endl;
    std::cout << (*x)[2]->val << std::endl;
    std::cout << *x->ele_num << std::endl;
    std::cout << *x->ref_count << std::endl;

    delete x;

    // SmartArr<char*>* x = new SmartArr<char*>();
    // char* a = new char('a');
    // x->Push(a);

    // std::cout << (*x)[0]->val << std::endl;
    // std::cout << *a << std::endl;

    // delete x;

    
    return 0;
}