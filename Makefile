SRC = $(shell find include test -iname \*.[ch]pp)

format:
	@echo Formatting $(SRC)
	@clang-format -i -style=file $(SRC)
