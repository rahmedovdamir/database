#ifndef VALUE_H
#define VALUE_H

#include "core.h"

Value* create_int_value(int val);
Value* create_float_value(float val);
Value* create_string_value(const char* val);
Value* create_bool_value(bool val);

void print_value(Value* val);

int compare_values(Value* a, Value* b);

void free_value(Value* val);

#endif
