#include "../include/value.h"
#include <stdio.h>

Value* create_int_value(int val) {
    Value* v = (Value*)malloc(sizeof(Value));
    v->type = TYPE_INT;
    v->data.int_val = val;
    return v;
}

Value* create_float_value(float val) {
    Value* v = (Value*)malloc(sizeof(Value));
    v->type = TYPE_FLOAT;
    v->data.float_val = val;
    return v;
}

Value* create_string_value(const char* val) {
    Value* v = (Value*)malloc(sizeof(Value));
    v->type = TYPE_STRING;
    strncpy(v->data.string_val, val, MAX_STRING_LEN - 1);
    v->data.string_val[MAX_STRING_LEN - 1] = '\0';
    return v;
}

Value* create_bool_value(bool val) {
    Value* v = (Value*)malloc(sizeof(Value));
    v->type = TYPE_BOOL;
    v->data.bool_val = val;
    return v;
}

void print_value(Value* val) {
    if (!val) return;
    
    switch (val->type) {
        case TYPE_INT:
            printf("%d", val->data.int_val);
            break;
        case TYPE_FLOAT:
            printf("%.2f", val->data.float_val);
            break;
        case TYPE_STRING:
            printf("%s", val->data.string_val);
            break;
        case TYPE_BOOL:
            printf("%s", val->data.bool_val ? "true" : "false");
            break;
    }
}


int compare_values(Value* a, Value* b) {
    if (a->type != b->type) return -2;
    
    switch (a->type) {
        case TYPE_INT:
            if (a->data.int_val == b->data.int_val) return 0;
            return a->data.int_val > b->data.int_val ? 1 : -1;
        case TYPE_FLOAT:
            if (a->data.float_val == b->data.float_val) return 0;
            return a->data.float_val > b->data.float_val ? 1 : -1;
        case TYPE_STRING:
            return strcmp(a->data.string_val, b->data.string_val);
        case TYPE_BOOL:
            if (a->data.bool_val == b->data.bool_val) return 0;
            return a->data.bool_val ? 1 : -1;
    }
    return -2;
}

void free_value(Value* val) {
    if (val) free(val);
}