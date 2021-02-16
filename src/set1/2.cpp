#include "../../include/utils.h"
#include "../../include/xor.h"

int
main ()
{
  string hex_str1 = "1c0111001f010100061a024b53535009181c";
  string hex_str2 = "686974207468652062756c6c277320657965";
  string tgt_hex = "746865206b696420646f6e277420706c6179";

  assert (eq (hex ((unhex (hex_str1) ^ unhex (hex_str2))), tgt_hex));

  return 0;
}