G = gcc
Gflags = -Wall -Werror -Wextra
all: s21_cat

s21_cat: cat.c
	$(G) $(Gflags) -c cat.c 
	$(G) $(Gflags) cat.o -o s21_cat

clean:
	rm -rf cat.o s21_cat

rebuild: clean all
