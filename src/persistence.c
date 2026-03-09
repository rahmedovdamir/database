#include "../include/persistence.h"
#include "../include/utils.h"

bool save_database(Database* db, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) return false;
    fprintf(file, "DATABASE VERSION 1.0\n");
    fprintf(file, "TABLES %d\n", db->num_tables);
    
    for (int i = 0; i < db->num_tables; i++) {
        Table* t = db->tables[i];
        
        fprintf(file, "TABLE %s\n", t->name);
        fprintf(file, "COLUMNS %d\n", t->num_columns);
        
        for (int j = 0; j < t->num_columns; j++) {
            const char* type_str;
            switch (t->columns[j].type) {
                case TYPE_INT: type_str = "INT"; break;
                case TYPE_FLOAT: type_str = "FLOAT"; break;
                case TYPE_STRING: type_str = "STRING"; break;
                case TYPE_BOOL: type_str = "BOOL"; break;
            }
            fprintf(file, "%s %s\n", t->columns[j].name, type_str);
        }
        fprintf(file, "ROWS %d\n", t->num_rows);
        for (int j = 0; j < t->num_rows; j++) {
            if (t->rows[j].is_valid) {
                for (int k = 0; k < t->num_columns; k++) {
                    Value* v = t->rows[j].values[k];
                    switch (v->type) {
                        case TYPE_INT:
                            fprintf(file, "%d", v->data.int_val);
                            break;
                        case TYPE_FLOAT:
                            fprintf(file, "%.2f", v->data.float_val);
                            break;
                        case TYPE_STRING:
                            fprintf(file, "'%s'", v->data.string_val);
                            break;
                        case TYPE_BOOL:
                            fprintf(file, "%s", v->data.bool_val ? "true" : "false");
                            break;
                    }
                    if (k < t->num_columns - 1) fprintf(file, ",");
                }
                fprintf(file, "\n");
            }
        }
    }
    
    fclose(file);
    return true;
}

Database* load_database(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) return NULL;
    
    Database* db = init_database();
    char buffer[256];
    int num_tables;
    
    fgets(buffer, sizeof(buffer), file); // Версия
    fscanf(file, "TABLES %d\n", &num_tables);
    
    for (int i = 0; i < num_tables; i++) {
        char table_name[MAX_NAME_LEN];
        int num_columns, num_rows;
        
        fscanf(file, "TABLE %s\n", table_name);
        fscanf(file, "COLUMNS %d\n", &num_columns);
        
        Column* cols = (Column*)malloc(sizeof(Column) * num_columns);
        
        for (int j = 0; j < num_columns; j++) {
            char type_str[20];
            fscanf(file, "%s %s\n", cols[j].name, type_str);
            cols[j].type = string_to_type(type_str);
        }
        
        Table* table = create_table(table_name, cols, num_columns);
        free(cols);
        
        fscanf(file, "ROWS %d\n", &num_rows);
        
        for (int j = 0; j < num_rows; j++) {
            fgets(buffer, sizeof(buffer), file);
            buffer[strcspn(buffer, "\n")] = 0;
            
            char** values_str;
            int num_vals;
            values_str = split_string(buffer, ",", &num_vals);
            
            Value** values = (Value**)malloc(sizeof(Value*) * num_columns);
            
            for (int k = 0; k < num_columns; k++) {
                values[k] = string_to_value(values_str[k], table->columns[k].type);
            }
            
            insert_row(table, values);
            free_string_array(values_str, num_vals);
            free(values);
        }
        
        add_table(db, table);
    }
    fclose(file);
    return db;
}