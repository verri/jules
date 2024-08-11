HEADERS := $(shell find include -name \*.[ch]pp)
SRC := $(shell find test benchmark -name \*.[ch]pp) $(HEADERS)

all:
	@echo Please, use CMake instead.

format:
	@echo Formatting source...
	@clang-format -i -style=file $(SRC)

tidy:
	@echo Tidying source...
	@clang-tidy $(HEADERS) -fix -fix-errors -- -std=c++20 -Iinclude -Ithird_party/range-v3/include -Ithird_party/debug_assert/

clean:
	@echo Cleaning gcov files...
	@find . -name '*.gcno' -exec rm {} \;
	@find . -name '*.gcda' -exec rm {} \;
	@find . -name '*.gcov' -exec rm {} \;

.PHONY: format clean tidy
