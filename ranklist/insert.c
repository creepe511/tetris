#include <stdlib.h>
#include <stdio.h>
#include "ranklist.h"
#include "sqlite3.h"
void insert(const char* start_time, int score, int duration,const char* user_name){
    Node* head;
    sqlite3* grDB = open_database();
    Node* new_node = create_node(start_time, score, duration, NULL);
    add_node(&head,new_node);
    insertNodeToDatabase(new_node, grDB, user_name);
    update_database(grDB,head);
    sqlite3_close(grDB);
    free_all_nodes(&head);
    
}