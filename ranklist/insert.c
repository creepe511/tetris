#include <stdlib.h>
#include <stdio.h>
#include "ranklist.h"
#include "sqlite3.h"

int main(int argc, char *argv[]) {
    if (argc != 6) {
        fprintf(stderr, "Usage: %s <start_time> <score> <duration> <note> <user_name>\n", argv[0]);
        return EXIT_FAILURE;
    }
    Node* head;
    sqlite3* grDB = open_database();

    int score_val = atoi(argv[2]);
    int duration_val = atoi(argv[3]);

    Node* new_node = create_node(argv[1], score_val, duration_val, argv[4]);
    add_node(&head,new_node);
    insertNodeToDatabase(new_node,grDB,argv[5]);
    update_database(grDB,head);
    sqlite3_close(grDB);
    free_all_nodes(&head);

    return EXIT_SUCCESS;
}