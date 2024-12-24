#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ranklist.c"

int main(){
    FILE *file;
    char line[64];
    char user_name[64];
    char *pos;
 
    // 打开文件
    file = fopen("temp.txt", "r");
    if (file == NULL) {
        perror("无法打开文件");
        return EXIT_FAILURE;
    }
 
    // 读取文件中的一行
    if (fgets(line, sizeof(line), file) == NULL) {
        perror("无法读取文件内容");
        fclose(file);
        return EXIT_FAILURE;
    }
 
    // 读取文件中的一行（也就是user_name）
    if (fgets(user_name, sizeof(user_name), file) == NULL) {
        perror("无法读取文件内容");
        fclose(file);
        return EXIT_FAILURE;
    }
 
    // 移除换行符（如果存在）
    user_name[strcspn(user_name, "\n")] = '\0';
 
    // 关闭文件
    fclose(file);

    Node* head = read_list_from_file("history.txt");
    Node *current = head;
    while (current != NULL) {
        Node* target = find_node(head,user_name);
    }
    
}