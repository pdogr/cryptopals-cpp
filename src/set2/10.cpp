#include "../../include/aes.h"
#include "../../include/disp.h"
#include "../../include/utils.h"

int
main ()
{
  string k = "YELLOW SUBMARINE";
  string ct = b64decode (readall ("../data/10.txt"));

  AES a (bytes (k), 128);
  vector<uint8_t> IV = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

  cout << to_string (a.decryptCBC (bytes (ct), IV)) << "\n";
}
