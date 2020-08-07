// Instantiate template classes; expand them as much as possible.

#include <tinymt/tinymt.h>

template class tinymt::tinymt_engine<
    std::uint_fast32_t, 32, tinymt::tinymt32_dc::default_parameter_set.mat1,
    tinymt::tinymt32_dc::default_parameter_set.mat2,
    tinymt::tinymt32_dc::default_parameter_set.tmat>;

template class tinymt::tinymt_engine<std::uint_fast32_t, 32, 0, 0, 0>;

int main() { return 0; }
