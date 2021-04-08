#include "../../include/aes.h"
#include "../../include/disp.h"
#include "../../include/utils.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
string b64encrypted
    = "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkgaGFpciBjYW4"
      "gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBqdXN0IHRvIHNheSBoaQpEaW"
      "QgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUgYnkK";

uint8_t key[16];
AES *a;
vector<uint8_t>
encryption_oracle_fixed_key (string input)
{

  string unknown_str = b64decode (b64encrypted);
  ssize_t unknown_len = unknown_str.size ();
  ssize_t input_len = input.size ();
  ssize_t total_len = input_len + unknown_len;

  uint8_t *buf = new uint8_t[total_len];
  for (int i = 0; i < input_len; ++i)
    {
      buf[i] = input[i];
    }
  for (int i = 0; i < unknown_len; ++i)
    {
      buf[input_len + i] = unknown_str[i];
    }
  vector<uint8_t> ct = a->encryptECB (
      vector<uint8_t> (buf, buf + sizeof (uint8_t) * total_len));
  return ct;
}
int
main ()
{
  int fd = open ("/dev/urandom", O_RDONLY);
  read (fd, key, 16);
  a = new AES (key, 128);
  ssize_t block_size = 16;
  vector<uint8_t> found;
  ssize_t unk_len = to_string (encryption_oracle_fixed_key ("")).size ();
  ssize_t total_input_size
      = block_size * (unk_len / block_size) + 2 * block_size;

  for (int i = 0; i < unk_len; ++i)
    {
      map<string, char> mp;
      string fixed_input;
      for (int j = 0; j < total_input_size - 1 - found.size (); ++j)
        {
          fixed_input += "p";
        }
      string ct = to_string (encryption_oracle_fixed_key (fixed_input));
      string match = ct.substr (total_input_size - block_size, block_size);
      for (int j = 0; j < found.size (); ++j)
        {
          fixed_input += found[j];
        }
      for (int j = 0; j < 256; ++j)
        {
          string cur = fixed_input + char (j);
          string ct = to_string (encryption_oracle_fixed_key (cur));
          mp[ct.substr (total_input_size - block_size, block_size)] = char (j);
        }

      if (mp.find (match) == mp.end ())
        {
          assert (0);
        }
      if (mp[match] == 0x1)
        {
          break;
        }
      found.push_back (mp[match]);
    }
  string pred = to_string (found);
  string actual = b64decode (b64encrypted);
  assert (pred == actual);
  cout << pred << "\n";
}
