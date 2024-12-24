#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ranklist.c"

int main() {
    FILE *file;
    char line[128];
    int score, duration;
    char start_time[64], user_name[64] = "";
 
    // 打开文件以读取
    file = fopen("data.txt", "r");
    if (file == NULL) {
        perror("无法打开文件");
        return EXIT_FAILURE;
    }
 
    // 读取并处理第一行（score）
    if (fgets(line, sizeof(line), file) == NULL || sscanf(line, "%d", &score) != 1) {
        fprintf(stderr, "无法读取或解析第一行（score）\n");
        fclose(file);
        return EXIT_FAILURE;
    }
 
    // 读取并处理第二行（duration）
    if (fgets(line, sizeof(line), file) == NULL || sscanf(line, "%d", &duration) != 1) {
        fprintf(stderr, "无法读取或解析第二行（duration）\n");
        fclose(file);
        return EXIT_FAILURE;
    }
 
    // 读取并处理第三行（start_time）
    if (fgets(line, sizeof(line), file) == NULL) {
        fprintf(stderr, "无法读取第三行（start_time）\n");
        fclose(file);
        return EXIT_FAILURE;
    }
    // 去除字符串末尾的换行符
    line[strcspn(line, "\n")] = 0;
    strncpy(start_time, line, sizeof(start_time) - 1);
    start_time[sizeof(start_time) - 1] = '\0'; // 确保字符串以null结尾
 
    // 尝试读取第四行（user_name），如果不存在则默认为空字符串
    if (fgets(line, sizeof(line), file) != NULL) {
        // 去除字符串末尾的换行符
        line[strcspn(line, "\n")] = 0;
        strncpy(user_name, line, sizeof(user_name) - 1);
        user_name[sizeof(user_name) - 1] = '\0'; // 确保字符串以null结尾
    }
 
    // 关闭文件
    fclose(file);

    // 清空文件（这里选择删除并重新创建文件的方式）
    remove("data.txt");
    file = fopen("data.txt", "w");
    if (file != NULL) {
        fclose(file);
    } else {
        perror("无法重新创建文件");
        return EXIT_FAILURE;
    }
    //创建新节点并且将其按score大小插入链表
    Node* head = read_list_from_file("history.txt");
    Node* new_node = create_node(start_time,score,duration,user_name);
    Node* current = head;
    Node* previous = NULL;
 
    // 遍历链表找到第一个score小于新节点score的位置（或链表末尾）
    while (current != NULL && current->score >= score) {
        previous = current;
        current = current->next;
    }
 
    // 插入新节点
    new_node->next = current;
    if (previous == NULL) {
        // 如果链表为空或新节点应成为新的头节点
        head = new_node;
    } else {
        // 否则，将新节点插入到previous和current之间
        previous->next = new_node;
    }

    //写入文件
    write_list_to_file(head,"history.txt");
}
    
    
    
