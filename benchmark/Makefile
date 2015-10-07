SUFFIXES += .d

CELERO_SRC := $(shell find ../third_party/Celero/src -iname '*.cpp')
CELERO_TMP := $(CELERO_SRC:../third_party/Celero/src/%=%)
CELERO_OBJ := $(CELERO_TMP:.cpp=.o)
CELERO_DEP := $(CELERO_TMP:.cpp=.d)

SRC =
OBJ := $(SRC:.cpp=.o) $(CELERO_OBJ) test_suite.o
DEP := $(SRC:.cpp=.d) $(CELERO_DEP) test_suite.d

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

$(CELERO_OBJ): %.o: ../third_party/Celero/src/%.cpp
	@echo "Compiling ($<)..."
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

$(CELERO_DEP): %.d: ../third_party/Celero/src/%.cpp
	@echo "Finding dependencies ($<)..."
	@$(CXX) $(CXXFLAGS) -MM -MT '$(patsubst %.cpp,%.o,$<)' $< -MF $@

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
