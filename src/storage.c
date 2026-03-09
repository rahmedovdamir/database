#include "../include/storage.h"
#include <stdio.h>

Database* init_database() {
    Database* db = (Database*)malloc(sizeof(Database));
    db->num_tables = 0;
    for (int i = 0; i < MAX_TABLES; i++) {
        db->tables[i] = NULL;
    }
    return db;
}
Table* find_table(Database* db,const char* name) {
    for (int i = 0; i < db->num_tables; i++) {
        if (strcmp(db->tables[i]->name, name) == 0) {
            return db->tables[i];
        }
    }
    return NULL;
}

bool add_table(Database* db, Table* table) {
    if (find_table(db, table->name) != NULL) return false;    
    db->tables[db->num_tables++] = table;
    return true;
}

bool remove_table(Database* db, const char* name) {
    for (int i = 0; i < db->num_tables; i++) {
        if (strcmp(db->tables[i]->name, name) == 0) {
            free_table(db->tables[i]);
            for (int j = i; j < db->num_tables - 1; j++) {
                db->tables[j] = db->tables[j + 1];
            }
            db->num_tables--;
            return true;
        }
    }
    return false;
}

void free_database(Database* db) {
    for (int i = 0; i < db->num_tables; i++) {
        free_table(db->tables[i]);
    }
    free(db);
}