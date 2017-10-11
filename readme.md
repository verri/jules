Jules
=====

[![Join the chat at https://gitter.im/verri-jules/Lobby](https://badges.gitter.im/verri-jules/Lobby.svg)](https://gitter.im/verri-jules/Lobby?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)
[![Build Status](https://travis-ci.org/verri/jules.svg?branch=master)](https://travis-ci.org/verri/jules)
[![codecov](https://codecov.io/gh/verri/jules/branch/master/graph/badge.svg)](https://codecov.io/gh/verri/jules)
[![GitHub license](https://img.shields.io/badge/license-Zlib-blue.svg)](https://raw.githubusercontent.com/verri/jules/master/license.txt)
[![Github Releases](https://img.shields.io/github/release/verri/jules.svg)](https://github.com/verri/jules/releases)
[![AUR](https://img.shields.io/aur/version/jules-git.svg)](https://aur.archlinux.org/packages/jules-git)

**Jules** is a statistical computing library that aims to provide R-like experience in modern C++.

Dependencies
------------

 - C++1z (experimental C++17) compiler
    - Tested on GCC 7.2 and Clang 5.0.0
 - [debug_assert](https://github.com/foonathan/debug_assert)
 - [range-v3](https://github.com/ericniebler/range-v3)
 - [CBLAS](http://www.netlib.org/blas/) (optional)
 - [Catch](https://github.com/philsquared/Catch.git) (testing)

Usage
-----

### Installation

**Jules** is a header-only library. Download the [latest release](https://github.com/verri/jules/releases/latest)
and extract it to `/path/to/release`. The zip file already contains all mandatory dependencies.

For Arch Linux users, install the AUR package [jules-git](https://aur.archlinux.org/packages/jules-git)
via [yaourt](https://archlinux.fr/yaourt-en),
[makepkg](https://wiki.archlinux.org/index.php/Arch_User_Repository#Build_and_install_the_package),
or any other method of your preference.

If you intend to use BLAS optimized functions, make sure CBLAS is installed.

### Compilation

Enable C++1z and include the path where **Jules** is installed. Arch Linux
users do not need the `-isystem` option, since the library is already in the
system path.

```sh
CXXFLAGS="-std=c++17 -isystem/path/to/release/include"
```

If using BLAS, make sure to link it using `-lblas` or `-lcblas`, depending on your system.

Documentation
-------------

Generated with [standardese](https://github.com/foonathan/standardese)
[here](https://verri.github.io/jules/) (not up-to-date).

Acknowledgements
----------------

This project is supported by **FAPESP** as part of the project *"High level data
classification based on complex network applied to invariant pattern recognition"*
(2013/25876-6).
