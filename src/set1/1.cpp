#include "../../include/utils.h"
int
main ()
{
  string text = "49276d206b696c6c696e6720796f757220627261696e206c696b652061207"
                "06f69736f6e"
                "6f7573206d757368726f6f6d";
  string tgt
      = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";
  assert (b64encode (unhex (text)) == tgt);
  return 0;
}