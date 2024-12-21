#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ranklist.h"
#include "sqlite3.h"

#define MAX_LINE_LENGTH 256
#define MAX_STRING_VALUE 256

int main() {
    FILE *file;
    char line[MAX_LINE_LENGTH];
    int score, duration;
    char start_time[MAX_STRING_VALUE], user_name[MAX_STRING_VALUE] = "";
 
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

    // 输出读取到的内容（可选，用于验证）
    printf("score: %d\n", score);
    printf("duration: %d\n", duration);
    printf("start_time: %s\n", start_time);
    printf("user_name: %s\n", user_name); // 如果user_name是空的，这里将输出一个空字符串
 
    // 清空文件（这里选择删除并重新创建文件的方式）
    remove("data.txt");
    file = fopen("data.txt", "w");
    if (file != NULL) {
        fclose(file);
    } else {
        perror("无法重新创建文件");
        return EXIT_FAILURE;
    }

    
    
    Node* head;
    sqlite3* grDB = open_database();
    Node* new_node = create_node(start_time,score, duration, "");
    add_node(&head,new_node);
    insertNodeToDatabase(new_node,grDB,user_name);
    update_database(grDB,head);
    sqlite3_close(grDB);
    free_all_nodes(&head);
      

    return EXIT_SUCCESS;

    
}