/* ranklist - 实现文件*/
#include "ranklist.h"
#include "sqlite3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
Node* read_from_database(sqlite3* db) {
    char* errmsg = 0;
    char sql[512];
    sqlite3_stmt* stmt;

    snprintf(sql, sizeof(sql), "SELECT 游戏时间, 分数, 游戏时长, 备注 FROM `ranklist`;");
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
void insertNodeToDatabase(Node* node, sqlite3* db,const char* user_name) {
    char *errMsg = 0;
    char sql[512];
 
    // 准备 SQL 语句
    snprintf(sql, sizeof(sql),
             "INSERT INTO ranklist (用户名, 游戏时间, 分数, 游戏时长, 备注) VALUES ('%s', '%s', %d, %d, '%s');",
             user_name,node->start_time, node->score, node->duration, node->note);
 
    // 执行 SQL 语句
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    } else {
        fprintf(stdout, "Data inserted successfully\n");
    }
}

//删除节点并同步删除数据库中的记录
void del_in_database(sqlite3* db,Node** head, const char* start_time){
    delete_node(head, start_time);
    // 然后从数据库中删除记录
    char sql[512]; 
    snprintf(sql, sizeof(sql), "DELETE FROM ranklist WHERE 游戏时间='%s';", start_time);
 
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare delete statement: %s\n", sqlite3_errmsg(db));
        return; 
    }
 
    // 执行SQL语句
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Failed to delete record from database: %s\n", sqlite3_errmsg(db));
    }
 
    // 释放DELETE语句对象
    sqlite3_finalize(stmt);
}

//将链表数据同步到数据库（用于删除和修改备注的操作之后）
void update_database(sqlite3* db, Node* head) {
    char sql_update[512];
    snprintf(sql_update, sizeof(sql_update),
             "UPDATE `ranklist` SET 分数=?, 游戏时长=?, 备注=? WHERE 游戏时间=?;");
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
        sqlite3_bind_text(stmt_update, 5, current->note ? current->note : "", -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt_update, 3, current->score);
        sqlite3_bind_int(stmt_update, 4, current->duration);
        sqlite3_bind_text(stmt_update, 2, current->start_time ? current->start_time : "", -1, SQLITE_STATIC);
 
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
    snprintf(sql_delete, sizeof(sql_delete), "DELETE FROM `ranklist` WHERE 游戏时间 IS NULL;");
 
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
void execute_sort_query(sqlite3* db, const char *sort_by) {
    char *err_msg = 0;
    char sql[256];

    // 构建SQL查询语句
    snprintf(sql, sizeof(sql), "SELECT * FROM `ranklist` ORDER BY `%s`;", sort_by);

    // 执行查询
    sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (err_msg) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf("Query executed successfully.\n");
    }
}



// 创建一个新的游戏记录节点
Node* create_node(const char* start_time, int score, int duration, const char* note) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        //perror("Failed to allocate memory for new node");
        return NULL;
    }

    // 使用strncpy复制字符串，确保不会超出数组边界
    strncpy(new_node->start_time, start_time, sizeof(new_node->start_time) - 1);
    new_node->start_time[sizeof(new_node->start_time) - 1] = '\0';

    new_node->score = score;

    new_node->duration = duration;

    strncpy(new_node->note, note, sizeof(new_node->note) - 1);
    new_node->note[sizeof(new_node->note) - 1] = '\0';

    new_node->next = NULL;
    return new_node;
}

// 添加一个新记录到链表中
void add_node(Node** head, Node* new_node) {
    if (!new_node) return;
    new_node->next = *head;
    *head = new_node;
}

// 删除指定开始时间的节点
int delete_node(Node** head, const char* start_time) {
    Node* current = *head;
    Node* prev = NULL;

    while (current) {
        if (strcmp(current->start_time, start_time) == 0) {
            if (prev) {
                prev->next = current->next;
            } else {
                *head = current->next;
            }
            free(current);
            return 1; // 删除成功
        }
        prev = current;
        current = current->next;
    }
    
    return 0; // 未找到匹配的记录
}

//按游戏时间查找节点
Node* find_node(Node* head, const char* start_time) {
    Node* current = head;
    while (current != NULL) {
        if (strcmp(current->start_time, start_time) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL; // 没有找到匹配的节点
}

// 修改备注内容
void edit_note(Node* head, const char* start_time, const char* new_note) {
    Node* node = find_node(head, start_time);
    if (node && new_note) {
        // 清除旧的note内容，然后复制新的内容
        strncpy(node->note, new_note, sizeof(node->note) - 1);
        node->note[sizeof(node->note) - 1] = '\0'; // 确保字符串以null结尾
    }
}

// 释放链表中所有的节点
void free_all_nodes(Node** head) {
    Node* current = *head;
    while (current) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    *head = NULL;
}