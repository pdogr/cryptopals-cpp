#include "../../include/aes.h"
#include "../../include/disp.h"
#include "../../include/utils.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
uint8_t key[16], IV[16];
vector<uint8_t> fixed_iv;
AES *a;
string secret;
pair<vector<uint8_t>, vector<uint8_t> >
encrypt_secret ()
{
  return { a->encryptCBC (bytes (secret), fixed_iv), fixed_iv };
}
bool
check_valid_padding (vector<uint8_t> v)
{
  auto e = a->decryptCBC (v, fixed_iv);
  return is_valid_PKCS7 (e);
}
int
main ()
{
  int fd = open ("/dev/urandom", O_RDONLY);
  read (fd, key, 16);
  a = new AES (key, 128);
  read (fd, IV, 16);
  fixed_iv = vector<uint8_t> (IV, IV + 16);
  vector<string> secrets = readline ("../data/17.txt");
  secret = b64decode (secrets[random_long (0, secrets.size () - 1)]);
  auto b = bytes (secret);
  auto [nct, iv] = encrypt_secret ();

  vector<uint8_t> ct (16 + nct.size ());

  for (int i = 0; i < 16; ++i)
    ct[i] = iv[i];
  for (int i = 0; i < nct.size (); ++i)
    ct[i + 16] = nct[i];
  vector<uint8_t> pt;
  for (int block = 0; block + 16 < ct.size (); block += 16)
    {
      vector<uint8_t> cblock (16);

      // detect last byte
      vector<uint8_t> query;
      for (int i = 0; i < block + 32; ++i)
        {
          query.push_back (ct[i]);
        }
      bool found = false;
      for (int g = 0; g < 0x100; ++g)
        {
          uint8_t guess = g & 0xff;
          query[block + 15] ^= guess;
          if (check_valid_padding (query))
            {
              query[block + 14] ^= 0x1;
              if (check_valid_padding (query))
                {
                  cblock[15] = guess ^ 0x1;
                  found = true;
                }
              query[block + 14] ^= 0x1;
            }
          query[block + 15] ^= guess;
          if (found)
            break;
        }

      assert (found);
      for (int pos = 0xe; pos >= 0; pos--)
        {
          uint8_t x = (0x10 - pos) & 0xff;
          bool found = false;
          for (int i = 15; i > pos; --i)
            {
              query[block + i] ^= (cblock[i] ^ x);
            }
          for (int g = 0; g < 0x100; ++g)
            {
              uint8_t guess = g;
              query[block + pos] ^= guess;
              auto b = a->decryptCBC (query, fixed_iv);
              if (check_valid_padding (query))
                {
                  cblock[pos] = guess ^ x;
                  found = true;
                }
              query[block + pos] ^= guess;
              if (found)
                break;
            }
          assert (found);
          for (int i = 15; i > pos; --i)
            query[block + i] ^= (cblock[i] ^ x);
        }
      for (int i = 0; i < 16; ++i)
        {
          pt.push_back (cblock[i]);
        }
    }
  assert (is_valid_PKCS7 (pt));
  uint8_t *s = new uint8_t[pt.size ()];
  for (int i = 0; i < pt.size (); ++i)
    {
      s[i] = pt[i];
    }

  ssize_t sz = remove_pading (s, pt.size ());
  string decrypted_secret = to_string (pt).substr (0, sz);
  assert (decrypted_secret == secret);
  cout << decrypted_secret << "\n";
  delete[] s;
}
