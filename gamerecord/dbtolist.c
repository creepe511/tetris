/* dbtolist - 实现文件*/

#include "linkedlist.h"
#include "dbtolist.h"
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>

Node *head = NULL;
int rc;
sqlite3_stmt *stmt = NULL;
const char* table_name;

//打开数据库
__declspec(dllexport) sqlite3* open_database(){
    sqlite3* db;
    rc = sqlite3_open("grDB.db", &db);
    if (rc) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }
    return db;
}


// 从SQLite数据库中读取数据并创建链表
__declspec(dllexport) Node* read_from_database(sqlite3* db) {
    char* errmsg = 0;
    char sql[512];
    snprintf(sql, sizeof(sql), "SELECT 游戏时间, 分数, 用户, 游戏时长, 备注 FROM %s;", table_name);
 
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return NULL;
    }
 
    Node* head = NULL;
    Node* current = NULL;
    Node* temp = NULL;
 
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const unsigned char* start_time = sqlite3_column_text(stmt, 0);
        int score = sqlite3_column_int(stmt, 1);
        int duration = sqlite3_column_int(stmt, 2);
        const unsigned char* note = sqlite3_column_text(stmt, 3);
 
        temp = create_node((const char*)start_time, score, duration, (const char*)note);
 
        if (head == NULL) {
            head = temp;
            current = head;
        } else {
            current->next = temp;
            current = temp;
        }
    }
 
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Execution failed: %s\n", sqlite3_errmsg(db));
    }
 
    sqlite3_finalize(stmt);
 
    return head;
}

//将链表数据同步到数据库
__declspec(dllexport) void update_database(sqlite3* db, Node* head) {
    const char* sql = ("UPDATE %s SET 分数=?, 游戏时长=?, 备注=? WHERE 游戏时间=?;",table_name);
    sqlite3_stmt* stmt;
 
    // 检查 SQL 语句的准备情况
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }
 
    // 遍历链表并更新数据库
    Node* current = head;
    while (current != NULL) {
        // 绑定参数到 SQL 语句
        sqlite3_reset(stmt); // 重置语句到其初始状态，以便可以重新绑定参数
        sqlite3_bind_text(stmt, 1, current->note, -1, SQLITE_STATIC); //绑定顺序不重要，只要索引正确即可
        sqlite3_bind_int(stmt, 2, current->score);
        sqlite3_bind_int(stmt, 3, current->duration);
        sqlite3_bind_text(stmt, 4, current->start_time, -1, SQLITE_STATIC); // 主键 start_time
 
        // 执行 SQL 语句
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            fprintf(stderr, "Failed to update record: %s\n", sqlite3_errmsg(db));
        }
        current = current->next;
    }
 
    // 释放 SQL 语句对象
    sqlite3_finalize(stmt);
}

//按顺序查询数据库
__declspec(dllexport) void execute_sort_query(const char *sort_by) {
    sqlite3 *db;
    char *err_msg = 0;
    char sql[256];
 
    open_database();
 
    // 构建SQL查询语句
    snprintf(sql, sizeof(sql), "SELECT * FROM %s ORDER BY %s;", table_name, sort_by);
 
    // 执行查询
    sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (err_msg) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf("Query executed successfully.\n");
    }
 
    // 关闭数据库
    sqlite3_close(db);
}
