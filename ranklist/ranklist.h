

#ifndef RANKLIST_H
#define RANKLIST_H

#include <stdio.h>

//排行榜ranklist.txt
// 定义游戏记录节点结构体
typedef struct Node {
    char user_name[64];
    char start_time[64];              // 游戏开始时间
    int score;                       // 游戏分数
    int duration;               // 游戏时长（秒）
    struct Node* next;               // 指向下一个节点的指针
} Node;

// 创建一个新的游戏记录节点(返回一个新节点new_node)
Node* create_node(const char* start_time, int score, int duration, const char* user_name);

// 添加一个新记录到链表中（传入头节点head的地址&head）
void add_node(Node** head, Node* new_node);

//更新链表（排序并删除第11个节点）
Node* update_list(Node** head);

// 释放链表内存的函数
void free_list(Node* head);

// 链表写入到文件的函数
void write_list_to_file(Node* head, const char* filename);

// 从文件读取链表的函数
Node* read_list_from_file(const char* filename);

// 删除链表第11个节点（如果节点总数大于或等于11）
Node* delete_node(Node* head);

//修改用户名
void modifyUserName(Node* head, const char* newName);
#endif // RANKLIST_H