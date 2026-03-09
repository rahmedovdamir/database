
#include "../include/query.h"
#include "../include/utils.h"
#include "../include/value.h"  
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>


Query* parse_query(const char* query_str) {
    Query* query = (Query*)malloc(sizeof(Query));
    if (!query) return NULL;
    
    memset(query, 0, sizeof(Query));
    
    char* query_copy = my_strdup(query_str);
    char* lower_query = my_strdup(query_str);
    
    if (!query_copy || !lower_query) {
        free(query_copy);
        free(lower_query);
        free(query);
        return NULL;
    }
    
    for (int i = 0; lower_query[i]; i++) {
        lower_query[i] = tolower(lower_query[i]);
    }
    
    if (strstr(lower_query, "create table") == lower_query) {
        query->type = CMD_CREATE_TABLE;
        sscanf(query_str, "create table %49s", query->table_name);
        
        const char* open_paren = strchr(query_str, '(');
        const char* close_paren = strchr(query_str, ')');
        
        if (open_paren && close_paren && close_paren > open_paren) {
            int cols_len = close_paren - open_paren - 1;
            if (cols_len > 0) {
                char* cols_buf = (char*)malloc(cols_len + 1);
                if (cols_buf) {
                    strncpy(cols_buf, open_paren + 1, cols_len);
                    cols_buf[cols_len] = '\0';
                    
                    char** col_defs;
                    int num_cols;
                    col_defs = split_string(cols_buf, ",", &num_cols);
                    
                    if (col_defs) {
                        query->num_columns = num_cols;
                        query->columns = (char**)malloc(sizeof(char*) * num_cols);
                        
                        if (query->columns) {
                            for (int i = 0; i < num_cols; i++) {
                                if (col_defs[i]) {
                                    query->columns[i] = my_strdup(col_defs[i]);
                                } else {
                                    query->columns[i] = NULL;
                                }
                            }
                        }
                        
                        free_string_array(col_defs, num_cols);
                    }
                    
                    free(cols_buf);
                }
            }
        }
    }
    else if (strstr(lower_query, "insert into") == lower_query) {
        query->type = CMD_INSERT;
        const char* values_ptr = strstr(query_str, "values");
        if (values_ptr) {
            char table_temp[50];
            sscanf(query_str, "insert into %49s", table_temp);
            strncpy(query->table_name, table_temp, MAX_NAME_LEN - 1);
            query->table_name[MAX_NAME_LEN - 1] = '\0';
            
            const char* open_paren = strchr(values_ptr, '(');
            const char* close_paren = strchr(values_ptr, ')');
            
            if (open_paren && close_paren && close_paren > open_paren) {
                int vals_len = close_paren - open_paren - 1;
                if (vals_len > 0) {
                    char* vals_buf = (char*)malloc(vals_len + 1);
                    if (vals_buf) {
                        strncpy(vals_buf, open_paren + 1, vals_len);
                        vals_buf[vals_len] = '\0';
                        
                        char** val_strings;
                        int num_vals = 0;
                        val_strings = split_string(vals_buf, ",", &num_vals);
                        
                        if (val_strings) {
                            query->num_values = num_vals;
                            query->values = (Value**)malloc(sizeof(Value*) * num_vals);
                            
                            if (query->values) {
                                for (int i = 0; i < num_vals; i++) {
                                    if (val_strings[i]) {
                                        char* val_copy = my_strdup(val_strings[i]);
                                        if (val_copy) {
                                            char* trimmed = trim_string(val_copy);
                                            
                                            if (trimmed[0] == '\'') {
                                                int len = strlen(trimmed);
                                                if (len >= 2 && trimmed[len-1] == '\'') {
                                                    trimmed[len-1] = '\0';
                                                    query->values[i] = create_string_value(trimmed + 1);
                                                } else {
                                                    query->values[i] = create_string_value(trimmed);
                                                }
                                            }
                                            else if (strcmp(trimmed, "true") == 0) {
                                                query->values[i] = create_bool_value(1);
                                            }
                                            else if (strcmp(trimmed, "false") == 0) {
                                                query->values[i] = create_bool_value(0);
                                            }
                                            else if (strchr(trimmed, '.') != NULL) {
                                                query->values[i] = create_float_value((float)atof(trimmed));
                                            }
                                            else {
                                                query->values[i] = create_int_value(atoi(trimmed));
                                            }
                                            
                                            free(val_copy);
                                        }
                                    }
                                }
                            }
                            
                            free_string_array(val_strings, num_vals);
                        }
                        
                        free(vals_buf);
                    }
                }
            }
        }
    }
    else if (strstr(lower_query, "select") == lower_query) {
        query->type = CMD_SELECT;
        
        if (strstr(lower_query, "select * from") == lower_query) {
            char table_temp[50];
            sscanf(query_str, "select * from %49s", table_temp);
            strncpy(query->table_name, table_temp, MAX_NAME_LEN - 1);
            query->table_name[MAX_NAME_LEN - 1] = '\0';
        }
    }
    else if (strstr(lower_query, "delete") == lower_query) {
        query->type = CMD_DELETE;
        
        const char* where_ptr = strstr(query_str, "where");
        if (where_ptr) {
            char table_temp[50];
            sscanf(query_str, "delete from %49s", table_temp);
            strncpy(query->table_name, table_temp, MAX_NAME_LEN - 1);
            query->table_name[MAX_NAME_LEN - 1] = '\0';
            
            char* condition_copy = my_strdup(where_ptr + 5); 
            if (condition_copy) {
                char* trimmed_cond = trim_string(condition_copy);
                
                char* eq_ptr = strchr(trimmed_cond, '=');
                if (eq_ptr) {
                    *eq_ptr = '\0';
                    char* col_name = trim_string(trimmed_cond);
                    char* val_str = trim_string(eq_ptr + 1);
                    
                    query->condition_column = 0;
                    query->operation = 0; 
                    
                    if (val_str[0] == '\'') {
                        int len = strlen(val_str);
                        if (len >= 2 && val_str[len-1] == '\'') {
                            val_str[len-1] = '\0';
                            query->condition_value = create_string_value(val_str + 1);
                        } else {
                            query->condition_value = create_string_value(val_str);
                        }
                    }
                    else if (strcmp(val_str, "true") == 0) {
                        query->condition_value = create_bool_value(1);
                    }
                    else if (strcmp(val_str, "false") == 0) {
                        query->condition_value = create_bool_value(0);
                    }
                    else if (strchr(val_str, '.') != NULL) {
                        query->condition_value = create_float_value((float)atof(val_str));
                    }
                    else {
                        query->condition_value = create_int_value(atoi(val_str));
                    }
                }
                
                free(condition_copy);
            }
        }
    }
    else {
        query->type = CMD_UNKNOWN;
    }
    
    free(query_copy);
    free(lower_query);
    return query;
}

void execute_query(Database* db, Query* query) {
    Table* table;
    
    if (!db || !query) {
        printf("Ошибка: неверные параметры запроса\n");
        return;
    }
    
    switch (query->type) {
        case CMD_CREATE_TABLE:
            if (find_table(db, query->table_name)) {
                printf("Таблица %s уже существует\n", query->table_name);
                break;
            }
            
            if (query->num_columns <= 0) {
                printf("Ошибка: не указаны колонки\n");
                break;
            }
            
            Column* cols = (Column*)malloc(sizeof(Column) * query->num_columns);
            if (!cols) {
                printf("Ошибка выделения памяти\n");
                break;
            }
            
            for (int i = 0; i < query->num_columns; i++) {
                if (!query->columns[i]) continue;
                
                char col_name[MAX_NAME_LEN] = "";
                char col_type[MAX_NAME_LEN] = "int";
                
                char* col_copy = my_strdup(query->columns[i]);
                if (col_copy) {
                    char* trimmed = trim_string(col_copy);
                    
                    char* space = strchr(trimmed, ' ');
                    if (space) {
                        *space = '\0';
                        strncpy(col_name, trimmed, MAX_NAME_LEN - 1);
                        strncpy(col_type, space + 1, MAX_NAME_LEN - 1);
                    } else {
                        strncpy(col_name, trimmed, MAX_NAME_LEN - 1);
                    }
                    
                    free(col_copy);
                }
                
                col_name[MAX_NAME_LEN - 1] = '\0';
                col_type[MAX_NAME_LEN - 1] = '\0';
                
                strncpy(cols[i].name, col_name, MAX_NAME_LEN - 1);
                cols[i].name[MAX_NAME_LEN - 1] = '\0';
                cols[i].type = string_to_type(col_type);
            }
            
            table = create_table(query->table_name, cols, query->num_columns);
            if (table && add_table(db, table)) {
                printf("Таблица %s создана\n", query->table_name);
            } else {
                if (table) free_table(table);
                printf("Ошибка при создании таблицы %s\n", query->table_name);
            }
            
            free(cols);
            break;
            
        case CMD_INSERT:
            table = find_table(db, query->table_name);
            if (!table) {
                printf("Таблица %s не найдена\n", query->table_name);
                break;
            }
            
            if (query->num_values != table->num_columns) {
                printf("Ошибка: ожидалось %d значений, получено %d\n", 
                       table->num_columns, query->num_values);
                break;
            }
            
            if (insert_row(table, query->values)) {
                printf("Строка добавлена в таблицу %s\n", query->table_name);
                query->values = NULL;
                query->num_values = 0;
            } else {
                printf("Ошибка добавления строки\n");
            }
            break;
            
        case CMD_SELECT:
            table = find_table(db, query->table_name);
            if (!table) {
                printf("Таблица %s не найдена\n", query->table_name);
                break;
            }
            
            select_all(table);
            break;
            
        case CMD_DELETE:
            table = find_table(db, query->table_name);
            if (!table) {
                printf("Таблица %s не найдена\n", query->table_name);
                break;
            }
            
            if (query->condition_value) {
                int deleted = delete_rows(table, query->condition_column, 
                                         query->condition_value, query->operation);
                printf("Удалено строк: %d\n", deleted);
                query->condition_value = NULL;
            } else {
                printf("Ошибка: не указано условие удаления\n");
            }
            break;
            
        default:
            printf("Неизвестная команда\n");
    }
}

void free_query(Query* query) {
    if (!query) return;
    
    if (query->columns) {
        for (int i = 0; i < query->num_columns; i++) {
            if (query->columns[i]) {
                free(query->columns[i]);
            }
        }
        free(query->columns);
        query->columns = NULL;
    }
    
    if (query->values) {
        for (int i = 0; i < query->num_values; i++) {
            if (query->values[i]) {
                free_value(query->values[i]);
            }
        }
        free(query->values);
        query->values = NULL;
    }
    
    if (query->condition_value) {
        free_value(query->condition_value);
        query->condition_value = NULL;
    }
    
    free(query);
}