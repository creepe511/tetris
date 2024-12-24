/* ranklist - 实现文件*/
#include "ranklist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 创建新节点
Node* create_node(const char* start_time, int score, int duration, const char* user_name) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        // 内存分配失败处理
        return NULL;
    }
    strncpy(new_node->user_name, user_name, sizeof(new_node->user_name) - 1);
    strncpy(new_node->start_time, start_time, sizeof(new_node->start_time) - 1);
    new_node->score = score;
    new_node->duration = duration;
    new_node->next = NULL;
    return new_node;
}
// 添加一个新记录到链表尾部
void add_node(Node** head, Node* new_node) {
    if (*head == NULL) {
        // 如果链表为空，新节点将成为链表的头节点
        *head = new_node;
    } else {
        // 否则，遍历链表找到最后一个节点
        Node* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        // 将最后一个节点的next指针指向新节点
        current->next = new_node;
    }
}

//修改用户名
void modifyUserName(Node* head, const char* newName)
{
    Node* current = head;
    while (current!= NULL) {
        // 直接将当前节点的用户名修改为新用户名
        strncpy(current->user_name, newName, sizeof(current->user_name) - 1);
        current->user_name[sizeof(current->user_name) - 1] = '\0'; 
        current = current->next;
    }
    current = head;
    // 保存更新后的链表数据到文件
    saveToFile(head, "history.txt"); 
}

//按用户名查找节点
Node* find_node(Node* head, const char* user_name) {
    Node* current = head;
    while (current != NULL) {
        if (strcmp(current->user_name, user_name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL; // 没有找到匹配的节点
}

// 释放链表内存的函数
void free_list(Node* head) {
    Node* current = head;
    Node* next;
    while (current) {
        next = current->next;
        free(current);
        current = next;
    }
}

// 将链表写入到文件的函数
void write_list_to_file(Node* head, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("无法打开文件");
        exit(EXIT_FAILURE);
    }
 
    Node* current = head;
    while (current) {
        fprintf(file, "%s %s %d %d\n", 
        current->user_name, 
        current->start_time, 
        current->score, 
        current->duration);
        current = current->next;
    }
 
    fclose(file);
}
 
// 从文件读取链表
Node* read_list_from_file(const char* filename) {
    FILE *file;
    char line[128];
    char user_name[64];
    char start_time[64];
    int score, duration;
    Node* head = NULL;  // 链表头指针初始化为NULL
 
    // 打开文件
    file = fopen("history.txt", "r");
    if (file == NULL) {
        perror("无法打开文件");
        return NULL;
    }
 
    // 读取并解析文件中的每一行，创建节点并添加到链表中
    while (fgets(line, sizeof(line), file)) {
        // 移除字符串末尾的换行符
        line[strcspn(line, "\n")] = 0;
 
        // 使用sscanf解析行内容
        int result = sscanf(line, "%[^ ] %[^ ] %d %d", user_name, start_time, &score, &duration);
        if (result != 4) {
            fprintf(stderr, "行格式不正确: %s\n", line);
            continue;  // 跳过这一行，继续处理下一行
        }
 
        // 创建新节点并添加到链表中
        Node* new_node = create_node(start_time, score, duration, user_name);
        if (new_node != NULL) {
            add_node(&head, new_node);
        }
    }
    fclose(file);
    return head;
}

// 删除链表第11个节点（如果节点总数大于或等于11）
Node* delete_node(Node* head) {
    if (head == NULL) {
        return NULL;
    }
 
    Node *current = head, *previous = NULL;
    int count = 1;
 
    while (current != NULL) {
        if (count == 11) {
            // 找到第11个节点，进行删除操作
            if (previous != NULL) {
                previous->next = current->next;
            } else {
                // 如果要删除的是第一个节点，则更新头指针
                head = current->next;
            }
            free(current); // 释放已删除节点的内存
            break; // 退出循环，因为我们已经删除了节点
        }
        previous = current;
        current = current->next;
        count++;
    }
 
    // 如果count小于11，则没有进行删除操作，直接返回头指针
    return head;
}
 