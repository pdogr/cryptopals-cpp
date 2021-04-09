#include "../../include/aes.h"
#include "../../include/disp.h"
#include "../../include/utils.h"
#include <fcntl.h>
string prepend = "comment1=cooking%20MCs;userdata=";

string append = ";comment2=%20like%20a%20pound%20of%20bacon";
uint8_t key[16];
uint64_t nonce (0ull);

AES *a;
vector<uint8_t>
quote_encrypt (string s)
{
  string safe_s;
  nonce += 1;
  for (int i = 0; i < s.size (); ++i)
    {
      if (s[i] != ';' and s[i] != '=')
        {
          safe_s += s[i];
        }
    }
  string total = prepend + safe_s + append;
  auto b = a->encryptCTR (bytes (total), nonce);
  return b;
}
bool
is_admin (vector<uint8_t> v)
{
  string pt = to_string (a->decryptCTR (v, nonce));
  string tgt = ";admin=true;";
  cout << pt << "\n";
  for (int i = 0; i + tgt.size () - 1 < pt.size (); ++i)
    {
      bool match = true;
      for (int j = 0; j < tgt.size (); ++j)
        {
          if (tgt[j] != pt[i + j])
            match = false;
          if (!match)
            break;
        }
      if (match)
        return 1;
    }
  return 0;
}
int
main ()
{
  int fd = open ("/dev/urandom", O_RDONLY);
  read (fd, key, 16);
  a = new AES (key, 128);
  string block1 = "aaaaaaaaaaaaaaaa";
  string block2 = "aaaaa;admin=true";

  string payload = block1 + block1;
  auto b = quote_encrypt (payload);
  for (int i = 32; i < 48; ++i)
    {
      uint8_t x = block1[i - 32] ^ block2[i - 32];
      b[i + 16] = (b[i + 16] ^ x);
    }
  cout << is_admin (b) << "\n";
}
