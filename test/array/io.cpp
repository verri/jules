#include "jules/array/array.hpp"

#include <catch.hpp>

#include <sstream>
#include <string>

using namespace std::string_literals;

TEST_CASE("Array formatted output", "[array]")
{
  auto values = jules::vector<int>{1, 2, 3, 4};
  auto mat = jules::matrix<int>{{1, 2}, {3, 4}};

  // Standard string
  auto ss = std::stringstream{};
  auto jass = jules::array_ostream(ss);

  jass << values;
  CHECK(ss.str() == "{1\t2\t3\t4}");

  ss.str("");
  jass << jules::dimensions_format(0, "["s, " "s, "]"s) << values;
  CHECK(ss.str() == "[1 2 3 4]");

  ss.str("");
  jass << jules::comma_separated << values;
  CHECK(ss.str() == "1,2,3,4");

  ss.str("");
  jass << jules::space_separated << mat;
  CHECK(ss.str() == "1 2\n3 4");

  // Wide string
  auto wss = std::wstringstream{};
  auto jawss = jules::array_ostream(wss);

  jawss << values;
  CHECK(wss.str() == L"{1\t2\t3\t4}");

  wss.str(L"");
  jawss << jules::dimensions_format(0, L"["s, L" "s, L"]"s) << values;
  CHECK(wss.str() == L"[1 2 3 4]");

  wss.str(L"");
  jawss << jules::comma_separated << values;
  CHECK(wss.str() == L"1,2,3,4");

  wss.str(L"");
  jawss << jules::space_separated << mat;
  CHECK(wss.str() == L"1 2\n3 4");
}
