
#include "ranklist.h"
#include "sqlite3.h"

int main(const char* start_time){
    Node* head;
    sqlite3* grDB = open_database();
    head = read_from_database(grDB);
    del_in_database(grDB, &head, start_time);
    update_database(grDB,head);
    sqlite3_close(grDB);
    free_all_nodes(&head);
}