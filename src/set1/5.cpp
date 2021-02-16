#include "../../include/utils.h"
#include "../../include/xor.h"

int
main ()
{
  string plaintext = "Burning 'em, if you ain't quick and nimble\n"
                     "I go crazy when I hear a cymbal";
  string key = "ICE";

  string xored = plaintext ^ key;

  string tgthex = "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a2"
                  "6226324272765"
                  "272a282b2f20430a652e2c652a3124333a653e2b2027630c692b2028316"
                  "5286326302e27"
                  "282f";
  assert (eq (hex (xored), tgthex));

  return 0;
}