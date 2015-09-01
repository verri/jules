SUFFIXES += .d

SRC = dataframe/column.cpp dataframe/dataframe.cpp
OBJ := $(SRC:.cpp=.o) test_suite.o
DEP := $(SRC:.cpp=.d) test_suite.d

CXXFLAGS += -std=c++14 -Wall -Wextra -I../include -isystem../third_party/Catch/include

NODEPS = clean

all: test_suite

ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
-include $(DEP)
endif

test: test_suite
	./test_suite

test_suite: $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.d: %.cpp
	$(CXX) $(CXXFLAGS) -MM -MT '$(patsubst %.cpp,%.o,$<)' $< -MF $@

clean:
	rm -f dataframe/*.o dataframe/*.d test_suite

.PHONY: test clean
