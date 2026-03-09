#include "include/storage.h"
#include "include/query.h"
#include "include/persistence.h"
#include <stdio.h>

int main() {
    Database* db = init_database();
    char input[256];
    printf("база данных\n");
    printf("Команды: CREATE TABLE, INSERT INTO, SELECT, DELETE, SAVE, LOAD, EXIT\n");
    printf("Пример: create table users (id int, name string, age int)\n");
    while (fgets(input, sizeof(input), stdin)) {
        input[strcspn(input, "\n")] = 0;
        if (strcmp(input, "exit") == 0) {
            break;
        }
        else if (strcmp(input, "save") == 0) {
            if (save_database(db, "database.txt")) {
                printf("База данных сохранена\n");
            } else {
                printf("Ошибка сохранения\n");
            }
        }
        else if (strlen(input) > 0) {
            Query* query = parse_query(input);
            if (query->type != CMD_UNKNOWN) {
                execute_query(db, query);
            } else {
                printf("Неверная команда\n");
            }
            free_query(query);
        }
    }
    
    free_database(db);
    return 0;
}