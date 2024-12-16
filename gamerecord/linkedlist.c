/* linkedlist.h - 头文件 */

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <time.h>

// 游戏记录节点结构体
typedef struct Node {
    time_t start_time;  // 游戏开始时间
    int score;          // 游戏得分
    time_t duration;    // 游戏时长
    char* note;         // 游戏备注
    struct Node* next;  // 指向下一个节点
} Node;

// 创建一个新的游戏记录节点
Node* create_record(time_t start_time, int score, time_t duration, const char* note);

// 添加一个新记录到链表中
void add_record(Node** head, Node* new_record);

// 删除指定开始时间的记录
int delete_record(Node** head, time_t start_time);

// 查找指定开始时间的记录
Node* find_record(Node* head, time_t start_time);

// 更新一个记录的内容
void update_record(Node* record, int score, time_t duration, const char* note);

// 更新备注内容
void update_note(Node* record, const char* note);

// 释放链表中所有的记录
void free_all_records(Node** head);

#endif // LINKEDLIST_H
