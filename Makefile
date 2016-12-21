HEADERS = $(shell find jules -name \*.[ch]pp -not -name undef\*)
SRC = $(shell find test benchmark -name \*.[ch]pp) $(HEADERS)

all: test

test:
	@$(MAKE) --no-print-directory -C test
	@echo "Running test suite..."
	@valgrind --error-exitcode=1 --leak-check=full test/test_suite -d yes

format:
	@echo Formatting source...
	@clang-format -i -style=file $(SRC)

tidy:
	@echo Tidying source...
	@clang-tidy $(HEADERS) -fix -fix-errors -- -std=c++14 -I.

clean:
	@$(MAKE) --no-print-directory -C test clean
	@find . -name '*.gcno' -exec rm {} \;
	@find . -name '*.gcda' -exec rm {} \;
	@find . -name '*.gcov' -exec rm {} \;

.PHONY: format test clean tidy
