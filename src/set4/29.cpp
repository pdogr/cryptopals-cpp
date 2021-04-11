#include "../../include/disp.h"
#include "../../include/sha.h"
#include "../../include/utils.h"
#include <string.h>
vector<uint8_t> K;
size_t random_size;

size_t
padding (size_t bitlen)
{
  return (959 - (bitlen % 0x200)) % 0x200;
}
vector<uint8_t>
get_padding (size_t bitlen)
{
  vector<uint8_t> v;
  size_t p = padding (bitlen) + 65;
  v.resize (p / 8, 0);
  v[0] = 0x80;
  for (int i = 7; i >= 0; --i)
    {
      v[v.size () + i - 8] = bitlen & 0xff;
      bitlen >>= 8;
    }
  return v;
}
string msg
    = "comment1=cooking%20MCs;userdata=foo;comment2=%20like%20a%20pound%"
      "20of%20bacon";
string tgt_suffix = ";admin=true;";

bool
is_valid_SHA1MAC (vector<uint8_t> msg, size_t bitlen, vector<uint8_t> hash)
{
  vector<uint8_t> calc = SHA1MAC (K, msg, bitlen);
  return calc == hash;
}
vector<uint8_t>
get_valid_hash (string s)
{
  return SHA1MAC (K, bytes (s), s.size () * 8);
}
int
main ()
{
  random_size = random_long (16, 1000);
  K = random_vec (random_size);
  cout << "Msg size: " << msg.size () << " "
       << "Random key size: " << random_size << "\n";

  // attack starts here
  vector<uint8_t> b1 = bytes (msg);
  vector<uint8_t> true_hash = get_valid_hash (msg);
  for (size_t guess = 1; guess < 1000; ++guess)
    {

      size_t tot_len = 8 * (guess + msg.size ());
      size_t p = padding (tot_len);
      vector<uint8_t> vtgt = bytes (tgt_suffix);
      auto pd = get_padding (tot_len);
      vector<uint8_t> vforged_msg (b1 + pd + vtgt);
      SHA1_CTX ctx = hash2SHA1ctx (true_hash, tot_len + 65 + p);
      string forged_msg = to_string (vforged_msg);
      vector<uint8_t> forged_mac
          = SHA1MAC (vector<uint8_t> (), vtgt, 8 * vtgt.size (), &ctx);
      if (is_valid_SHA1MAC (vforged_msg, 8 * vforged_msg.size (), forged_mac))
        {
          cout << "Guessed key length:" << guess << "\n";
          cout << "Forged msg: " << forged_msg << "\n";
          cout << "True mac:" << get_valid_hash (forged_msg) << "\n";
          cout << "Forged mac: " << forged_mac << "\n";
          cout << "Successfully forged msg: " << forged_msg << "\n";
          break;
        }
    }
  return 0;
}
