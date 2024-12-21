
#include "ranklist.h"
#include "sqlite3.h"

int main(const char* start_time, const char* new_note){
    Node* head;
    sqlite3* grDB = open_database();
    head = read_from_database(grDB);
    edit_note(head, start_time, new_note);
    update_database(grDB,head);
    sqlite3_close(grDB);
    free_all_nodes(&head);
}