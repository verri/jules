TEMPDIR := $(shell mktemp -d -u)
ROOTDIR := $(shell pwd)
VERSION := $(shell git describe --long --tags | sed 's/\([^-]*\)-.*/\1/g')
HEADERS := $(shell find include -name \*.[ch]pp)
SRC := $(shell find test benchmark -name \*.[ch]pp) $(HEADERS)
ZIP := jules-$(VERSION).zip

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
	@clang-tidy $(HEADERS) -fix -fix-errors -- -std=c++1z -I.

clean:
	@$(MAKE) --no-print-directory -C test clean
	@echo Cleaning gcov files...
	@find . -name '*.gcno' -exec rm {} \;
	@find . -name '*.gcda' -exec rm {} \;
	@find . -name '*.gcov' -exec rm {} \;

get-deps: third_party/range-v3 third_party/debug_assert

third_party/range-v3:
	@echo Getting range-v3...
	@git clone https://github.com/ericniebler/range-v3.git third_party/range-v3

third_party/debug_assert:
	@echo Getting debug_assert...
	@git clone https://github.com/foonathan/debug_assert.git third_party/debug_assert

release: $(ZIP)

$(ZIP): third_party/range-v3 third_party/debug_assert
	util/release $(ZIP) $(TEMPDIR) $(ROOTDIR)

.PHONY: format test clean tidy get-deps release
