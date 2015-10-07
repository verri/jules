SUFFIXES += .d

CELERO_SRC = $(shell find ../third_party/Celero/src -iname '*.cpp')

SRC = $(CELERO_SRC)
OBJ := $(SRC:.cpp=.o) test_suite.o
DEP := $(SRC:.cpp=.d) test_suite.d

CXXFLAGS += -std=c++14 -g -march=native -O3 -DNDEBUG -Wall -Wextra -I../include -isystem../third_party/Celero/include
LDFLAGS += -lpthread

NODEPS = clean

all: test_suite

ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
-include $(DEP)
endif

test: test_suite
	@echo "Running test suite..."
	@./test_suite

test_suite: $(OBJ)
	@echo "Linking test suite..."
	@$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(OBJ)

%.o: %.cpp
	@echo "Compiling ($<)..."
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

%.d: %.cpp
	@echo "Finding dependencies ($<)..."
	@$(CXX) $(CXXFLAGS) -MM -MT '$(patsubst %.cpp,%.o,$<)' $< -MF $@

clean:
	@echo "Cleaning test..."
	@find . -iname '*.[od]' -exec rm {} \;
	@find ../third_party/Celero/src -iname '*.[od]' -exec rm {} \;

.PHONY: test clean
