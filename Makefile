ROOTDIR := $(shell pwd)
VERSION := $(shell git describe --long --tags | sed 's/\([^-]*\)-.*/\1/g')
HEADERS := $(shell find include -name \*.[ch]pp)
SRC := $(shell find test benchmark -name \*.[ch]pp) $(HEADERS)
ZIP := jules-$(VERSION).zip

all:
	@echo Please, use CMake instead.

format:
	@echo Formatting source...
	@clang-format -i -style=file $(SRC)

tidy:
	@echo Tidying source...
	@clang-tidy $(HEADERS) -fix -fix-errors -- -std=c++1z -Iinclude

clean:
	@$(MAKE) --no-print-directory -C test clean
	@echo Cleaning gcov files...
	@find . -name '*.gcno' -exec rm {} \;
	@find . -name '*.gcda' -exec rm {} \;
	@find . -name '*.gcov' -exec rm {} \;

release: $(ZIP)

$(ZIP):
	util/release $(ZIP) $(ROOTDIR)

.PHONY: format clean tidy get-deps release
