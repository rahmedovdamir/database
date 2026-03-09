#include "../include/storage.h"
#include "../include/query.h"
#include <assert.h>

void test_create_table() {
    printf("Тест создания таблицы\n");
    Database* db = init_database();
    Query* q = parse_query("create table test (id int, name string)");
    execute_query(db, q);
    
    Table* t = find_table(db, "test");
    assert(t != NULL);
    assert(t->num_columns == 2);
    assert(strcmp(t->columns[0].name, "id") == 0);
    assert(t->columns[0].type == TYPE_INT);
    
    free_query(q);
    free_database(db);
    printf("OK\n");
}

void test_insert_select() {
    printf("Тест вставки и выборки\n");
    
    Database* db = init_database();
    
    Query* q1 = parse_query("create table test (id int, name string)");
    execute_query(db, q1);
    
    Query* q2 = parse_query("insert into test values (1, 'John')");
    execute_query(db, q2);
    
    Table* t = find_table(db, "test");
    assert(t->num_rows == 1);
    assert(t->rows[0].values[0]->data.int_val == 1);
    assert(strcmp(t->rows[0].values[1]->data.string_val, "John") == 0);
    
    free_query(q1);
    free_query(q2);
    free_database(db);
    printf("OK\n");
}

void test_delete() {
    printf("Тест удаления\n");
    
    Database* db = init_database();
    
    Query* q1 = parse_query("create table test (id int, name string)");
    execute_query(db, q1);
    
    Query* q2 = parse_query("insert into test values (1, 'John')");
    execute_query(db, q2);
    Query* q3 = parse_query("insert into test values (2, 'Jane')");
    execute_query(db, q3);
    
    Table* t = find_table(db, "test");
    assert(t->num_rows == 2);
    Query* q4 = parse_query("delete from test where id=1");
    execute_query(db, q4);
    
    assert(t->num_rows == 2);
    int valid_rows = 0;
    for (int i = 0; i < t->num_rows; i++) {
        if (t->rows[i].is_valid) valid_rows++;
    }
    assert(valid_rows == 1);
    
    free_query(q1);
    free_query(q2);
    free_query(q3);
    free_query(q4);
    free_database(db);
    printf("OK\n");
}

int main() {
    printf("Запуск тестов\n");
    test_create_table();
    test_insert_select();
    test_delete();
    printf("Все тесты пройдены!\n");
    return 0;
}