#ifndef UTILS_H
#define UTILS_H

#include "core.h"

char* my_strdup(const char* str);
char** split_string(const char* str, const char* delim, int* count);
char* trim_string(char* str);
Value* string_to_value(const char* str, DataType type);
DataType string_to_type(const char* type_str);
void free_string_array(char** arr, int count);

#endif