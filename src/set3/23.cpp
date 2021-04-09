#include "../../include/disp.h"
#include "../../include/mt19937.h"
#include "../../include/utils.h"
#include <fcntl.h>
uint32_t
invert_rs (uint32_t y, uint32_t shift)
{
  uint32_t inv = y;
  for (int i = shift; i < 32; i += shift)
    {
      inv ^= (y >> i);
    }
  return inv;
}
uint32_t
invert_lsa32 (uint32_t y, uint32_t shift, uint32_t mask)
{
  uint32_t inv = y;
  uint32_t m = (1ull << shift) - 1;
  for (int i = 0; i < 32; i += shift)
    {
      inv ^= ((inv & m) << shift) & mask;
      m <<= shift;
    }
  return inv;
}
uint32_t
untemper32 (uint32_t y)
{
  uint32_t u, d, s, b, t, c, l;
  u = 11, d = 0xFFFFFFFF;
  s = 7, b = 0x9D2C5680;
  t = 15, c = 0xEFC60000;
  l = 18;
  y = invert_rs (y, l);
  y = invert_lsa32 (y, t, c);
  y = invert_lsa32 (y, s, b);
  y = invert_rs (y, u);
  return y;
}
int
main ()
{

  uint32_t seed, random_offset;
  int fd = open ("/dev/urandom", O_RDONLY);
  read (fd, &seed, sizeof (uint32_t));
  read (fd, &random_offset, sizeof (uint32_t));
  random_offset %= 0x100000;
  MT19937 mt (seed);
  for (int i = 0; i < random_offset; ++i)
    {
      mt ();
    }
  vector<uint32_t> v (624);
  for (int i = 0; i < 624; ++i)
    {
      v[i] = mt ();
    }
  vector<uint32_t> state (624);
  for (int i = 0; i < 624; ++i)
    {
      state[i] = untemper32 (v[i]);
    }
  MT19937 mt_clone (state);
  for (int i = 0; i < 0x100000; ++i)
    {
      assert (mt () == mt_clone ());
    }
  cout << "MT19937 cloned successfully\n";
}
