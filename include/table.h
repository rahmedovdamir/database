#ifndef TABLE_H
#define TABLE_H

#include "core.h"
#include "value.h"

typedef struct Column {
    char name[MAX_NAME_LEN];
    DataType type;
} Column;

typedef struct Row {
    Value** values;
    int num_columns;
    bool is_valid;
} Row;

typedef struct Table {
    char name[MAX_NAME_LEN];
    Column* columns;
    int num_columns;
    Row* rows;
    int num_rows;
    int capacity;
} Table;

Table* create_table(const char* name, Column* columns, int num_columns);

bool insert_row(Table* table, Value** values);

void select_all(Table* table);

int delete_rows(Table* table, int column_index, Value* condition, int operation);

void free_table(Table* table);

#endif 