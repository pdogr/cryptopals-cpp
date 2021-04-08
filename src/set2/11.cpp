#include "../../include/aes.h"
#include "../../include/disp.h"
#include "../../include/utils.h"

#define ECB_MODE 0
#define CBC_MODE 1

pair<vector<uint8_t>, int>
encryption_oracle (string input)
{
  ssize_t prepend = random_long (5, 10);
  ssize_t append = random_long (5, 10);
  ssize_t input_len = input.size ();
  ssize_t total_len = prepend + input_len + append;

  uint8_t *buf = new uint8_t[total_len];
  random_bytes (buf, prepend);
  for (int i = 0; i < input_len; ++i)
    {
      buf[prepend + i] = input[i];
    }
  random_bytes (buf + prepend + input_len, append);
  uint8_t key[16];
  random_bytes (key, 16);
  AES a (key, 128);
  int toss = random_long (0, 1);
  if (toss == ECB_MODE)
    {
      vector<uint8_t> ct = a.encryptECB (
          vector<uint8_t> (buf, buf + sizeof (uint8_t) * total_len));
      return { ct, ECB_MODE };
    }
  else
    {
      uint8_t iv[16];
      random_bytes (iv, 16);
      vector<uint8_t> ct = a.encryptCBC (
          vector<uint8_t> (buf, buf + sizeof (uint8_t) * total_len),
          vector<uint8_t> (iv, iv + 16 * sizeof (uint8_t)));
      return { ct, CBC_MODE };
    }
}
int
main ()
{
  int num_times = 1e3;
  string input;
  int correct (0);
  for (int i = 0; i < 50; ++i)
    {
      input += "p";
    }
  for (int i = 1; i <= num_times; ++i)
    {
      pair<vector<uint8_t>, int> magic = encryption_oracle (input);
      int actual = magic.second;
      string ct = to_string (magic.first);
      set<string> st;
      for (int j = 0; j < static_cast<int> (ct.size ()); j += 16)
        {
          st.insert (ct.substr (j, 16));
        }
      int pred = CBC_MODE;
      if (ct.size () / 16 - st.size () >= 1)
        {
          pred = ECB_MODE;
        }
      if (pred == actual)
        {
          ++correct;
        }
    }
  cout << "Correct %:" << 100 * float (correct) / float (num_times) << "\n";
}
