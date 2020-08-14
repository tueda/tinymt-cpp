tinymt-cpp
==========

[![CI](https://github.com/tueda/tinymt-cpp/workflows/CI/badge.svg?branch=master)](https://github.com/tueda/tinymt-cpp/actions?query=branch%3Amaster)
[![Documentation Status](https://readthedocs.org/projects/tinymt-cpp/badge/?version=latest)](https://tinymt-cpp.readthedocs.io/en/latest/?badge=latest)
[![Codecov](https://codecov.io/gh/tueda/tinymt-cpp/branch/master/graph/badge.svg)](https://codecov.io/gh/tueda/tinymt-cpp/branch/master)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/tueda/tinymt-cpp.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/tueda/tinymt-cpp/context:cpp)

A C++11 header-only implementation of the
[TinyMT](http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/TINYMT/index.html)
pseudo-random number generator.


tinymt-cpp is a one-header-file library written in C++11, which implements
a pseudo-random number generator engine based on the TinyMT algorithms. It can
be used in the same way as random number generators in the standard library and
combined with random number distributions.


Example
-------

```cpp
#include <iostream>
#include <tinymt/tinymt.h>

int main() {
  tinymt::tinymt32 rng;

  std::cout << rng() << std::endl;
  std::cout << rng() << std::endl;

  return 0;
}
```


Development
-----------

```bash
# Prerequisites (including optional ones)
brew install cmake doxygen gcc lcov llvm pre-commit

# Pre-commit hooks.
pre-commit install
pre-commit install --hook-type commit-msg

# Linters.
pre-commit run --all-files

# Tests.
cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build/debug --target check

# Documents.
cmake -S . -B build/docs -DBUILD_TESTING=OFF
cmake --build build/docs --target doc

# Code coverage.
cmake -S . -B build/coverage -DCMAKE_BUILD_TYPE=Debug -DUSE_CODE_COVERAGE=ON
lcov -z -d build/coverage
cmake --build build/coverage --target check
lcov -c -d build/coverage -o build/coverage/coverage.info
genhtml -o build/coverage/html build/coverage/coverage.info

# Compiler sanitizers.
cmake -S . -B build/sanitizer -DCMAKE_BUILD_TYPE=Debug -DUSE_SANITIZER=ON
cmake --build build/sanitizer --target check

# Clang-Tidy.
cmake -S . -B build/clang-tidy -DCMAKE_BUILD_TYPE=Debug -DUSE_CLANG_TIDY=ON
cmake --build build/clang-tidy --target check

# Benchmarking.
cmake -S . -B build/release -DCMAKE_BUILD_TYPE=Release -DBUILD_BENCHMARKING=ON
cmake --build build/release --target bench
```


License
-------

[BSD-3-Clause](https://github.com/tueda/tinymt-cpp/blob/master/LICENSE)


References
----------

- Mutsuo Saito, Makoto Matsumoto, Vincent Roca and Emmanuel Baccelli,  
  TinyMT32 Pseudorandom Number Generator (PRNG),  
  [RFC 8682 (2020) 1-12](https://www.rfc-editor.org/rfc/rfc8682.html).

- 斎藤睦夫, 松本眞,  
  高速並列計算用の状態空間の小さな高品質疑似乱数生成器,  
  [研究報告ハイパフォーマンスコンピューティング（HPC） 2011-HPC-131(3) (2011) 1-6](http://id.nii.ac.jp/1001/00077610/).

- [Tiny Mersenne Twister (TinyMT): A small-sized variant of Mersenne Twister](http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/TINYMT/index.html).

- [MersenneTwister-Lab/TinyMT](https://github.com/MersenneTwister-Lab/TinyMT).
