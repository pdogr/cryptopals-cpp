#include "../../include/aes.h"
#include "../../include/disp.h"
#include "../../include/utils.h"

int
main ()
{
  string valid = "ICE ICE BABY";
  for (int i = 0; i < 4; ++i)
    valid += char (0x4);
  assert (is_valid_PKCS7 (bytes (valid)) == 1);
  string non_valid = "ICE ICE BABY";
  for (int i = 0; i < 4; ++i)
    non_valid += char (0x5);
  assert (is_valid_PKCS7 (bytes (non_valid)) == 0);
}
