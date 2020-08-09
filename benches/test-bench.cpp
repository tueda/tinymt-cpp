#define ANKERL_NANOBENCH_IMPLEMENT
#include <nanobench.h>
#include <tinymt/tinymt.h>

#include <chrono>

using namespace std;
using namespace ankerl::nanobench;
using namespace tinymt;

#define bench_init(bench, rng, name) \
  do {                               \
    bench.run(name, [&]() {          \
      auto r = rng;                  \
      doNotOptimizeAway(r);          \
    });                              \
  } while (false)

template <typename Rng>
void bench_gen_raw(Bench& bench, const Rng& rng, const char* name) {
  Rng r(rng);
  bench.run(name, [&]() {
    auto x = r();
    doNotOptimizeAway(x);
  });
}

template <typename Rng>
void bench_gen_double(Bench& bench, const Rng& rng, const char* name) {
  Rng r(rng);
  auto dist = uniform_real_distribution<>();
  bench.run(name, [&]() {
    auto x = dist(r);
    doNotOptimizeAway(x);
  });
}

template <typename Rng>
void bench_gen_gauss(Bench& bench, const Rng& rng, const char* name) {
  Rng r(rng);
  auto dist = normal_distribution<>();
  bench.run(name, [&]() {
    auto x = dist(r);
    doNotOptimizeAway(x);
  });
}

#define bench_set(b, bench_func)                                           \
  do {                                                                     \
    bench_func(b, tinymt32(), "tinymt::tinymt32");                         \
    bench_func(b,                                                          \
               tinymt32_dc({tinymt::detail::tinymt32_default_param_mat1,   \
                            tinymt::detail::tinymt32_default_param_mat2,   \
                            tinymt::detail::tinymt32_default_param_tmat}), \
               "tinymt::tinymt32_dc");                                     \
    bench_func(b, mt19937(), "std::mt19937");                              \
    bench_func(b, mt19937_64(), "std::mt19937_64");                        \
                                                                           \
  } while (false)

int main() {
  Bench b;
  b.warmup(1000).relative(true);

  b.title("init");
  bench_set(b, bench_init);

  b.title("gen_raw");
  bench_set(b, bench_gen_raw);

  b.title("gen_double");
  bench_set(b, bench_gen_double);

  b.title("gen_gauss");
  bench_set(b, bench_gen_gauss);

  return 0;
}
