#include "../../include/aes.h"
#include "../../include/disp.h"
#include "../../include/utils.h"
#include "../../include/xor.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
uint8_t key[16];
uint64_t nonce (0ull);
vector<uint8_t> fixed_iv;
AES *a;
vector<vector<uint8_t> >
encrypt_multiple (vector<string> strs)
{
  vector<vector<uint8_t> > v;
  for (auto &s : strs)
    {
      v.push_back (a->encryptCTR (bytes (s), nonce));
    }
  return v;
}
int
main ()
{
  fstream fin ("../data/pg200.txt");
  string s;
  map<uint8_t, long double> freq;
  char c;
  while (fin.get (c))
    {
      if (c >= '0' and c <= '9')
        continue;
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
  int fd = open ("/dev/urandom", O_RDONLY);
  read (fd, key, 16);
  a = new AES (key, 128);
  vector<string> secrets = readline ("../data/19.txt");
  for (auto &s : secrets)
    {
      s = b64decode (s);
    }
  a = new AES (key, 128);
  auto vs = encrypt_multiple (secrets);
  int mx = 0;
  for (auto v : vs)
    {
      mx = max (mx, int (v.size ()));
    }
  cout << mx << "\n";
  string key_string;
  int kl = 0;
  while (1)
    {
      int nkl = mx;
      for (auto v : vs)
        {
          if (v.size () <= kl)
            continue;
          if (nkl > v.size ())
            nkl = v.size ();
        }
      if (kl == mx)
        break;
      int ckl = nkl - kl;
      cout << nkl << " " << kl << "\n";
      string c;
      for (auto v : vs)
        {
          for (int i = kl; i < nkl and i < v.size (); ++i)
            c += v[i];
        }
      vector<vector<uint8_t> > blocks (ckl);
      for (int i = 0; i < c.size (); ++i)
        {
          blocks[i % ckl].push_back (c[i]);
        }
      for (int i = 0; i < ckl; ++i)
        {
          auto scores = decodeSingleXor (to_string (blocks[i]), freq, 1);
          key_string += blocks[i].back () ^ scores[0].F.back ();
        }
      kl = nkl;
    }
  for (int id = 0; id < vs.size (); ++id)
    {
      auto v = vs[id];
      cout << secrets[id] << "\n";
      for (int i = 0; i < v.size (); ++i)
        {
          cout << char (v[i] ^ key_string[i]);
        }
      cout << "\n";
    }
}
