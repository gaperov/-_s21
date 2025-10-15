G = gcc
Gflags = -Wall -Werror -Wextra
all: s21_grep

s21_grep: grep.c
	$(G) $(Gflags) -c grep.c 
	$(G) $(Gflags) grep.o -o s21_grep

clean:
	rm -rf grep.o s21_grep

rebuild: clean all
