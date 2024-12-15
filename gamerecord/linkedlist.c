#include<stdio.h>
#include<stdlib.h>
#include<time.h>

typedef struct Node 
{
    int score;
    time_t start_time;
    time_t duration_time;
    struct Node* next;
}Node;

//创建节点的函数
Node* creatNode(int score, time_t start_time, time_t duration_time)
{
    Node* newNode = (Node*)malloc(sizeof(Node));//分配内存，在数据导入sql数据库后释放
    if (newNode == NULL) {
        // 内存分配失败，打印错误信息并退出程序
        printf("Failed to allocate memory for new node");
        exit;
    }
 
    // 初始化新节点的成员变量
    newNode->score = score;
    newNode->start_time = start_time;
    newNode->duration_time = duration_time;
    newNode->next = NULL;
 
    return newNode; // 返回新节点的指针
}


