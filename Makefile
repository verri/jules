SRC = $(shell find include test -iname \*.[ch]pp)

all: format test

test:
	@$(MAKE) --no-print-directory -C test test

format:
	@echo Formatting source...
	@clang-format -i -style=file $(SRC)

clean:
	@$(MAKE) --no-print-directory -C test clean

.PHONY: format test clean
