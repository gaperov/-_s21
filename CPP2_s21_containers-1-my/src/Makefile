COMPILER = g++ -std=c++17

ifeq ($(shell uname), Linux)
    DISTRIBUTION := $(shell cat /etc/*-release 2>/dev/null | grep -i '^ID=' | cut -d'=' -f2)
	ifeq ($(DISTRIBUTION), fedora)
		TESTFLAGS = -lcheck -lsubunit -lm
		OPEN = xdg-open
	else
		TESTFLAGS = -lcheck_pic -lsubunit -lm
	 endif
endif
ifeq ($(shell uname), Darwin)
	TESTFLAGS = -lcheck
endif

clean:
	rm -rf obj
	rm -rf *.out
	rm -rf *.a
	rm -rf TEST
	rm -rf VL.txt

style:
	clang-format -i --style=Google tests/*.cpp tests/*.hpp containers/*.hpp *.hpp

check_style:
	clang-format -n --style=Google tests/*.cpp tests/*.hpp containers/*.hpp *.hpp

test: $(BIBLIO)
	$(COMPILER) tests/*.cpp -o TEST -lm -lgtest
	./TEST

leaks_test: 
	@valgrind --leak-check=full --log-file=VL.txt ./TEST
	cat VL.txt