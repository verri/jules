SUFFIXES += .d

SRC = $(shell find . -name '*.cpp')
OBJ := $(SRC:.cpp=.o)
DEP := $(SRC:.cpp=.d)

BLASLIBS := $(shell ../util/find_blas --libs)

CXXFLAGS += -DJULES_DEBUG_THROWS -std=c++1z -Wall -Wextra -pedantic -I../include
LDFLAGS += $(BLASLIBS)

NODEPS = clean

all: test_suite

ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
-include $(DEP)
endif

test_suite: $(OBJ)
	@echo "Linking test suite..."
	@$(CXX) $(CXXFLAGS) -o $@ $(OBJ) $(LDFLAGS)

%.o: %.cpp
	@echo "Compiling ($<)..."
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

%.d: %.cpp
	@echo "Finding dependencies ($<)..."
	@$(CXX) $(CXXFLAGS) -MM -MT '$(patsubst %.cpp,%.o,$<)' $< -MF $@

clean:
	@echo "Cleaning test..."
	@find . -name '*.[od]' -exec rm {} \;

.PHONY: test clean
