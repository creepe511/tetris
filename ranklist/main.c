#include "dbtolist.h"
#include "linkedlist.h"
#include "sqlite3.h"

int main(){
    Node* head;
    const char* table_name = "testTable";
    sqlite3* grDB = open_database();
    Node* new_node = create_node("2024-12-19-16:22", 666,60,"66");
    add_node(&head,new_node);
    update_database(grDB,head,table_name);
    sqlite3_close(grDB);
    free_all_nodes(&head);

    return 0;
}