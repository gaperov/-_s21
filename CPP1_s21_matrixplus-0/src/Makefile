CC = gcc             
CFLAGS = -Wall -Werror -Wextra -g -std=c++17   
LIBRARY = s21_matrix_oop.a
SRC_FILES = s21_matrix_oop.cc
TEST_FILES = tests/s21_matrix_test.cc


OBJS = $(SRC_FILES:.cc=.o)
TEST_OBJ = $(TEST_FILES:.cc=.o)


all: $(LIBRARY) test
	rm $(OBJS)

test: CFLAGS += -lgtest -lgtest_main -pthread -lstdc++
test: $(TEST_OBJ) $(LIBRARY)
	$(CC) $(CFLAGS) -o s21_matrix_test $^
	./s21_matrix_test

$(LIBRARY): $(OBJS)
	ar rcs $@ $(OBJS)


%.o: %.cc
	$(CC) $(CFLAGS) -c $< -o $@

linters:
	cp ../materials/linters/.clang-format .
	clang-format -n *.cc *.h tests/*.cc
	rm .clang-format

clean:
	rm -f $(OBJS) $(LIBRARY) s21_matrix_test $(TEST_OBJ)
	