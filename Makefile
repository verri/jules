HEADERS = $(shell find jules -name \*.[ch]pp)
SRC = $(shell find test benchmark -name \*.[ch]pp) $(HEADERS)

all: test

test:
	@$(MAKE) --no-print-directory -C test test

format:
	@echo Formatting source...
	@clang-format -i -style=file $(SRC)

tidy:
	@echo Tidying source...
	@clang-tidy $(HEADERS) -- -std=c++14 -I.

clean:
	@$(MAKE) --no-print-directory -C test clean

.PHONY: format test clean
