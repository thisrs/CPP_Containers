CFLAGS=-std=c++17 -Wall -Wextra -pedantic -pthread
TESTFLAGS=-lgtest -lgtest_main -lpthread
CC=g++
EXEC_NAME=test.out
SOURCES_LIB=s21_containers.h
OBJECTS_LIB=$(SOURCES_LIB:.cpp=.o)
TESTS_OBJ=array_test.cpp map_test.cpp multiset_test.cpp queue_test.cpp set_test.cpp stack_test.cpp test_list.cpp vector_test.cpp


.PHONY: all clean test
all: clean test

clean:
	-rm -rf *.out *.o *.gcda *.gcno *.info
	-rm -rf test

test:
	$(CC) $(CFLAGS) $(TESTS_OBJ) $(TESTFLAGS) -o test
	./test
