tinymt-cpp
==========

[![CI](https://github.com/tueda/tinymt-cpp/workflows/CI/badge.svg?branch=master)](https://github.com/tueda/tinymt-cpp/actions?query=branch%3Amaster)
[![codecov](https://codecov.io/gh/tueda/tinymt-cpp/branch/master/graph/badge.svg?token=RXT09V4NIP)](https://codecov.io/gh/tueda/tinymt-cpp/branch/master)

A C++11 header-only implementation of the
[TinyMT](http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/TINYMT/index.html)
pseudo random number generator.


Development
-----------

```bash
# Prerequisites.
brew install clang-format cmake doxygen gcc lcov pre-commit

# Pre-commit hooks.
pre-commit install
pre-commit install --hook-type commit-msg

# Linters.
pre-commit run --all-files

# Tests.
cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build/debug --target check
cmake --build build/debug --target doc

# Code coverage.
cmake -S . -B build/coverage -DCMAKE_BUILD_TYPE=Debug -DUSE_CODE_COVERAGE=YES
lcov -z -d build/coverage
cmake --build build/coverage --target check
lcov -c -d build/coverage -o build/coverage/coverage.info
genhtml -o build/coverage/html build/coverage/coverage.info

# Compiler sanitizers.
cmake -S . -B build/sanitizer -DCMAKE_BUILD_TYPE=Debug -DUSE_SANITIZER=YES
cmake --build build/sanitizer --target check

# Benchmarking.
cmake -S . -B build/release -DCMAKE_BUILD_TYPE=Release
cmake --build build/release --target bench
```


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
