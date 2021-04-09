#include "../../include/aes.h"
#include "../../include/disp.h"
#include "../../include/utils.h"
#include "../../include/xor.h"
#include <fcntl.h>
uint8_t key[16];
uint64_t nonce (0ull);
vector<uint8_t> fixed_iv;
AES *a;
vector<uint8_t>
encrypt_CTR (vector<uint8_t> pt)
{
  return a->encryptCTR (pt, nonce);
}
vector<uint8_t>
edit (vector<uint8_t> ct, int offset, vector<uint8_t> new_text)
{
  vector<uint8_t> pt = a->decryptCTR (ct, nonce);
  for (int i = 0; i < new_text.size (); ++i)
    {
      if (i + offset < pt.size ())
        {
          pt[i + offset] = new_text[i];
        }
    }
  return a->encryptCTR (pt, nonce);
}
int
main ()
{
  int fd = open ("/dev/urandom", O_RDONLY);
  read (fd, key, 16);
  read (fd, &nonce, sizeof (uint64_t));
  string secret = b64decode (readall ("../data/25.txt"));
  string _key = "YELLOW SUBMARINE";
  a = new AES (bytes (_key), 128);
  vector<uint8_t> pt = a->decryptECB ((bytes (secret)));
  delete a;
  a = new AES (key, 128);
  vector<uint8_t> ct = encrypt_CTR (pt);

  vector<uint8_t> new_text (ct.size ());
  vector<uint8_t> key_stream = edit (ct, 0, new_text);

  auto secret_text = (ct ^ key_stream);
  assert (to_string (secret_text) == to_string (pt));
  cout << to_string (secret_text) << "\n";
}
