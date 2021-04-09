#include "../../include/disp.h"
#include "../../include/mt19937.h"
#include "../../include/utils.h"
#include <fcntl.h>
vector<uint8_t> known_pt;

vector<uint8_t>
encrypt_random_prefix (uint32_t seed)
{
  ssize_t random_prefix_len = random_long (1, 0xffff);
  uint8_t *tmp = new uint8_t[random_prefix_len];
  vector<uint8_t> pt (random_prefix_len + known_pt.size ());
  int fd = open ("/dev/urandom", O_RDONLY);
  read (fd, tmp, random_prefix_len * sizeof (uint8_t));
  for (int i = 0; i < random_prefix_len; ++i)
    pt[i] = tmp[i];
  delete[] tmp;
  for (int i = 0; i < known_pt.size (); ++i)
    {
      pt[i + random_prefix_len] = known_pt[i];
    }
  return tfm_mt19937 (pt, seed);
}
int
main ()
{

  uint32_t seed;
  known_pt = bytes ("AAAAAAAAAAAAAAAAAA");
  int fd = open ("/dev/urandom", O_RDONLY);
  read (fd, &seed, sizeof (uint32_t));
  auto b = tfm_mt19937 (known_pt, seed);
  auto b2 = tfm_mt19937 (b, seed);
  assert (b2 == known_pt);
  seed &= 0xffff; // 16 bit seed
  cout << "Seed used: " << seed << "\n";
  vector<uint8_t> ct = encrypt_random_prefix (seed);
  // bruteforce ?
  for (int i = 0; i < 0xffff; ++i)
    {
      uint32_t guess = i & 0xffffffff;
      auto b = tfm_mt19937 (ct, guess);
      bool eq = true;
      for (int j = 0; j < known_pt.size (); ++j)
        {
          if (known_pt[j] != b[b.size () - known_pt.size () + j])
            {
              eq = false;
              break;
            }
        }
      if (eq)
        {
          cout << "Found match: " << guess << "\n";
        }
    }
}
