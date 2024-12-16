// linkedlist.h - 头文件

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdio.h>

// 定义游戏记录节点结构体
typedef struct Node {
    char start_time[30];              // 游戏开始时间
    int score;                       // 游戏分数
    char duration[15];               // 游戏时长（秒）
    char note[15];                   // 游戏备注
    struct Node* next;               // 指向下一个节点的指针
} Node;

// 创建一个新的游戏记录节点
Node* create_record(const char* start_time, int score, const char* duration, const char* note);

// 添加一个新记录到链表中
void add_record(Node** head, Node* new_record);

// 删除指定开始时间的记录
int delete_record(Node** head, const char* start_time);

// 查找指定开始时间的记录
Node* find_record(Node* head, const char* start_time);

// 修改备注内容
void edit_note(Node* record, const char* new_note);

// 释放链表中所有的记录
void free_all_records(Node** head);

#endif // LINKEDLIST_H