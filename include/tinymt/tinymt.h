#ifndef TINYMT_TINYMT_H
#define TINYMT_TINYMT_H

// Based on TinyMT by Mutsuo Saito and Makoto Matsumoto,
//   https://github.com/MersenneTwister-Lab/TinyMT/tree/v1.1.1
// See also RFC 8682.

#include <cstddef>
#include <cstdint>
#include <istream>
#include <limits>
#include <ostream>
#include <type_traits>

namespace tinymt {

namespace detail {

// Status.

template <class UIntType, std::size_t WordSize, std::uintmax_t Mat1,
          std::uintmax_t Mat2, std::uintmax_t TMat>
struct tinymt_engine_status;

template <class UIntType, std::uintmax_t Mat1, std::uintmax_t Mat2,
          std::uintmax_t TMat>
struct tinymt_engine_status<UIntType, 32, Mat1, Mat2, TMat> {
  using result_type = UIntType;
  result_type status[4];
  static constexpr result_type mat1 = Mat1;
  static constexpr result_type mat2 = Mat2;
  static constexpr result_type tmat = TMat;
  static constexpr std::size_t state_size = 4;
  static constexpr std::uintmax_t word_mask = 0xffffffffu;  // 2^32 - 1
  static constexpr std::uintmax_t mask = 0x7fffffffu;       // 2^31 - 1
  struct is_dynamic : std::false_type {};
};

template <class UIntType>
struct tinymt_engine_status<UIntType, 32, 0, 0, 0> {
  using result_type = UIntType;
  result_type status[4];
  result_type mat1;
  result_type mat2;
  result_type tmat;
  static constexpr std::size_t state_size = 4;
  static constexpr std::uintmax_t word_mask = 0xffffffffu;  // 2^32 - 1
  static constexpr std::uintmax_t mask = 0x7fffffffu;       // 2^31 - 1
  struct is_dynamic : std::true_type {};
};

template <class UIntType, std::uintmax_t Mat1, std::uintmax_t Mat2,
          std::uintmax_t TMat>
struct tinymt_engine_status<UIntType, 64, Mat1, Mat2, TMat> {
  using result_type = UIntType;
  result_type status[2];
  static constexpr result_type mat1 = Mat1;
  static constexpr result_type mat2 = Mat2;
  static constexpr result_type tmat = TMat;
  static constexpr std::size_t state_size = 2;
  static constexpr std::uintmax_t word_mask = 0xffffffffffffffffu;  // 2^64 - 1
  static constexpr std::uintmax_t mask = 0x7fffffffffffffffu;       // 2^63 - 1
  struct is_dynamic : std::false_type {};
};

template <class UIntType>
struct tinymt_engine_status<UIntType, 64, 0, 0, 0> {
  using result_type = UIntType;
  result_type status[2];
  result_type mat1;
  result_type mat2;
  result_type tmat;
  static constexpr std::size_t state_size = 2;
  static constexpr std::uintmax_t word_mask = 0xffffffffffffffffu;  // 2^64 - 1
  static constexpr std::uintmax_t mask = 0x7fffffffffffffffu;       // 2^63 - 1
  struct is_dynamic : std::true_type {};
};

// Parameter set.

template <class UIntType>
struct tinymt_engine_parameter_set {
  UIntType mat1;
  UIntType mat2;
  UIntType tmat;
};

// Methods.

template <class UIntType, std::size_t WordSize, std::uintmax_t Mat1,
          std::uintmax_t Mat2, std::uintmax_t TMat>
struct tinymt_engine_methods;

template <class UIntType, std::uintmax_t Mat1, std::uintmax_t Mat2,
          std::uintmax_t TMat>
struct tinymt_engine_methods<UIntType, 32, Mat1, Mat2, TMat> {
  using result_type = UIntType;
  using status_type = tinymt_engine_status<UIntType, 32, Mat1, Mat2, TMat>;
  using parameter_set_type = detail::tinymt_engine_parameter_set<UIntType>;

  static constexpr std::size_t sh0 = 1;
  static constexpr std::size_t sh1 = 10;
  static constexpr std::size_t sh8 = 8;
  static constexpr result_type mask32 = (result_type)status_type::word_mask;
  static constexpr result_type mask = (result_type)status_type::mask;
  static constexpr parameter_set_type default_parameter_set = {
      0x8f7011ee, 0xfc78ff1f, 0x3793fdff};

  static void init(status_type& s, result_type seed) {
    const int MIN_LOOP = 8;
    const int PRE_LOOP = 8;

    s.status[0] = seed & mask32;
    s.status[1] = s.mat1;
    s.status[2] = s.mat2;
    s.status[3] = s.tmat;

    for (int i = 1; i < MIN_LOOP; i++) {
      s.status[i & 3] ^= i + 1812433253 * (s.status[(i - 1) & 3] ^
                                           (s.status[(i - 1) & 3] >> 30));
      s.status[i & 3] &= mask32;
    }

    for (int i = 0; i < PRE_LOOP; i++) {
      next_state(s);
    }
  }

  static void next_state(status_type& s) {
    result_type x = (s.status[0] & mask) ^ s.status[1] ^ s.status[2];
    result_type y = s.status[3];
    x ^= (x << sh0) & mask32;
    y ^= (y >> sh0) ^ x;
    s.status[0] = s.status[1];
    s.status[1] = s.status[2];
    s.status[2] = (x ^ (y << sh1)) & mask32;
    s.status[3] = y;
    if (y & 1) {
      s.status[1] ^= s.mat1;
      s.status[2] ^= s.mat2;
    }
  }

  static result_type temper(const status_type& s) {
    result_type t0 = s.status[3];
    result_type t1 = s.status[0] + (s.status[2] >> sh8);
    t0 ^= t1;
    if (t1 & 1) {
      t0 ^= s.tmat;
    }
    return t0 & mask32;
  }
};

}  // namespace detail

// Main class.

template <class UIntType, size_t WordSize, UIntType Mat1, UIntType Mat2,
          UIntType TMat>
class tinymt_engine {
  static_assert(std::is_unsigned<UIntType>::value,
                "result_type must be an unsigned integral type");
  static_assert(WordSize == 32 || WordSize == 64, "word_size must be 32 or 64");

  using methods =
      detail::tinymt_engine_methods<UIntType, WordSize, Mat1, Mat2, TMat>;
  using status_type =
      detail::tinymt_engine_status<UIntType, WordSize, Mat1, Mat2, TMat>;

  static_assert(std::numeric_limits<UIntType>::max() >= status_type::word_mask,
                "size of result_type must be lager than word_size");
  static_assert(Mat1 <= status_type::word_mask, "Mat1 must be < 2^word_size");
  static_assert(Mat2 <= status_type::word_mask, "Mat2 must be < 2^word_size");
  static_assert(TMat <= status_type::word_mask, "TMat must be < 2^word_size");

  status_type s_;

 public:
  using result_type = UIntType;
  using parameter_set_type = detail::tinymt_engine_parameter_set<UIntType>;
  static constexpr std::size_t word_size = WordSize;
  static constexpr std::size_t state_size = status_type::state_size;
  static constexpr result_type default_seed = 1;
  static constexpr parameter_set_type default_parameter_set =
      methods::default_parameter_set;

  template <class T = result_type,
            typename std::enable_if<std::is_same<T, result_type>::value &&
                                        !status_type::is_dynamic::value,
                                    std::nullptr_t>::type = nullptr>
  explicit tinymt_engine(result_type seed = default_seed) {
    methods::init(s_, seed);
  }

  template <class T = result_type,
            typename std::enable_if<std::is_same<T, result_type>::value &&
                                        status_type::is_dynamic::value,
                                    std::nullptr_t>::type = nullptr>
  explicit tinymt_engine(const parameter_set_type& params,
                         result_type seed = default_seed) {
    s_.mat1 = params.mat1 & status_type::word_mask;
    s_.mat2 = params.mat2 & status_type::word_mask;
    s_.tmat = params.tmat & status_type::word_mask;
    methods::init(s_, seed);
  }

  void seed(result_type value = default_seed) { methods::init(s_, value); }

  void discard(unsigned long long z) {
    for (unsigned long long i = 0; i < z; i++) {
      methods::next_state(s_);
    }
  }

  static constexpr result_type min() { return 0; }

  static constexpr result_type max() { return status_type::max; }

  result_type operator()() {
    methods::next_state(s_);
    return methods::temper(s_);
  }

  friend bool operator==(const tinymt_engine& a, const tinymt_engine& b) {
    if (a.s_.mat1 != b.s_.mat1 || a.s_.mat2 != b.s_.mat2 ||
        a.s_.tmat != b.s_.tmat) {
      return false;
    }
    for (std::size_t i = 0; i < a.state_size; i++) {
      if (a.s_.status[i] != b.s_.status[i]) {
        return false;
      }
    }
    return true;
  }

  friend bool operator!=(const tinymt_engine& a, const tinymt_engine& b) {
    return !(a == b);
  }

  template <class CharT, class Traits>
  friend std::basic_ostream<CharT, Traits>& operator<<(
      std::basic_ostream<CharT, Traits>& os, const tinymt_engine& a) {
    os << a.s_.status[0];
    for (std::size_t i = 1; i < a.state_size; i++) {
      os << ' ' << a.s_.status[i];
    }
    if (status_type::is_dynamic::value) {
      os << ' ' << a.s_.mat1;
      os << ' ' << a.s_.mat2;
      os << ' ' << a.s_.tmat;
    }
    return os;
  }

  template <class CharT, class Traits, class T = result_type,
            typename std::enable_if<std::is_same<T, result_type>::value &&
                                        !status_type::is_dynamic::value,
                                    std::nullptr_t>::type = nullptr>
  friend std::basic_istream<CharT, Traits>& operator>>(
      std::basic_istream<CharT, Traits>& is, tinymt_engine& a) {
    for (std::size_t i = 0; i < a.state_size; i++) {
      is >> a.s_.status[i] >> std::ws;
    }
    return is;
  }

  template <class CharT, class Traits, class T = result_type,
            typename std::enable_if<std::is_same<T, result_type>::value &&
                                        status_type::is_dynamic::value,
                                    std::nullptr_t>::type = nullptr>
  friend std::basic_istream<CharT, Traits>& operator>>(
      std::basic_istream<CharT, Traits>& is, tinymt_engine& a) {
    for (std::size_t i = 0; i < a.state_size; i++) {
      is >> a.s_.status[i] >> std::ws;
    }
    is >> a.s_.mat1 >> std::ws;
    is >> a.s_.mat2 >> std::ws;
    is >> a.s_.tmat >> std::ws;
    return is;
  }
};

using tinymt32_dc = tinymt_engine<uint_fast32_t, 32, 0, 0, 0>;

using tinymt32 =
    tinymt_engine<uint_fast32_t, 32, tinymt32_dc::default_parameter_set.mat1,
                  tinymt32_dc::default_parameter_set.mat2,
                  tinymt32_dc::default_parameter_set.tmat>;

}  // namespace tinymt

#endif  // TINYMT_TINYMT_H
