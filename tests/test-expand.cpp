// Instantiate template classes; expand them as much as possible.

#include <tinymt/tinymt.h>

template class tinymt::tinymt_engine<
    std::uint_fast32_t, 32, tinymt::detail::tinymt32_default_param_mat1,
    tinymt::detail::tinymt32_default_param_mat2,
    tinymt::detail::tinymt32_default_param_tmat>;

template class tinymt::tinymt_engine<std::uint_fast32_t, 32, 0, 0, 0>;

int main() { return 0; }
