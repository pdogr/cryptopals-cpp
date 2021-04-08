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

  string s1 = "this is a test\n";
  string s2 = "wokka wokka!!!\n";
  vector<uint8_t> v1 = bytes (s1), v2 = bytes (s2);

  assert (hamming (v1, v2) == 37);

  string b64encoded = readall ("../data/6.txt");
  vector<uint8_t> ct = bytes (b64decode (b64encoded));
  vector<int> probableKeyLengths = kasiki (ct);

  vector<pair<int, long double> > possibleKeys;
  for (int keyGuess : probableKeyLengths)
    {
      if (keyGuess > 50)
        break;
      long double normedDistance = 0;
      int times = 0;
      for (int block = 0; block < ct.size () and block < 16 * keyGuess;
           block += 2 * keyGuess)
        {
          int hammingDistance = 0;
          for (int i = 0; i < keyGuess and block + i + keyGuess < ct.size ();
               ++i)
            {
              hammingDistance += __builtin_popcount (
                  ct[block + i] ^ ct[block + i + keyGuess]);
            }
          ++times;
          normedDistance
              += ((long double)hammingDistance / (long double)keyGuess);
        }
      possibleKeys.push_back (
          { keyGuess, normedDistance / (long double)times });
    }

  sort (all (possibleKeys),
        [&] (pair<int, long double> &a, pair<int, long double> &b) {
          return a.S < b.S;
        });
  possibleKeys.resize (1);
  for (auto [key, metric] : possibleKeys)
    {
      vector<vector<uint8_t> > blocks (key);
      for (int i = 0; i < ct.size (); ++i)
        {
          blocks[i % key].push_back (ct[i]);
        }
      string keyString = "";
      for (int i = 0; i < key; ++i)
        {
          vector<pair<vector<uint8_t>, long double> > scores
              = decodeSingleXor (to_string (blocks[i]), freq, 1);
          keyString += blocks[i].back () ^ scores[0].F.back ();
        }
      cout << key << " " << keyString << "\n";
      string s = to_string (ct) ^ keyString;
      cout << s << "\n";
    }
  return 0;
}