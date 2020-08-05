// Validation with Figure 2 in RFC 8682.

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <tinymt/tinymt.h>

#include <iomanip>
#include <iostream>

using namespace std;
using namespace tinymt;

TEST_CASE("RFC 8682: TinyMT32") {
  const tinymt32::result_type expected[] = {
      2545341989, 981918433,  3715302833, 2387538352, 3591001365, 3820442102,
      2114400566, 2196103051, 2783359912, 764534509,  643179475,  1822416315,
      881558334,  4207026366, 3690273640, 3240535687, 2921447122, 3984931427,
      4092394160, 44209675,   2188315343, 2908663843, 1834519336, 3774670961,
      3019990707, 4065554902, 1239765502, 4035716197, 3412127188, 552822483,
      161364450,  353727785,  140085994,  149132008,  2547770827, 4064042525,
      4078297538, 2057335507, 622384752,  2041665899, 2193913817, 1080849512,
      33160901,   662956935,  642999063,  3384709977, 1723175122, 3866752252,
      521822317,  2292524454,
  };

  tinymt32 mt;

  // Generate 50 pseudorandom numbers.

  tinymt32::result_type x[50];

  for (int i = 0; i < 50; i++) {
    x[i] = mt();
  }

  // Print them.

  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 5; j++) {
      cout << setw(10) << x[i * 5 + j];
      if (j < 4) {
        cout << " ";
      } else {
        cout << endl;
      }
    }
  }

  // Check the generated pseudorandom numbers.

  for (int i = 0; i < 50; i++) {
    CHECK_MESSAGE(x[i] == expected[i], "x[" << i << "]");
  }
}
