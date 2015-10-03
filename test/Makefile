SUFFIXES += .d

SRC = array/array_detail.cpp array/array.cpp \
	dataframe/column.cpp dataframe/dataframe.cpp dataframe/column_detail.cpp \
	range/range.cpp tutorial/naive.cpp
OBJ := $(SRC:.cpp=.o) test_suite.o
DEP := $(SRC:.cpp=.d) test_suite.d

CXXFLAGS += -std=c++14 -g -march=native -O3 -Wall -Wextra -I../include -isystem../third_party/Catch/include

NODEPS = clean

all: test_suite

ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
-include $(DEP)
endif

test: test_suite
	@echo "Running test suite..."
	@valgrind --leak-check=full ./test_suite -d yes

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

.PHONY: test clean
