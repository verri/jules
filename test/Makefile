CXX = clang++
CXXFLAGS = -std=c++14 -Wall -Wextra -I../include -I../third_party/Catch/include

all: column.test dataframe.test

column.test: dataframe/column.cpp
	$(CXX) $(CXXFLAGS) -o column.test $<

dataframe.test: dataframe/dataframe.cpp
	$(CXX) $(CXXFLAGS) -o dataframe.test $<
