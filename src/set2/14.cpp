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

uint8_t *random_prefix;
int random_length;
uint8_t key[16];
AES *a;
vector<uint8_t>
encryption_oracle_fixed_key_random_prefix (string input)
{

  string unknown_str = b64decode (b64encrypted);
  ssize_t unknown_len = unknown_str.size ();
  ssize_t input_len = input.size ();
  ssize_t total_len = random_length + input_len + unknown_len;

  uint8_t *buf = new uint8_t[total_len];
  for (int i = 0; i < random_length; ++i)
    {
      buf[i] = random_prefix[i];
    }
  for (int i = random_length; i < random_length + input_len; ++i)
    {
      buf[i] = input[i - random_length];
    }
  for (int i = random_length + input_len; i < total_len; ++i)
    {
      buf[i] = unknown_str[i - (random_length + input_len)];
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
  read (fd, &random_length, sizeof (int));
  random_length &= 0xf;
  random_prefix = new uint8_t[random_length];
  read (fd, random_prefix, random_length);
  ssize_t block_size = 16;
  vector<uint8_t> found;
  ssize_t unk_len
      = to_string (encryption_oracle_fixed_key_random_prefix ("")).size ();
  ssize_t total_input_size
      = block_size * (unk_len / block_size) + 2 * block_size;
  int prefix_len_guess = 0;
  string guess = "";
  vector<uint8_t> prev;
  while (prefix_len_guess <= 17)
    {
      vector<uint8_t> cur = encryption_oracle_fixed_key_random_prefix (guess);
      bool flag = 1;
      if (prev.size ())
        {
          for (int i = 0; i < 16; ++i)
            {
              if (cur[i] != prev[i])
                flag = 0;
            }
          if (flag)
            break;
        }
      prev = cur;
      guess += "p";
      prefix_len_guess++;
    }
  prefix_len_guess--;
  for (int i = 0; i < unk_len; ++i)
    {
      map<string, char> mp;
      string fixed_input;
      for (int j = 0; j < prefix_len_guess; ++j)
        {
          fixed_input += "a";
        }
      for (int j = 0; j < total_input_size - 1 - found.size (); ++j)
        {
          fixed_input += "p";
        }
      string ct = to_string (
          encryption_oracle_fixed_key_random_prefix (fixed_input));
      string match
          = ct.substr (16 + total_input_size - block_size, block_size);
      for (int j = 0; j < found.size (); ++j)
        {
          fixed_input += found[j];
        }
      for (int j = 0; j < 256; ++j)
        {
          string cur = fixed_input + char (j);
          string ct
              = to_string (encryption_oracle_fixed_key_random_prefix (cur));
          mp[ct.substr (16 + total_input_size - block_size, block_size)]
              = char (j);
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
