// dbtolist - 头文件

#ifndef DBTOLIST_H
#define DBTOLIST_H

#include <stdio.h>
#include "sqlite3.h"

//规定数据库名为grDB.db,表名为ranklist
// 定义游戏记录节点结构体
typedef struct Node {
    char start_time[64];              // 游戏开始时间
    int score;                       // 游戏分数
    int duration;               // 游戏时长（秒）
    char note[128];                   // 游戏备注
    struct Node* next;               // 指向下一个节点的指针
} Node;

// 创建一个新的游戏记录节点(返回一个新节点new_node)
__declspec(dllexport) Node* create_node(const char* start_time, int score, int duration, const char* note);

// 添加一个新记录到链表中（传入头节点head的地址&head）
__declspec(dllexport) void add_node(Node** head, Node* new_node);

// 删除链表中指定开始时间的节点
__declspec(dllexport) int delete_node(Node** head, const char* start_time);

//按游戏时间查找节点
__declspec(dllexport) Node* find_node(Node* head, const char* start_time);

// 修改链表节点中备注内容
__declspec(dllexport) void edit_note(Node* head, const char* start_time, const char* new_note);

// 释放链表中所有的记录
__declspec(dllexport) void free_all_nodes(Node** head);

//打开数据库（规定数据库名为grDB.db,表名为ranklist)
__declspec(dllexport) sqlite3* open_database();

//关闭数据库使用 sqlite3_close(sqlite* db)

// 将链表节点的数据插入到 SQLite 数据库中
__declspec(dllexport) void insertNodeToDatabase(Node* node, sqlite3* db, const char* user_name);

//删除节点并同步删除数据库中的记录
__declspec(dllexport) void del_in_database(sqlite3* db,Node** head, const char* start_time);

// 从SQLite数据库中读取数据并创建链表(返回head)
__declspec(dllexport) Node* read_from_database(sqlite3* db);

//将链表数据同步到数据库
__declspec(dllexport) void update_database(sqlite3* db, Node* head);

//按顺序查询数据库
__declspec(dllexport) void execute_sort_query(sqlite3* db, const char *sort_by);
//当输入的参数为“游戏时间”则按“游戏时间”排序，输入“分数”“游戏时长”同理


#endif // DBTOLIST_H