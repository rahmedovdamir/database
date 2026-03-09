#include "../include/core.h"
#include <stdio.h>
const char* data_type_to_string(DataType type) {
    switch (type) {
        case TYPE_INT:
            return "INT";
        case TYPE_FLOAT:
            return "FLOAT";
        case TYPE_STRING:
            return "STRING";
        case TYPE_BOOL:
            return "BOOL";
        default:
            return "UNKNOWN";
    }
}


