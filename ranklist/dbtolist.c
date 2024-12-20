/* dbtolist - 实现文件*/

#include "linkedlist.h"
#include "dbtolist.h"
#include "sqlite3.h"
#include <stdio.h>
#include <stdlib.h>

//打开数据库
sqlite3* open_database(){
    sqlite3* db;
    int rc;
    rc = sqlite3_open("grDB.db", &db);
    if (rc != SQLITE_OK) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    return NULL;
    }   

    return db;
}


// 从SQLite数据库中读取数据并创建链表
Node* read_from_database(sqlite3* db, const char* table_name) {
    char* errmsg = 0;
    char sql[512];
    sqlite3_stmt* stmt;

    snprintf(sql, sizeof(sql), "SELECT 游戏时间, 分数, 游戏时长, 备注 FROM `%s`;", table_name);
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK){
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return NULL;
    }
            
    Node* head = NULL;
    Node* current = NULL;
    Node* temp = NULL;
            
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const unsigned char* start_time = sqlite3_column_text(stmt, 0);
        if (start_time == NULL) {
            start_time = (const unsigned char*)"";
        }

        int score = sqlite3_column_int(stmt, 1);
        if (sqlite3_column_type(stmt, 1) == SQLITE_NULL) {
            score = 0;  // 如果是 NULL，设置为 0 
        }


        int duration = sqlite3_column_int(stmt, 2);
        if (sqlite3_column_type(stmt, 2) == SQLITE_NULL) {
            duration = 0;  // 如果是 NULL，设置为 0 
        }

        const unsigned char* note = sqlite3_column_text(stmt, 3);
        if (note == NULL) {
        note = (const unsigned char*)"";
        }
            
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

// 将链表节点的数据插入到 SQLite 数据库中
void insertNodeToDatabase(Node* node, sqlite3* db,const char* table_name) {
    char *errMsg = 0;
    char sql[512];
 
    // 准备 SQL 语句
    snprintf(sql, sizeof(sql),
             "INSERT INTO %s (游戏时间, 分数, 游戏时长, 备注) VALUES ('%s', %d, %d, '%s');",table_name,
             node->start_time, node->score, node->duration, node->note);
 
    // 执行 SQL 语句
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    } else {
        fprintf(stdout, "Data inserted successfully\n");
    }
}

//删除节点并同步删除记录
void del_in_database()

//将链表数据同步到数据库（用于删除和修改备注的操作之后）
void update_database(sqlite3* db, Node* head, const char* table_name) {
    char sql_update[512];
    snprintf(sql_update, sizeof(sql_update),
             "UPDATE `%s` SET 分数=?, 游戏时长=?, 备注=? WHERE 游戏时间=?;", table_name);
    sqlite3_stmt* stmt_update;
 
    // 检查 SQL 语句的准备情况
    if (sqlite3_prepare_v2(db, sql_update, -1, &stmt_update, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare update statement: %s\n", sqlite3_errmsg(db));
        return;
    }
 
    // 遍历链表并更新数据库
    Node* current = head;
    while (current != NULL) {
        sqlite3_reset(stmt_update); // 重置语句到其初始状态
        sqlite3_bind_text(stmt_update, 1, current->note ? current->note : "", -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt_update, 2, current->score);
        sqlite3_bind_int(stmt_update, 3, current->duration);
        sqlite3_bind_text(stmt_update, 4, current->start_time ? current->start_time : "", -1, SQLITE_STATIC);
 
        // 执行 SQL 语句
        if (sqlite3_step(stmt_update) != SQLITE_DONE) {
            fprintf(stderr, "Failed to update record: %s\n", sqlite3_errmsg(db));
        }
        current = current->next;
    }
 
    // 释放更新语句对象
    sqlite3_finalize(stmt_update);
 
    // 准备 DELETE 语句以删除游戏时间为 NULL 的行
    char sql_delete[256];
    snprintf(sql_delete, sizeof(sql_delete), "DELETE FROM `%s` WHERE 游戏时间 IS NULL;", table_name);
 
    // 执行 DELETE 语句
    char *err_msg = 0;
    if (sqlite3_exec(db, sql_delete, 0, 0, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "Failed to execute delete statement: %s\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf("Successfully deleted rows with NULL 游戏时间.\n");
    }
}
    
//按顺序查询数据库
void execute_sort_query(sqlite3* db, const char *sort_by, const char* table_name) {
    char *err_msg = 0;
    char sql[256];

    // 构建SQL查询语句
    snprintf(sql, sizeof(sql), "SELECT * FROM `%s` ORDER BY `%s`;", table_name, sort_by);

    // 执行查询
    sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (err_msg) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf("Query executed successfully.\n");
    }
}
