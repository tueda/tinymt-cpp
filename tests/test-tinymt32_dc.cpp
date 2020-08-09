#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <tinymt/tinymt.h>

using namespace std;
using namespace tinymt;

// Parameter sets taken from https://github.com/jj1bdx/tinymtdc-longbatch/.

// tinymt32dc.0.1048576.txt.1, i.e., RFC 8682
constexpr tinymt32_dc::param_type id0 = {0x8f7011eeU, 0xfc78ff1fU, 0x3793fdffU};

// tinymt32dc.1.1048576.txt:1
constexpr tinymt32_dc::param_type id1 = {0xda251b45U, 0xfed0ffb5U, 0x9b5cf7ffU};

// tinymt32dc.2.1048576.txt:1
constexpr tinymt32_dc::param_type id2 = {0xf20d1e43U, 0xff90ffe5U, 0xdd372f7fU};

TEST_CASE("range") {
  std::size_t w = tinymt32_dc::word_size;
  CHECK(w == 32);

  CHECK(tinymt32_dc::min() == 0);
  CHECK(tinymt32_dc::max() == 0xffffffffU);
}

TEST_CASE("seed") {
  tinymt32_dc r(id1);
  r.seed(42);

  for (int i = 1; i <= 9; i++) {
    r();
  }

  CHECK(r() == 2165469340);
}

TEST_CASE("discard") {
  tinymt32_dc r1(id1);
  tinymt32_dc r2(id1);

  for (int i = 0; i < 10; i++) {
    r1();
  }

  r2.discard(10);

  CHECK(r1() == r2());
}

TEST_CASE("equals1") {
  tinymt32_dc r1(id1);
  tinymt32_dc r2(id1);

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

TEST_CASE("equals2") {
  tinymt32_dc r1(id1);
  tinymt32_dc r2(id2);

  CHECK(r1 != r2);
}

TEST_CASE("serialize1") {
  stringstream buf;

  tinymt32_dc r1(id1);
  r1();
  r1();
  buf << r1;
  tinymt32::result_type x1 = r1();

  tinymt32_dc r2(id2);
  buf >> r2;
  tinymt32::result_type x2 = r2();

  CHECK(x1 == x2);
}

TEST_CASE("serialize2") {
  stringstream buf;

  tinymt32_dc r1(id0);
  r1();
  r1();
  buf << r1;
  tinymt32_dc::result_type x1 = r1();

  tinymt32 r2;
  buf >> r2;
  tinymt32::result_type x2 = r2();

  CHECK(x1 == x2);
}
