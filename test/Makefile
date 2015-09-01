SRC = dataframe/column.cpp dataframe/dataframe.cpp
OBJ := $(SRC:.cpp=.o)

CXXFLAGS += -std=c++14 -Wall -Wextra -I../include -isystem../third_party/Catch/include

all: test

test: test_suite

test_suite: test_suite.cpp $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
