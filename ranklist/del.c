#include <stdlib.h>
#include <stdio.h>
#include "ranklist.h"
#include "sqlite3.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <start_time>\n", argv[0]);
        return EXIT_FAILURE;
    }

    Node* head;
    sqlite3* grDB = open_database();
    head = read_from_database(grDB);
    del_in_database(grDB, &head, argv[1]);
    update_database(grDB,head);
    sqlite3_close(grDB);
    free_all_nodes(&head);

    return EXIT_SUCCESS;
}