#include "../include/utils.h"
#include "../include/value.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

char* my_strdup(const char* str) {
    if (!str) return NULL;
    size_t len = strlen(str) + 1;
    char* new_str = (char*)malloc(len);
    if (new_str) {
        memcpy(new_str, str, len);
    }
    return new_str;
}

char** split_string(const char* str, const char* delim, int* count) {
    if (!str || !delim || !count) return NULL;
    
    char* str_copy = my_strdup(str);
    if (!str_copy) return NULL;
    
    char** result = NULL;
    char* token;
    int num_tokens = 0;
    
    token = strtok(str_copy, delim);
    while (token) {
        result = (char**)realloc(result, sizeof(char*) * (num_tokens + 1));
        result[num_tokens] = my_strdup(token);
        num_tokens++;
        token = strtok(NULL, delim);
    }
    
    *count = num_tokens;
    free(str_copy);
    return result;
}

char* trim_string(char* str) {
    if (!str) return str;
    
    while (isspace((unsigned char)*str)) str++;
    
    if (*str == 0) return str;
    
    char* end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    
    end[1] = '\0';
    return str;
}

Value* string_to_value(const char* str, DataType type) {
    if (!str) return NULL;
    
    char* str_copy = my_strdup(str);
    if (!str_copy) return NULL;
    
    char* trimmed = trim_string(str_copy);
    Value* result = NULL;
    
    switch (type) {
        case TYPE_INT:
            result = create_int_value(atoi(trimmed));
            break;
        case TYPE_FLOAT:
            result = create_float_value((float)atof(trimmed));
            break;
        case TYPE_STRING:
            if (trimmed[0] == '\'') {
                int len = strlen(trimmed);
                if (len > 2 && trimmed[len-1] == '\'') {
                    trimmed[len-1] = '\0';
                    result = create_string_value(trimmed + 1);
                } else {
                    result = create_string_value(trimmed);
                }
            } else {
                result = create_string_value(trimmed);
            }
            break;
        case TYPE_BOOL:
            result = create_bool_value(strcmp(trimmed, "true") == 0);
            break;
    }
    
    free(str_copy);
    return result;
}

DataType string_to_type(const char* type_str) {
    if (!type_str) return TYPE_INT;
    
    char lower[20];
    strncpy(lower, type_str, 19);
    lower[19] = '\0';
    
    for (int i = 0; lower[i]; i++) {
        lower[i] = tolower(lower[i]);
    }
    
    if (strcmp(lower, "int") == 0) return TYPE_INT;
    if (strcmp(lower, "float") == 0) return TYPE_FLOAT;
    if (strcmp(lower, "string") == 0) return TYPE_STRING;
    if (strcmp(lower, "bool") == 0) return TYPE_BOOL;
    
    return TYPE_INT;
}

void free_string_array(char** arr, int count) {
    if (!arr) return;
    
    for (int i = 0; i < count; i++) {
        if (arr[i]) {
            free(arr[i]);
        }
    }
    free(arr);
}