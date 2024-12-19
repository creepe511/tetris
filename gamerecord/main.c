#include "dbtolist.h"
#include "linkedlist.h"
#include "sqlite3.h"

int main(){
    Node* head;
    sqlite3* grDB = open_database();
    read_from_database(grDB);
    Node* new_node = create_node("2024-12-19-16:22", 666,60,"66");
    add_node(&head,new_node);
    update_database(grDB,head);
    sqlite3_close(grDB);
    free_all_nodes(&head);

    return 0;
}