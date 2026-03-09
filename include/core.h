#ifndef CORE_H
#define CORE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_TABLES 100
#define MAX_COLUMNS 10
#define MAX_ROWS 1000
#define MAX_NAME_LEN 50
#define MAX_STRING_LEN 100

typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING,
    TYPE_BOOL
} DataType;

typedef struct Value {
    DataType type;
    union {
        int int_val;
        float float_val;
        char string_val[MAX_STRING_LEN];
        bool bool_val;
    } data;
} Value;

#endif 