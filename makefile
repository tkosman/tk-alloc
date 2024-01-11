CC := gcc
C_FLAGS := -Wall -Wextra -Wno-deprecated-declarations

.PHONY:all
all:
	$(MAKE) build run

.PHONY:build
build:
	@mkdir -p target
	$(CC) $(C_FLAGS) ./src/*.c ./app/*.c -I./include -o target/app.out

.PHONY:run
run:
	@./target/app.out

.PHONY:test
test:
	@mkdir -p target
	$(CC) $(C_FLAGS) ./src/*.c ./test/*.c -I./include -o target/test.out
	@./target/test.out

.PHONY:analyze
analyze: 
	@mkdir -p target/testCoverage
	@cd target/testCoverage 
	$(CC) $(C_FLAGS) -fprofile-arcs -ftest-coverage ./src/*.c ./test/*.c -I./include -o target/testCoverage/test.out
	@cd target/testCoverage && ./test.out
	@mv *.gcno *.gcda target/testCoverage/
	@gcov -o target/testCoverage ./src/*.c
	@mv *.gcov target/testCoverage/
	@cd target/testCoverage && rm -rf *.gcno *.gcda




#TODO: Yet to be implemented

.PHONY:memcheck
memcheck: test
	

.PHONY:regression
regression:
	$(MAKE) clean --no-print-directory
	$(MAKE) app --no-print-directory
	$(MAKE) test --no-print-directory
	$(MAKE) analyze --no-print-directory
	./test.out
	$(MAKE) memcheck --no-print-directory

.PHONY:clean
clean:
	@rm -rf target