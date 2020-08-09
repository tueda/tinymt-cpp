#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <tinymt/tinymt.h>

using namespace std;
using namespace tinymt;

TEST_CASE("range") {
  std::size_t w = tinymt32::word_size;
  CHECK(w == 32);

  CHECK(tinymt32::min() == 0);
  CHECK(tinymt32::max() == 0xffffffffU);
}

TEST_CASE("seed") {
  tinymt32 r;
  r.seed(42);

  for (int i = 1; i <= 9; i++) {
    r();
  }

  CHECK(r() == 392941632);
}

TEST_CASE("discard") {
  tinymt32 r1;
  tinymt32 r2;

  for (int i = 0; i < 10; i++) {
    r1();
  }

  r2.discard(10);

  CHECK(r1() == r2());
}

TEST_CASE("equals") {
  tinymt32 r1;
  tinymt32 r2;

  CHECK(r1 == r2);

  r1();

  CHECK(r1 != r2);

  r2();

  CHECK(r1 == r2);

  r2();

  CHECK(r1 != r2);

  r1();

  CHECK(r1 == r2);
}

TEST_CASE("serialize") {
  stringstream buf;

  tinymt32 r1;
  r1();
  r1();
  buf << r1;
  tinymt32::result_type x1 = r1();

  tinymt32 r2;
  buf >> r2;
  tinymt32::result_type x2 = r2();

  CHECK(x1 == x2);
}
