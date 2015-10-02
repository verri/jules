#include "track.hpp"
#include "array/array.hpp"

#include <iostream>

int main()
{
    Memory::Reset();

    using jules::scalar;
    using jules::vector;
    using jules::matrix;
    using jules::ndarray;

    scalar<double> x = 10;
    (void) x;

    std::array<double, 4> values{{1.0, 2.0, 3.0, 4.0}};

    vector<double> y;          // default empty
    vector<double> z(20);      // 20 default elements
    vector<double> a(1.0, 30); // 30 elements with value 1.0
    vector<double> b(values.begin(), values.size());
    vector<double> c = jules::as_vector(values);

    matrix<double> M(1.0, 10, 10);
    M[0][0] = 2.0;

    std::cout << Memory::Size() << " bytes in " << Memory::Count() << " allocations ("
              << (static_cast<double>(Memory::Size()) / Memory::Count())
              << " bytes/alloc)" << std::endl;

    Memory::Finalize();

    return 0;
}
