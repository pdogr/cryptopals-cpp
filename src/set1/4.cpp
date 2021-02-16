#include "../../include/utils.h"
#include "../../include/xor.h"

int
main ()
{
  fstream fin ("../data/pg200.txt");
  string s;
  map<uint8_t, long double> freq;
  char c;
  while (fin.get (c))
    {
      freq[tolower (c)] += 1;
    }
  long double total = 0;
  for (auto [c, f] : freq)
    {
      total += f;
    }
  for (auto &[c, f] : freq)
    {
      f /= total;
    }

  fstream file2 ("../data/4.txt");
  vector<pair<vector<uint8_t>, long double> > possible;
  while (file2 >> s)
    {
      vector<pair<vector<uint8_t>, long double> > pbs
          = decodeSingleXor (unhex (s), freq, 4);
      for (auto pb : pbs)
        {
          possible.push_back (pb);
        }
    }
  sort (all (possible),
        [&] (pair<vector<uint8_t>, long double> a,
             pair<vector<uint8_t>, long double> b) { return a.S > b.S; });
  for (int i = 0; i < 5; ++i)
    {
      cout << possible[i] << "\n";
    }

  return 0;
}