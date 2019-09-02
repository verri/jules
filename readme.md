# Jules

[![Join the chat at https://gitter.im/verri-jules/Lobby](https://badges.gitter.im/verri-jules/Lobby.svg)](https://gitter.im/verri-jules/Lobby?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)
[![Build Status](https://travis-ci.org/verri/jules.svg?branch=master)](https://travis-ci.org/verri/jules)
[![codecov](https://codecov.io/gh/verri/jules/branch/master/graph/badge.svg)](https://codecov.io/gh/verri/jules)
[![GitHub license](https://img.shields.io/badge/license-Zlib-blue.svg)](https://raw.githubusercontent.com/verri/jules/master/license.txt)
[![Github Releases](https://img.shields.io/github/release/verri/jules.svg)](https://github.com/verri/jules/releases)
[![AUR](https://img.shields.io/aur/version/jules-git.svg)](https://aur.archlinux.org/packages/jules-git)

**Jules** is a statistical computing library that aims to provide R-like experience in modern C++.

Currently, it focus on a versatile, yet easy-to-use multi-dimensional array implementation
with some common statistical functionalities.

A implementation of data frames and their common operations inspired by [Tidyverse](https://www.tidyverse.org/)
is also intended, but it will probably require many features to come to C++ in the next years.

## Dependencies

 - C++17 compiler
    - Tested on GCC 7.4.0 and Clang 7.0.0
 - [debug_assert](https://github.com/foonathan/debug_assert)
 - [range-v3](https://github.com/ericniebler/range-v3) (soon to be superseded by C++20 Ranges)
 - [CBLAS](http://www.netlib.org/blas/) (optional)
 - [Catch](https://github.com/philsquared/Catch.git) (testing)


## Installation

If you intend to use BLAS optimized functions, make sure CBLAS is installed.

### Recommended: Local Git Submodule

The recommended installation of the library is including it as a submodule of your Git project.

In your project, do
```
$ git submodule add https://github.com/verri/jules.git external/jules
```

### Local copy

Download the [latest release](https://github.com/verri/jules/releases/latest) and unzip it.

### System-wide installation

For Arch Linux users, install the AUR package [jules-git](https://aur.archlinux.org/packages/jules-git)
via [pacaur](https://aur.archlinux.org/packages/pacaur/),
[makepkg](https://wiki.archlinux.org/index.php/Arch_User_Repository#Build_and_install_the_package),
or any other method of your preference.

*System-wide installation is recommended only if you are sure about what you doing.*

Clone the repository.
```
$ git clone https://github.com/verri/jules.git
$ cd jules
```

Install via CMake 3.0+.
```
$ cmake -H. -Bbuild
$ cmake --build build --target install
```

## Usage and Compilation

`Jules` is a header-only library, so you just need to instruct the compiler its location.

### Recommended: CMake 3.0+

If you installed the library as a submodule, include the following line in your `CMakeLists.txt`
```
add_subdirectory(external/jules)
target_link_libraries(my-target PRIVATE jules)
```

If you installed system-wide, use
```
find_package(Cool REQUIRED)
target_link_libraries(my-target PRIVATE ${Cool_LIBRARY})
```

If using BLAS, make sure to link to your target.
```
find_package(BLAS REQUIRED)
target_link_libraries(my-target PRIVATE ${BLAS_LIBRARIES})
```

### Compiler flags

You can directly specify the compiler flags:
```sh
CXXFLAGS="-std=c++17 -isystem/path/to/release/include"
```

If using BLAS, make sure to link it using `-lblas` or `-lcblas`, depending on your system.

## Documentation

Generated with [standardese](https://github.com/foonathan/standardese)
[here](https://verri.github.io/jules/) (not up-to-date, since `standardese` is not
progressing very well).

Soon, we should provide a cheatsheet with a summary of functionalities.  In the meantime,
the examples in the test folder should suffice.

## Acknowledgements

This project is supported by **FAPESP** as part of the project *"High level data
classification based on complex network applied to invariant pattern recognition"*
(2013/25876-6).
