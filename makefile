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
	$(MAKE) unitTest --no-print-directory
	$(MAKE) e2eTests --no-print-directory

.PHONY: unitTest
unitTest: 
	@mkdir -p target/testCoverage
	@cd target/testCoverage 
	$(CC) $(C_FLAGS) -fprofile-arcs -ftest-coverage ./src/*.c ./test/*.c -I./include -o target/testCoverage/test.out
	@echo "###############################################"
	@echo "Unit Test Run:"
	@cd target/testCoverage && ./test.out
	@echo "###############################################"
	@echo "Unit Test Coverage:"
	@mv *.gcno *.gcda target/testCoverage/
	@gcov -o target/testCoverage ./src/*.c
	@mv *.gcov target/testCoverage/
	@cd target/testCoverage && rm -rf *.gcno *.gcda

.PHONY: e2eTests
e2eTests:
	@cd e2eTests && $(MAKE) buildLib
	@cd e2eTests && $(MAKE) compileTests
	@cd e2eTests && $(MAKE) runTests
	@cd e2eTests && $(MAKE) cleanTests

.PHONY:analyze
analyze:
#	Valgrind not for macos :0
# 	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes --error-exitcode=1 ./target/app.out

#	TODO: review the following
#	Clang-tidy
	@clang-tidy --quiet -checks=bugprone-*,-bugprone-easily-swappable-parameters,clang-analyzer-*,cert-*,concurrency-*,misc-*,-misc-include-cleaner,modernize-*,performance-*,readability-*,-clang-diagnostic-deprecated-declarations --warnings-as-errors=* ./src/*.c -- -I./include

#	Scan-build
# @scan-build --status-bugs --keep-cc --show-description make

#	Clang Static Analyzer
# @clang --analyze -Xanalyzer -analyzer-output=text -I./include ./src/*.c

#TODO: Yet to be implemented
# @clang -fsanitize=address ./src/*.c -I./include -o target/sanitizer.out
    # @./target/sanitizer.out

	

#TODO: Yet to be implemented
# .PHONY:regression
# regression:
# 	$(MAKE) clean --no-print-directory
# 	$(MAKE) app --no-print-directory
# 	$(MAKE) test --no-print-directory
# 	$(MAKE) analyze --no-print-directory
# 	./test.out
# 	$(MAKE) memcheck --no-print-directory

.PHONY:clean
clean:
	@rm -rf target