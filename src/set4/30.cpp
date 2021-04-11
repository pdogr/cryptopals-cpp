#include "../../include/disp.h"
#include "../../include/md4.h"
#include "../../include/utils.h"
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
  size_t p = (padding (bitlen) + 65) / 8;
  v.resize (p, 0);
  v[0] = 0x80;
  for (int i = 0; i < 8; ++i)
    {
      v[p - 1 - 7 + i] = bitlen & 0xff;
      bitlen >>= 8;
    }
  return v;
}
string msg = "comment1=cooking%20MCs;userdwadawdwadawdawdata=foo;comment2=%"
             "20like%20a%20pound%"
             "20of%20bacon";
string tgt_suffix = ";admin=true;always blue;";

bool
is_valid_MD4MAC (vector<uint8_t> msg, size_t bitlen, vector<uint8_t> hash)
{
  vector<uint8_t> calc = MD4MAC (K, msg, bitlen);
  return calc == hash;
}
vector<uint8_t>
get_valid_hash (string s)
{
  return MD4MAC (K, bytes (s), s.size () * 8);
}
int
main ()
{
  cout << std::hex;
  vector<uint8_t> t1, t2 (bytes ("a")),
      t3 (bytes (
          "123456789012345678901234567890123456789012345678901234567890123"
          "45678901234567890")),
      t4 (bytes ("abcdefghijklmnopqrstuvwxyz")),
      t5 (bytes (
          "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"));

  cout << md4sum (t1, 0) << "\n";
  cout << md4sum (t2, 8) << "\n";
  cout << md4sum (t3, 8 * t3.size ()) << "\n";
  cout << md4sum (t4, 8 * t4.size ()) << "\n";
  random_size = random_long (16, 1000);
  K = random_vec (random_size);
  vector<uint8_t> M = random_vec (0xaaaeff);
  cout << "Size of M: " << M.size () << "\n";

  cout << MD4MAC (K, M, M.size () * 8) << "\n";

  for (auto &x : M)
    K.push_back (x);
  cout << md4sum (K, 8 * K.size ()) << "\n";
  cout << "MD4MAC working\nMessage forging...\n";

  // attack starts here
  random_size = random_long (16, 1000);
  K = random_vec (random_size);
  cout << "Msg size: " << msg.size () << " "
       << "Random key size: " << random_size << "\n";
  vector<uint8_t> b1 = bytes (msg);
  vector<uint8_t> true_hash = get_valid_hash (msg);
  for (size_t guess = 1; guess < 1000; ++guess)
    {

      size_t tot_len = 8 * (guess + msg.size ());
      size_t p = padding (tot_len);
      vector<uint8_t> vtgt = bytes (tgt_suffix);
      auto pd = get_padding (tot_len);
      vector<uint8_t> vforged_msg (b1 + pd + vtgt);
      MD4_CTX ctx = hash2MD4ctx (true_hash, tot_len + 65 + p);
      string forged_msg = to_string (vforged_msg);
      vector<uint8_t> forged_mac
          = MD4MAC (vector<uint8_t> (), vtgt, 8 * vtgt.size (), &ctx);

      if (is_valid_MD4MAC (vforged_msg, 8 * vforged_msg.size (), forged_mac))
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
