#include "../include/random.h"
void
MT19937::twist ()
{
  for (int i = 0; i < n; ++i)
    {
      uint_fast32_t x = (MT[i] & upper_mask) + (MT[(i + 1) % n] & lower_mask);
      uint_fast32_t xA = x >> 1;
      if (x & 0x1)
        {
          xA ^= a;
        }
      MT[i] = MT[(i + m) % n] ^ xA;
    }
  index = 0;
}

MT19937::MT19937 (uint32_t seed)
{
  w = 32, n = 624, m = 397, r = 31;
  a = 0x9908B0DF;
  u = 11, d = 0xFFFFFFFF;
  s = 7, b = 0x9D2C5680;
  t = 15, c = 0xEFC60000;
  l = 18;
  index = n;
  MT = vector<uint_fast32_t> (n);
  mask = (1ull << w) - 1;
  upper_mask = (mask << r) & mask;
  lower_mask = (~upper_mask) & mask;

  MT[0] = seed;
  uint_fast32_t f (1812433253);
  for (int i = 1; i < n; ++i)
    {
      MT[i]
          = ((uint64_t) (f * (MT[i - 1] ^ (MT[i - 1] >> (w - 2))) + i)) & mask;
    }
}
uint32_t
MT19937::operator() ()
{
  if (index >= n)
    {
      twist ();
    }
  uint_fast32_t y = MT[index];
  y = y ^ ((y >> u) & d);
  y = y ^ ((y << s) & b);
  y = y ^ ((y << t) & c);
  y = y ^ (y >> l);
  index += 1;
  return (y & mask);
}
