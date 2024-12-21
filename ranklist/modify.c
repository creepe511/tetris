#include <stdlib.h>
#include <stdio.h>
#include "ranklist.h"
#include "sqlite3.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <start_time> <new_note>\n", argv[0]);
        return EXIT_FAILURE;
    }
    Node* head;
    sqlite3* grDB = open_database();
    head = read_from_database(grDB);
    edit_note(head, argv[1], argv[2]);//argc[1]为游戏时间，argc[2]为新备注
    update_database(grDB,head);
    sqlite3_close(grDB);
    free_all_nodes(&head);

    return EXIT_SUCCESS;
}