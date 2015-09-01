SRC = $(shell find include test -iname \*.[ch]pp)

all: format test

test:
	$(MAKE) -C test test

format:
	@echo Formatting $(SRC)
	@clang-format -i -style=file $(SRC)

.PHONY: format test
