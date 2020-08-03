// Validation with Figure 2 in RFC 8682.

#include <tinymt/tinymt.h>

#include <iomanip>
#include <iostream>

using namespace std;
using namespace tinymt;

int main() {
  tinymt32 mt;

  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 5; j++) {
      cout << setw(10) << mt();
      if (j < 4) {
        cout << " ";
      } else {
        cout << endl;
      }
    }
  }

  return 0;
}
