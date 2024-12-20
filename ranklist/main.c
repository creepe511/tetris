#include "dbtolist.h"
#include "linkedlist.h"
#include "sqlite3.h"

int main(){
    Node* head;
    const char* table_name = "testTable";
    sqlite3* grDB = open_database();
    Node* new_node = create_node("2024-12-19-16:22", 666,60,"66");
    add_node(&head,new_node);
<<<<<<< HEAD
    insertNodeToDatabase(new_node, grDB, table_name);
=======
    update_database(grDB,head,table_name);
>>>>>>> ef1b3f318a009b474c6ded5347cf945feaa5f110
    sqlite3_close(grDB);
    free_all_nodes(&head);

    return 0;
}