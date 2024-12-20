// dbtolist - 头文件

#ifndef DBTOLIST_H
#define DBTOLIST_H

#include "linkedlist.h"
#include "sqlite3.h"

//打开数据库
sqlite3* open_database();

//关闭数据库使用 sqlite3_close(sqlite* db)

// 将链表节点的数据插入到 SQLite 数据库中
void insertNodeToDatabase(Node* node, sqlite3* db);

//删除节点并同步删除数据库中的记录
void del_in_database(sqlite3* db,Node** head, const char* start_time);

// 从SQLite数据库中读取数据并创建链表(返回head)
Node* read_from_database(sqlite3* db);

//将链表数据同步到数据库
void update_database(sqlite3* db, Node* head);

//按顺序查询数据库
void execute_sort_query(sqlite3* db, const char *sort_by);
//当输入的参数为“游戏时间”则按“游戏时间”排序，输入“分数”“游戏时长”同理




#endif // DBTOLIST_H