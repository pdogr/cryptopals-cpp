#include "../../include/disp.h"
#include "../../include/random.h"
#include "../../include/utils.h"
#include <random>

int
main ()
{

  uint32_t seed = time (0);
  MT19937 m (seed);
  mt19937 mt_rand (seed);
  cout << "Testing mt19337 with default implementation\n";
  for (int i = 0; i < 1e6; ++i)
    {
      assert (mt_rand () == m ());
    }
  cout << "All values same\n";
  return 0;
}
