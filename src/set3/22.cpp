#include "../../include/disp.h"
#include "../../include/mt19937.h"
#include "../../include/utils.h"
#include <random>

int
main ()
{
  uint32_t cur_time = time (0);
  uint32_t wait1 = random_long (10, 1000);
  MT19937 m (cur_time + wait1);
  cout << cur_time + wait1 << "\n";
  uint32_t val = m ();
  uint32_t wait2 = random_long (10, 1000);
  uint32_t finish_time = time (0) + wait1 + wait2;
  for (int i = 0; i < wait1 + wait2 + 100; ++i)
    {

      MT19937 _m (finish_time - i);
      if (_m () == val)
        {
          cout << "Found seed: " << finish_time - i << "\n";
          break;
        }
    }

  return 0;
}
