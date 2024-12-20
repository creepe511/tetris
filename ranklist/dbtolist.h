// dbtolist - 头文件

#ifndef DBTOLIST_H
#define DBTOLIST_H

#include "linkedlist.h"
#include "sqlite3.h"

//打开数据库
sqlite3* open_database();

//关闭数据库使用 sqlite3_close(sqlite* db)

<<<<<<< HEAD
// 将链表节点的数据插入到 SQLite 数据库中
void insertNodeToDatabase(Node* node, sqlite3* db, const char* table_name);

=======
>>>>>>> ef1b3f318a009b474c6ded5347cf945feaa5f110
// 从SQLite数据库中读取数据并创建链表(返回head)
Node* read_from_database(sqlite3* db, const char* table_name);

//将链表数据同步到数据库
void update_database(sqlite3* db, Node* head,const char* table_name);

//按顺序查询数据库
void execute_sort_query(sqlite3* db, const char *sort_by, const char* table_name);
//当输入的参数为“游戏时间”则按“游戏时间”排序，输入“分数”“游戏时长”同理




#endif // DBTOLIST_H