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

  string hexString
      = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
  cout << decodeSingleXor (unhex (hexString), freq, 1) << "\n";

  return 0;
}