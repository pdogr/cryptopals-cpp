#include "../include/xor.h"
#include "../include/utils.h"
vector<uint8_t>
operator^ (const vector<uint8_t> &ca, const vector<uint8_t> &cb)
{
  vector<uint8_t> big = ca, smol = cb;
  if (ca.size () < cb.size ())
    {
      swap (big, smol);
    }
  vector<uint8_t> res (big.size ());
  for (int i = 0; i < big.size (); ++i)
    {
      res[i] = (big[i] ^ smol[i % smol.size ()]);
    }
  return res;
}

string
operator^ (const string &a, const string &b)
{
  return to_string (bytes (a) ^ bytes (b));
}

vector<pair<vector<uint8_t>, long double> >
decodeSingleXor (string S, map<uint8_t, long double> freq, int topk = 1)
{
  vector<uint8_t> bytesString = bytes (S);
  gp_hash_table<uint8_t, long double> klMap;

  for (int i = 0; i < (1 << 8); ++i)
    {
      vector<uint8_t> xored (bytesString.size ());
      for (int j = 0; j < xored.size (); ++j)
        {
          xored[j] = bytesString[j] ^ i;
        }
      klMap[i] = score (freq, xored);
    }
  vector<pair<uint8_t, long double> > vals (all (klMap));
  sort (all (vals), [&] (pair<uint8_t, long double> a,
                         pair<uint8_t, long double> b) { return a.S < b.S; });
  vector<pair<vector<uint8_t>, long double> > ret;
  while (vals.size () and topk)
    {
      vector<uint8_t> bytesCopy (all (bytesString));
      transform (all (bytesCopy), bytesCopy.begin (),
                 [&] (uint8_t c) { return c ^ vals.back ().F; });
      ret.push_back ({ bytesCopy, vals.back ().S });
      vals.pop_back ();
      --topk;
    }
  return ret;
}

long double
score (map<uint8_t, long double> original_freq, vector<uint8_t> v)
{
  long double s = 0;
  for (auto c : v)
    {
      s += original_freq[tolower (c)];
    }
  return s / v.size ();
}

int
hamming (vector<uint8_t> a, vector<uint8_t> b)
{
  int hammingDistance = 0;
  int sz = min (a.size (), b.size ());
  for (int i = 0; i < sz; ++i)
    {
      hammingDistance += __builtin_popcount (a[i] ^ b[i]);
    }
  return hammingDistance;
}

vector<int>
kasiki (vector<uint8_t> ct)
{
  set<int> possibleKeyLengths;
  for (int len = 3; len <= 6; ++len)
    {
      map<string, vector<int> > occ;
      for (int i = 0; i + len - 1 < ct.size (); ++i)
        {
          string aux;
          for (int j = 0; j < len; ++j)
            {
              aux += ct[i + j];
            }
          occ[aux].push_back (i);
        }
      for (auto &[str, v] : occ)
        {
          if (v.size () < 3)
            continue;
          int g = v[1] - v[0];
          for (int j = 2; j < v.size (); ++j)
            {
              g = __gcd (v[j] - v[j - 1], g);
            }
          possibleKeyLengths.insert (g);
        }
    }
  set<int> divs;
  for (auto len : possibleKeyLengths)
    {
      for (int i = 1; i * i <= len; ++i)
        {
          if (len % i == 0)
            {
              divs.insert (i);
              divs.insert (len / i);
            }
        }
    }
  return vector<int> (all (divs));
}