#include "../../include/aes.h"
#include "../../include/disp.h"
#include "../../include/utils.h"
#include "../../include/xor.h"
#include <cctype>
#include <fcntl.h>

uint8_t key[16], IV[16];
AES *a;
vector<uint8_t>
quote_encrypt (string s)
{
  return a->encryptCBC (bytes (s), vector<uint8_t> (IV, IV + 16));
}
bool
decrypt (vector<uint8_t> v, vector<uint8_t> &pt)
{
  auto b = a->decryptCBC (v, vector<uint8_t> (IV, IV + 16));
  pt = b;
  if (!is_valid_PKCS7 (b))
    return false;
  return true;
}
int
main ()
{
  int fd = open ("/dev/urandom", O_RDONLY);
  read (fd, key, 16);
  a = new AES (key, 128);
  memcpy (IV, key, 16);

  string block1 = "AAAAAAAAAAAAAAAA";
  auto ct = quote_encrypt (block1 + block1 + block1);
  cout << "Key used: " << vector<uint8_t> (key, key + 16) << "\n";
  vector<uint8_t> pt (48);
  vector<uint8_t> b2 (16);
  uint8_t *tmp = new uint8_t[16];
  vector<uint8_t> payload (48);
  for (int i = 0; i < 16; ++i)
    payload[i] = block1[i], payload[32 + i] = block1[i];
  for (int i = 16; i < 32; ++i)
    payload[i] = b2[i - 16];
  for (int g = 0; g < 0xff; ++g)
    {
      b2[15] = (g & 0xff);
      payload[31] = b2[15];
      if (decrypt (payload, pt))
        {
          vector<uint8_t> p_1 (16), p_3 (16);
          for (int i = 0; i < 16; ++i)
            p_1[i] = pt[i], p_3[i] = pt[32 + i];
          auto key_guess = (p_1 ^ (p_3 ^ b2));
          cout << "Key calc: " << key_guess << "\n";
        }
    }
}
