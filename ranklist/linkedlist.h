// linkedlist.h - 头文件

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdio.h>

// 定义游戏记录节点结构体
typedef struct Node {
    char start_time[64];              // 游戏开始时间
    int score;                       // 游戏分数
    int duration;               // 游戏时长（秒）
    char note[128];                   // 游戏备注
    struct Node* next;               // 指向下一个节点的指针
} Node;

// 创建一个新的游戏记录节点(返回应该新节点new_node)
Node* create_node(const char* start_time, int score, int duration, const char* note);

// 添加一个新记录到链表中
void add_node(Node** head, Node* new_node);

// 删除指定开始时间的记录
int delete_node(Node** head, const char* start_time);

//按游戏时间查找节点
Node* find_node(Node* head, const char* start_time);

// 修改备注内容
void edit_note(Node* head, const char* start_time, const char* new_note);

// 释放链表中所有的记录
void free_all_nodes(Node** head);

#endif // LINKEDLIST_H