#include "../../include/aes.h"
#include "../../include/disp.h"
#include "../../include/utils.h"

int
main ()
{
  string s = "YELLOW SUBMARINE";
  vector<uint8_t> b = bytes (s);
  int input_len = b.size ();
  int block_len = 20;
  pair<ssize_t, uint8_t> pad_info = PKCS7 (input_len, block_len);
  int final_len = input_len + pad_info.F;
  uint8_t in[final_len];
  cout << "Initial: " << b << "\n\n";
  for (int i = 0; i < input_len; ++i)
    {
      in[i] = b[i];
    }
  add_padding (in, final_len, pad_info.F, pad_info.S);
  cout << "Final: " << vector<uint8_t> (in, in + final_len) << "\n\n";
}