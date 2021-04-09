#include "../include/mt19937.h"

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
MT19937::MT19937 (vector<uint32_t> vals)
{
  w = 32, n = 624, m = 397, r = 31;
  a = 0x9908B0DF;
  u = 11, d = 0xFFFFFFFF;
  s = 7, b = 0x9D2C5680;
  t = 15, c = 0xEFC60000;
  l = 18;
  index = n;
  mask = (1ull << w) - 1;
  upper_mask = (mask << r) & mask;
  lower_mask = (~upper_mask) & mask;

  uint_fast32_t f (1812433253);
  MT = vector<uint_fast32_t> (vals.begin (), vals.end ());
}
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
uint32_t
MT19937::operator() ()
{
  if (index >= n)
    {
      twist ();
    }
  uint_fast32_t y = MT[index];
  index += 1;
  // temper
  y = y ^ ((y >> u) & d);
  y = y ^ ((y << s) & b);
  y = y ^ ((y << t) & c);
  y = y ^ (y >> l);
  return (y & mask);
}

vector<uint8_t>
tfm_mt19937 (vector<uint8_t> v, uint32_t key)
{
  MT19937 mt (key);
  for (int i = 0; i < v.size (); i += 4)
    {
      uint32_t x = mt ();
      int id = i;
      while (id < i + 4 and id < v.size ())
        {
          v[id++] ^= (x & 0xff);
          x >>= 8;
        }
    }
  return v;
}
