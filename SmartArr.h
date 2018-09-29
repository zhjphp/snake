#ifndef SMARTARR_H
#define SMARTARR_H

#include "SmartArrNode.h"

// 引用计数，智能数组
template <class T>
class SmartArr {
public:
    SmartArrNode<T>* head;
    SmartArrNode<T>* tail;
    size_t* ref_count;
    size_t* ele_num;

    SmartArr();
    SmartArr(const SmartArr<T>&);

    void Push(T);
    void Pop();
    void Unshift(T);
    void Shift();
    SmartArrNode<T>* operator[](unsigned int);
    SmartArr<T>& operator=(const SmartArr<T>&);
    SmartArrNode<T>* Find(unsigned int);
    void Insert(size_t k, T ele);

    void ClearNode();
    ~SmartArr();
};

/**
 * SmartArr.cpp 内容    
*/

// #include "SmartArr.h"
#include "SmartArrNode.h"

template <class T>
SmartArr<T>::SmartArr():head(0), tail(0), ref_count(new size_t), ele_num(new size_t) {
    *ref_count = 1;
    *ele_num = 0;
}

// 浅复制
template <typename T>
SmartArr<T>::SmartArr(const SmartArr<T>& c) {
    head = c.head;
    tail = c.tail;
    ele_num = c.ele_num;
    // 引用计数增加
    (*c.ref_count)++;
    ref_count = c.ref_count;
}

// 末尾加入元素
template <class T>
void SmartArr<T>::Push(T ele) {
    // 实例化一个节点
    SmartArrNode<T>* n = new SmartArrNode<T>();
    // 节点数据写入
    n->val = ele;
    n->pre = tail;
    n->next = 0;
    // 更改旧尾节点的next指针为新尾节点
    if (*ele_num > 0)
        tail->next = n;
    // 更新尾节点指针
    tail = n;
    // 处理头尾节点，如果只有一个节点，则头尾都指向这个节点
    if (*ele_num == 0)
        head = n;
    // 元素数量 +1
    (*ele_num)++;
}

// 删除末尾元素
template <class T>
void SmartArr<T>::Pop() {
    SmartArrNode<T>* n = tail;
    // 更新尾节点
    tail = n->pre;
    tail->next = 0;
    // 删除旧尾节点
    delete n;
    // 元素数量 -1
    (*ele_num)--;
}

// 头部加入元素
template <class T>
void SmartArr<T>::Unshift(T ele) {
    // 实例化一个节点
    SmartArrNode<T>* n = new SmartArrNode<T>();
    // 节点数据写入
    n->val = ele;
    n->pre = 0;
    n->next = head;
    // 更改旧头节点的pre指针为新头节点
    if (*ele_num > 0)
        head->pre = n;
    // 更新头节点指针
    head = n;
    // 处理头尾节点，如果只有一个节点，则头尾都指向这个节点
    if (*ele_num == 0)
        tail = n;
    // 元素数量 +1
    (*ele_num)++;
}

// 删除头部元素
template <class T>
void SmartArr<T>::Shift() {
    SmartArrNode<T>* n = head;
    // 更新头结点
    head = n->next;
    head->pre = 0;
    // 删除旧头结点
    delete n;
    // 元素数量 -1
    (*ele_num)--;
}

// 重载[]运算符
template <class T>
SmartArrNode<T>* SmartArr<T>::operator[](unsigned int k) {
    return Find(k);
}

// 浅复制
template <typename T>
SmartArr<T>& SmartArr<T>::operator=(const SmartArr<T>& c) {
    head = c.head;
    tail = c.tail;
    ele_num = c.ele_num;
    // 引用计数增加
    (*c.ref_count)++;
    ref_count = c.ref_count;
    return *this;
}

// 按数组规则下标查找元素
template <class T>
SmartArrNode<T>* SmartArr<T>::Find(unsigned int k) {
    // 如果下标越界
    if (k >= *ele_num)
        return 0;
    // 如果是头结点
    if (k == 0)
        return head;
    // 如果是尾节点
    if (k == (*ele_num - 1))
        return tail;
    // 从头结点开始遍历
    size_t i = 0;
    for (SmartArrNode<T>* n = head; n != 0; n = n->next) {
        if (i == k)
            return n;
        i++;
    }
    return 0;
}

// 插入节点
template <typename T>
void SmartArr<T>::Insert(size_t k, T ele) {
    // 边界检查
    if (k > *ele_num) {

    }
    if (k == *ele_num) {
        // 如果是末尾增加，则执行Push方法
        Push(ele);
    } else if (k == 0) {
        // 如果是头部增加，则执行Unshift方法
        Unshift(ele);
    } else {
        // 中间位置增加节点
        // 找到原节点
        SmartArrNode<T>* src = Find(k);
        // 新建目标节点
        SmartArrNode<T>* dest = new SmartArrNode<T>;
        dest->val = ele;
        dest->pre = src->pre;
        dest->next = src;
        // 改变源节点的前一个节点
        src->pre->next = dest;
        // 改变原节点
        src->pre = dest;
        // 元素数量加一
        (*ele_num)++;
    }
}

// 遍历清除节点内存
template <typename T>
void SmartArr<T>::ClearNode() {
    SmartArrNode<T>* current = head;
    SmartArrNode<T>* next;
    do {
        next = current->next;
        delete current;
        current = next;
    } while (next != 0);
}

template <class T>
SmartArr<T>::~SmartArr() {
    // 删除所有 new 申请的空间
    (*ref_count)--;
    if (*ref_count == 0) {
        if (*ele_num > 0)
            ClearNode();
        delete ref_count;
        delete ele_num;
    }
}

// template class SmartArr<int>;    如果单独放入cpp文件需显示实例化，或将cpp文件引入调用程序

#endif