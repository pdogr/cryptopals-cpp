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
  MT19937 (vector<uint32_t> vals);
  uint32_t operator() ();
};

vector<uint8_t> tfm_mt19937 (vector<uint8_t> ks, uint32_t key);
