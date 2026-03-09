```markdown
# База данных на C

Простая in-memory база данных 
## Команды базы данных

### Создание таблицы
```sql
create table users (id int, name string, age int)
```

### Вставка данных
```sql
insert into users values (1, 'Damir', 19)
```

### Выборка 
```sql
select * from users
```

### Удаление
```sql
delete from users where id=1
```

### Сохранение и загрузка
```sql
save
load
```

## Makefile команды

```bash
make        # компиляция
make run    # компиляция и запуск
make test   # компиляция тестов
make clean  # очистка
```