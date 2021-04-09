#include <bits/stdc++.h>
using namespace std;

class MT19937
{
private:
  uint_fast32_t w, n, m, r;
  uint_fast32_t a, u, d, s, b, t, c, l;
  uint64_t lower_mask, upper_mask, mask;
  int index;
  vector<uint_fast32_t> MT;
  void twist ();

public:
  MT19937 (uint32_t seed);
  uint32_t operator() ();
};
