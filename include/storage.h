#ifndef STORAGE_H
#define STORAGE_H

#include "core.h"
#include "table.h"

typedef struct Database {
    Table* tables[MAX_TABLES];
    int num_tables;
} Database;

Database* init_database();

Table* find_table(Database* db, const char* name);

bool add_table(Database* db, Table* table);

bool remove_table(Database* db, const char* name);

void free_database(Database* db);

#endif 