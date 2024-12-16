/* linkedlist.c - 实现文件 */
#include "linkedlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char start_time[30];              // 游戏开始时间
    int score;                      // 游戏分数
    char duration[15];                   // 游戏时长（秒）
    char note[15];     // 游戏备注
    struct Node* next;              // 指向下一个节点的指针
} Node;

// 创建一个新的游戏记录节点
Node* create_record(const char* start_time, int score, const char* duration, const char* note) {
    Node* new_record = (Node*)malloc(sizeof(Node));
    if (!new_record) {
        //perror("Failed to allocate memory for new record");
        return NULL;
    }

    // 使用strncpy复制字符串，确保不会超出数组边界
    strncpy(new_record->start_time, start_time, sizeof(new_record->start_time) - 1);
    new_record->start_time[sizeof(new_record->start_time) - 1] = '\0'; // 确保字符串以null结尾

    new_record->score = score;

    strncpy(new_record->duration, duration, sizeof(new_record->duration) - 1);
    new_record->duration[sizeof(new_record->duration) - 1] = '\0';

    strncpy(new_record->note, note, sizeof(new_record->note) - 1);
    new_record->note[sizeof(new_record->note) - 1] = '\0';

    new_record->next = NULL;
    return new_record;
}

// 添加一个新记录到链表中
void add_record(Node** head, Node* new_record) {
    if (!new_record) return;
    new_record->next = *head;
    *head = new_record;
}

// 删除指定开始时间的记录
int delete_record(Node** head, const char* start_time) {
    Node* current = *head;
    Node* prev = NULL;

    while (current) {
        if (strcmp(current->start_time, start_time) == 0) {
            if (prev) {
                prev->next = current->next;
            } else {
                *head = current->next;
            }
            free(current);
            return 1; // 删除成功
        }
        prev = current;
        current = current->next;
    }
    return 0; // 未找到匹配的记录
}

// 根据备注查找记录
Node* find_record(Node* head, const char* note) {
    Node* current = head;
    while (current) {
        if (strcmp(current->note, note) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL; // 未找到匹配的记录
}

// 修改备注内容
void edit_note(Node* record, const char* new_note) {
    if (!record || !new_note) return;

    // 清除旧的note内容，然后复制新的内容
    strncpy(record->note, new_note, sizeof(record->note) - 1);
    record->note[sizeof(record->note) - 1] = '\0'; 
}

// 释放链表中所有的记录
void free_all_records(Node** head) {
    Node* current = *head;
    while (current) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    *head = NULL;
}