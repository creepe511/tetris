/* linkedlist.h - 头文件 */

#ifndef GAME_RECORD_H
#define GAME_RECORD_H

#include <time.h> // 用于存储时间戳

#define NOTE_MAX_LENGTH 100 // 备注的最大长度

// 游戏记录的节点结构
typedef struct Node {
    time_t start_time;              // 游戏开始时间
    int score;                      // 游戏分数
    int duration;                   // 游戏时长（秒）
    char note[NOTE_MAX_LENGTH];     // 游戏备注
    struct Node* next;              // 指向下一个节点的指针
} Node;

// 链表操作函数声明
Node* create_record(time_t start_time, int score, int duration, const char* note);
void add_record(Node** head, Node* new_record);
int delete_record(Node** head, time_t start_time);
Node* find_record(Node* head, time_t start_time);
void update_record(Node* record, int score, int duration, const char* note);
void free_all_records(Node** head);

#endif // GAME_RECORD_H
