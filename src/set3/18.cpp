#include "../../include/aes.h"
#include "../../include/disp.h"
#include "../../include/utils.h"
int
main ()
{
  string secret = b64decode (readall ("../data/18.txt"));
  vector<uint8_t> key = bytes ("YELLOW SUBMARINE");
  uint64_t nonce (0ull);
  AES a (key, 128);
  auto b = bytes (secret);
  auto pt = a.decryptCTR (b, nonce);
  cout << to_string (pt) << "\n";
  return 0;
}
