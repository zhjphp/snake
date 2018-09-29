#ifndef SMARTARRNODE_H
#define SMARTARRNODE_H


// 智能数组节点（偏特化）
template <class T>
class SmartArrNode {
public:
    SmartArrNode<T>* pre;
    SmartArrNode<T>* next;
    T val;
    int key;

    SmartArrNode();
    ~SmartArrNode();
};


template <class T>
class SmartArrNode<T*> {
public:
    SmartArrNode<T*>* pre;
    SmartArrNode<T*>* next;
    T* val;
    int key;

    SmartArrNode();
    ~SmartArrNode();
};

/**
 * SmartArrNode.cpp 文件内容
*/

// #include "SmartArrNode.h"

template <class T>
SmartArrNode<T*>::SmartArrNode() {
    pre = 0;
    next = 0;
    key = 0;
}

template <class T>
SmartArrNode<T>::SmartArrNode() {
    pre = 0;
    next = 0;
    key = 0;
}

template <class T>
SmartArrNode<T*>::~SmartArrNode() {
    delete val;
}

template <class T>
SmartArrNode<T>::~SmartArrNode() {
    
}

// template class SmartArrNode<int>;    如果单独放入cpp文件需显示实例化，或将cpp文件引入调用程序

#endif