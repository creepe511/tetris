#include <stdlib.h>
#include <stdio.h>
#include "ranklist.h"
#include "sqlite3.h"

int main() {
    sqlite3* grDB = open_database();
    char sql[512]; 
    snprintf(sql, sizeof(sql), "SELECT * FROM ranklist;");
 
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(grDB, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare select statement: %s\n", sqlite3_errmsg(grDB));
        return EXIT_FAILURE; 
    }
 
    // 执行SQL语句
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Failed to select the all users from database: %s\n", sqlite3_errmsg(grDB));
    }
 
    // 清理
    sqlite3_finalize(stmt);
    sqlite3_close(grDB);


    return EXIT_SUCCESS;
}