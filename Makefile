SRC = $(shell find . -iname \*.[ch]pp)

format:
	@echo Formatting $(SRC)
	@clang-format -i -style=file $(SRC)
