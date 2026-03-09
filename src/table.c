#include "../include/table.h"
#include <stdio.h>

Table* create_table(const char* name, Column* columns, int num_columns) {
    Table* table = (Table*)malloc(sizeof(Table));
    strncpy(table->name, name, MAX_NAME_LEN - 1);    
    table->columns = (Column*)malloc(sizeof(Column) * num_columns);
    memcpy(table->columns, columns, sizeof(Column) * num_columns);
    table->num_columns = num_columns;
    table->capacity = MAX_ROWS;
    table->rows = (Row*)malloc(sizeof(Row) * table->capacity);
    table->num_rows = 0;
    for (int i = 0; i < table->capacity; i++) {
        table->rows[i].values =NULL;
        table->rows[i].is_valid =false;
    }
    
    return table;
}

bool insert_row(Table* table, Value** values) {
    Row* row = &table->rows[table->num_rows];
    row->values = (Value**)malloc(sizeof(Value*) * table->num_columns);
    for (int i = 0; i < table->num_columns; i++) {
        row->values[i] = values[i];
    }
    row->num_columns = table->num_columns;
    row->is_valid = true;
    table->num_rows++;
    
    return true;
}

void select_all(Table* table) {
    printf("таблица: %s",table->name);
    for (int i = 0; i < table->num_columns; i++) {
        printf("%s\t", table->columns[i].name);
    }
    printf("\n");
    
    for (int i = 0; i < table->num_rows; i++) {
        if (table->rows[i].is_valid) {
            for (int j = 0; j < table->num_columns; j++) {
                print_value(table->rows[i].values[j]);
                printf("\t");
            }
            printf("\n");
        }
    }
    printf("\n");
}

void free_table(Table* table) {
    for (int i = 0; i < table->num_rows; i++) {
        if (table->rows[i].is_valid) {
            for (int j = 0; j < table->num_columns; j++) {
                free_value(table->rows[i].values[j]);
            }
            free(table->rows[i].values);
        }
    }
    free(table->rows);
    free(table->columns);
    free(table);
}

int delete_rows(Table* table,int column_index,Value* condition,int operation) {
    int deleted = 0;
    for (int i = 0; i < table->num_rows; i++) {
        if (!table->rows[i].is_valid) continue;
        Value* val = table->rows[i].values[column_index];
        int cmp = compare_values(val, condition);
        bool match = false;
        switch (operation) {
            case 0:
                match = (cmp == 0);
                break;
            case 1:
                match = (cmp > 0);
                break;
            case 2: 
                match = (cmp < 0);
                break;
        }
        if (match) {
            for (int j = 0; j < table->num_columns; j++) {
                free_value(table->rows[i].values[j]);
            }
            free(table->rows[i].values);
            table->rows[i].is_valid = false;
            deleted++;
        }
    }
    return deleted;
}