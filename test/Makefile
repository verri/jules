CXX = clang++
CXXFLAGS = -std=c++14 -Wall -Wextra -I../include -I../third_party/Catch/include

all: column

column: dataframe/column.cpp
	$(CXX) $(CXXFLAGS) -o column $<
