CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -g
SRC = src/*.c
TARGET = database
TEST_TARGET = test_db

all: $(TARGET)

$(TARGET): main.c $(SRC)
	$(CC) $(CFLAGS) main.c $(SRC) -o $(TARGET)

test: $(TEST_TARGET)

$(TEST_TARGET): tests/test_db.c $(SRC)
	$(CC) $(CFLAGS) tests/test_db.c $(SRC) -o $(TEST_TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all test clean run