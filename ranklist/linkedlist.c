/* linkedlist.c - 实现文件 */
#include "linkedlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// 创建一个新的游戏记录节点
Node* create_node(const char* start_time, int score, int duration, const char* note) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        //perror("Failed to allocate memory for new node");
        return NULL;
    }

    // 使用strncpy复制字符串，确保不会超出数组边界
    strncpy(new_node->start_time, start_time, sizeof(new_node->start_time) - 1);
    new_node->start_time[sizeof(new_node->start_time) - 1] = '\0';

    new_node->score = score;

    new_node->duration = duration;

    strncpy(new_node->note, note, sizeof(new_node->note) - 1);
    new_node->note[sizeof(new_node->note) - 1] = '\0';

    new_node->next = NULL;
    return new_node;
}

// 添加一个新记录到链表中
void add_node(Node** head, Node* new_node) {
    if (!new_node) return;
    new_node->next = *head;
    *head = new_node;
}

// 删除指定开始时间的记录
int delete_node(Node** head, const char* start_time) {
<<<<<<< HEAD
    Node* current = head;
=======
    Node* current = *head;
>>>>>>> ef1b3f318a009b474c6ded5347cf945feaa5f110
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
<<<<<<< HEAD
    
=======
>>>>>>> ef1b3f318a009b474c6ded5347cf945feaa5f110
    return 0; // 未找到匹配的记录
}

//按游戏时间查找节点
Node* find_node(Node* head, const char* start_time) {
    Node* current = head;
    while (current != NULL) {
        if (strcmp(current->start_time, start_time) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL; // 没有找到匹配的节点
}

// 修改备注内容
void edit_note(Node* head, const char* start_time, const char* new_note) {
    Node* node = find_node(head, start_time);
    if (node && new_note) {
        // 清除旧的note内容，然后复制新的内容
        strncpy(node->note, new_note, sizeof(node->note) - 1);
        node->note[sizeof(node->note) - 1] = '\0'; // 确保字符串以null结尾
    }
}

// 释放链表中所有的记录
void free_all_nodes(Node** head) {
    Node* current = *head;
    while (current) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    *head = NULL;
}