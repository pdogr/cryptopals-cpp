#include "../include/mac.h"

vector<uint8_t>
HMAC (const vector<uint8_t> key, const vector<uint8_t> msg,
      function<vector<uint8_t> (vector<uint8_t>, size_t)> hash,
      size_t block_size, size_t output_size)
{
  vector<uint8_t> K (block_size);
  if (key.size () > block_size)
    {
      K = hash (key, 8 * key.size ());
    }
  else
    {
      for (int i = 0; i < key.size (); ++i)
        {
          K[i] = key[i];
        }
    }
  vector<uint8_t> o_key_pad (block_size, 0x5c), i_key_pad (block_size, 0x36);
  for (int i = 0; i < block_size; ++i)
    {
      o_key_pad[i] ^= K[i];
      i_key_pad[i] ^= K[i];
    }
  vector<uint8_t> im (i_key_pad);
  im.insert (im.end (), msg.begin (), msg.end ());

  vector<uint8_t> hash_im = hash (im, 8 * im.size ());
  vector<uint8_t> oh (o_key_pad);
  oh.insert (oh.end (), hash_im.begin (), hash_im.end ());
  return hash (oh, 8 * oh.size ());
}
