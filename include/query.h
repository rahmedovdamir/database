#ifndef QUERY_H
#define QUERY_H

#include "core.h"
#include "storage.h"
#include "table.h"

typedef enum {
    CMD_CREATE_TABLE,
    CMD_INSERT,
    CMD_SELECT,
    CMD_DELETE,
    CMD_UNKNOWN
} CommandType;

typedef struct Query {
    CommandType type;
    char table_name[MAX_NAME_LEN];
    char** columns;
    Value** values;
    int num_columns;
    int num_values;
    int condition_column;
    Value* condition_value;
    int operation;
} Query;

Query* parse_query(const char* query_str);
void execute_query(Database* db, Query* query);
void free_query(Query* query);

#endif