#include "../include/aes.h"

AES::AES (vector<uint8_t> k, int key_len)
{
  w = nullptr;
  Nk = key_len / 32;
  Nb = 4;
  switch (key_len)
    {
    case 128:
    case 192:
    case 256:
      Nr = 10;
      break;
    default:
      assert (0);
    }
  w = new uint8_t[4 * Nb * (Nr + 1)];
  uint8_t *key = new uint8_t[Nk * 4];
  for (int i = 0; i < Nk * 4; ++i)
    {
      key[i] = k[i];
    }
  KeyExpansion (key, w, Nk);
}
AES::AES (uint8_t *k, int key_len)
    : AES (vector<uint8_t> (k, k + key_len / 8), key_len)
{
}
AES::AES::~AES ()
{
  if (w != nullptr)
    {
      delete[] w;
    }
}
void
AES::ShiftRows (uint8_t **state)
{
  uint8_t *tmp = new uint8_t[Nb];
  for (int i = 0; i < 4; ++i)
    {
      for (int j = 0; j < Nb; ++j)
        {
          tmp[j] = state[i][(i + j) % Nb];
        }
      memcpy (state[i], tmp, Nb * sizeof (uint8_t));
    }
  delete[] tmp;
}

void
AES::SubBytes (uint8_t **state)
{
  for (int i = 0; i < 4; ++i)
    {
      for (int j = 0; j < Nb; ++j)
        {
          state[i][j] = sbox[state[i][j] / 16][state[i][j] & 0xf];
        }
    }
}
void
AES::MixColumns (uint8_t **state)
{
  uint8_t *tmp = new uint8_t[4];
  for (int j = 0; j < Nb; ++j)
    {
      for (int i = 0; i < 4; ++i)
        {
          tmp[i] = state[i][j];
        }
      // galois mul from wiki

      uint8_t a[4];
      uint8_t b[4];
      uint8_t c;
      uint8_t h;
      /* The array 'a' is simply a copy of the input array 'r'
       * The array 'b' is each element of the array 'a' multiplied by 2
       * in Rijndael's Galois field
       * a[n] ^ b[n] is element n multiplied by 3 in Rijndael's Galois field */
      for (c = 0; c < 4; c++)
        {
          a[c] = tmp[c];
          h = tmp[c] & 0x80; /* hi bit */
          b[c] = tmp[c] << 1;
          if (h == 0x80)
            b[c] ^= 0x1B; /* Rijndael's Galois field */
        }
      tmp[0]
          = b[0] ^ a[3] ^ a[2] ^ b[1] ^ a[1]; /* 2 * a0 + a3 + a2 + 3 * a1 */
      tmp[1]
          = b[1] ^ a[0] ^ a[3] ^ b[2] ^ a[2]; /* 2 * a1 + a0 + a3 + 3 * a2 */
      tmp[2]
          = b[2] ^ a[1] ^ a[0] ^ b[3] ^ a[3]; /* 2 * a2 + a1 + a0 + 3 * a3 */
      tmp[3]
          = b[3] ^ a[2] ^ a[1] ^ b[0] ^ a[0]; /* 2 * a3 + a2 + a1 + 3 * a0 */
      for (int i = 0; i < 4; ++i)
        {
          state[i][j] = tmp[i];
        }
    }
  delete[] tmp;
}
void
AES::AddRoundKey (uint8_t **state, uint8_t *key)
{
  for (int i = 0; i < 4; i++)
    {
      for (int j = 0; j < Nb; j++)
        {
          state[i][j] = state[i][j] ^ key[i + 4 * j];
        }
    }
}

void
AES::KeyExpansion (uint8_t *key, uint8_t *w, int Nk)
{
  uint8_t *tmp = new uint8_t[4], *rcon = new uint8_t[4];
  for (int i = 0; i < Nk; ++i)
    {
      for (int j = 0; j < 4; ++j)
        {
          w[4 * i + j] = key[4 * i + j];
        }
    }
  int i = Nk;
  while (i < Nb * (Nr + 1))
    {
      memcpy (tmp, w + 4 * (i - 1), sizeof (uint8_t) * 4);
      if (i % Nk == 0)
        {
          RotWord (tmp);
          SubWord (tmp);
          Rcon (rcon, i / Nk);
          for (int j = 0; j < 4; ++j)
            {
              tmp[j] ^= rcon[j];
            }
        }
      else if (Nk > 6 and i % Nk == 4)
        {
          SubWord (tmp);
        }
      for (int j = 0; j < 4; ++j)
        {
          w[4 * i + j] = w[4 * i + j - 4 * Nk] ^ tmp[j];
        }
      ++i;
    }

  delete[] tmp;
  delete[] rcon;
}
void
AES::SubWord (uint8_t *word)
{
  for (int i = 0; i < 4; ++i)
    {
      word[i] = sbox[word[i] / 16][word[i] & 0xf];
    }
}

void
AES::RotWord (uint8_t *a)
{
  uint8_t tmp = a[0];
  for (int i = 1; i < 4; ++i)
    {
      a[i - 1] = a[i];
    }
  a[3] = tmp;
}
void
AES::Rcon (uint8_t *a, int i)
{
  a[0] = Rc[i];
  a[1] = a[2] = a[3] = 0;
}

void
AES::InvShiftRows (uint8_t **state)
{

  uint8_t *tmp = new uint8_t[Nb];
  for (int i = 0; i < 4; ++i)
    {
      for (int j = 0; j < Nb; ++j)
        {
          tmp[j] = state[i][((j - i) % Nb + Nb) % Nb];
        }
      memcpy (state[i], tmp, Nb * sizeof (uint8_t));
    }
  delete[] tmp;
}

void
AES::InvSubBytes (uint8_t **state)
{

  for (int i = 0; i < 4; ++i)
    {
      for (int j = 0; j < Nb; ++j)
        {
          state[i][j] = inv_sbox[state[i][j] / 16][state[i][j] & 0xf];
        }
    }
}

void
AES::InvMixColumns (uint8_t **state)
{
  uint8_t *tmp = new uint8_t[4];
  for (int j = 0; j < Nb; ++j)
    {
      for (int i = 0; i < 4; ++i)
        {
          tmp[i] = state[i][j];
        }
      state[0][j]
          = mul14[tmp[0]] ^ mul11[tmp[1]] ^ mul13[tmp[2]] ^ mul9[tmp[3]];
      state[1][j]
          = mul9[tmp[0]] ^ mul14[tmp[1]] ^ mul11[tmp[2]] ^ mul13[tmp[3]];
      state[2][j]
          = mul13[tmp[0]] ^ mul9[tmp[1]] ^ mul14[tmp[2]] ^ mul11[tmp[3]];
      state[3][j]
          = mul11[tmp[0]] ^ mul13[tmp[1]] ^ mul9[tmp[2]] ^ mul14[tmp[3]];
    }
  delete[] tmp;
}

void
AES::encryptBlock (uint8_t in[], uint8_t out[])
{
  uint8_t **state = new uint8_t *[4];
  state[0] = new uint8_t[4 * Nb];
  for (int i = 0; i < 4; ++i)
    {
      state[i] = state[0] + i * Nb;
      for (int j = 0; j < Nb; ++j)
        {
          state[i][j] = in[i + 4 * j];
        }
    }
  AddRoundKey (state, w);

  for (int r = 1; r <= Nr - 1; ++r)
    {
      SubBytes (state);
      ShiftRows (state);
      MixColumns (state);
      AddRoundKey (state, w + r * 4 * Nb);
    }
  SubBytes (state);
  ShiftRows (state);
  AddRoundKey (state, w + Nb * 4 * Nr);
  for (int i = 0; i < 4; ++i)
    {
      for (int j = 0; j < Nb; ++j)
        {
          out[i + 4 * j] = state[i][j];
        }
    }
  delete[] state[0];
  delete[] state;
}

void
AES::decryptBlock (uint8_t *in, uint8_t *out)
{
  uint8_t **state = new uint8_t *[4];
  state[0] = new uint8_t[4 * Nb];
  for (int i = 0; i < 4; ++i)
    {
      state[i] = state[0] + i * Nb;
      for (int j = 0; j < Nb; ++j)
        {
          state[i][j] = in[i + 4 * j];
        }
    }
  AddRoundKey (state, w + Nb * 4 * Nr);
  for (int r = Nr - 1; r >= 1; --r)
    {
      InvSubBytes (state);
      InvShiftRows (state);
      AddRoundKey (state, w + r * 4 * Nb);
      InvMixColumns (state);
    }
  InvSubBytes (state);
  InvShiftRows (state);
  AddRoundKey (state, w);
  for (int i = 0; i < 4; ++i)
    {
      for (int j = 0; j < Nb; ++j)
        {
          out[i + 4 * j] = state[i][j];
        }
    }
  delete[] state[0];
  delete[] state;
}

vector<uint8_t>
AES::encryptECB (vector<uint8_t> pt)
{
  ssize_t input_len = pt.size ();
  pair<ssize_t, uint8_t> padd = PKCS7 (input_len, 4 * Nb);
  ssize_t pad_len = padd.first;
  ssize_t total_len = input_len + pad_len;
  uint8_t *in = new uint8_t[total_len], *out = new uint8_t[total_len];
  for (int i = 0; i < input_len; ++i)
    {
      in[i] = pt[i];
    }

  add_padding (in, total_len, pad_len, padd.second);
  for (int i = 0; i < total_len; i += 4 * Nb)
    {
      encryptBlock (in + i, out + i);
    }
  vector<uint8_t> cipher_text (out, out + total_len * sizeof (uint8_t));
  delete[] in;
  delete[] out;
  return cipher_text;
}

vector<uint8_t>
AES::decryptECB (vector<uint8_t> ct)
{
  ssize_t input_len = ct.size ();
  uint8_t *in = new uint8_t[input_len], *out = new uint8_t[input_len];
  for (int i = 0; i < input_len; ++i)
    {
      in[i] = ct[i];
    }
  for (int i = 0; i < input_len; i += 4 * Nb)
    {
      decryptBlock (in + i, out + i);
    }
  ssize_t unpad_len = remove_pading (out, input_len);
  vector<uint8_t> plain_text (out, out + unpad_len * sizeof (uint8_t));
  delete[] in;
  delete[] out;
  return plain_text;
}

vector<uint8_t>
AES::encryptCBC (vector<uint8_t> pt, vector<uint8_t> IV)
{
  ssize_t input_len = pt.size ();
  pair<ssize_t, uint8_t> padd = PKCS7 (input_len, 4 * Nb);
  ssize_t pad_len = padd.first;
  ssize_t total_len = input_len + pad_len;
  uint8_t *in = new uint8_t[total_len], *out = new uint8_t[total_len];
  uint8_t *block = new uint8_t[4 * Nb];
  for (int i = 0; i < input_len; ++i)
    {
      in[i] = pt[i];
    }
  add_padding (in, total_len, pad_len, padd.second);
  for (int i = 0; i < 4 * Nb; ++i)
    {
      block[i] = IV[i];
    }
  for (int i = 0; i < total_len; i += 4 * Nb)
    {
      for (int j = 0; j < 4 * Nb; ++j)
        {
          block[j] ^= in[i + j];
        }
      encryptBlock (block, out + i);
      memcpy (block, out + i, 4 * Nb * sizeof (uint8_t));
    }
  vector<uint8_t> cipher_text (out, out + total_len * sizeof (uint8_t));
  delete[] in;
  delete[] out;
  delete[] block;
  return cipher_text;
}

vector<uint8_t>
AES::decryptCBC (vector<uint8_t> ct, vector<uint8_t> IV)
{
  ssize_t input_len = ct.size ();
  uint8_t *in = new uint8_t[input_len], *out = new uint8_t[input_len];
  uint8_t *block = new uint8_t[4 * Nb];
  for (int i = 0; i < input_len; ++i)
    {
      in[i] = ct[i];
    }
  for (int i = 0; i < 4 * Nb; ++i)
    {
      block[i] = IV[i];
    }
  for (int i = 0; i < input_len; i += 4 * Nb)
    {
      decryptBlock (in + i, out + i);
      for (int j = 0; j < 4 * Nb; ++j)
        {
          out[i + j] ^= block[j];
        }
      memcpy (block, in + i, 4 * Nb * sizeof (uint8_t));
    }
  ssize_t unpad_len = input_len;
  vector<uint8_t> plain_text (out, out + unpad_len * sizeof (uint8_t));
  delete[] in;
  delete[] out;
  delete[] block;
  return plain_text;
}

vector<uint8_t>
AES::encryptCTR (vector<uint8_t> pt, uint64_t nonce)
{
  ssize_t input_len = pt.size ();
  ssize_t total_len = 4 * Nb * ceil (input_len / 16.0);
  uint8_t *in = new uint8_t[16], *out = new uint8_t[total_len];
  uint64_t counter = 0;
  int id = 0;
  for (int i = 0; i <= 7; ++i)
    {
      in[i] = nonce & 0xff;
      nonce >>= 8;
    }
  for (int i = 0; i < total_len; i += 4 * Nb)
    {
      uint64_t temp = counter;
      for (int j = 0; j <= 7; ++j)
        {
          in[j + 8] = counter & 0xff;
          counter >>= 8;
        }
      counter = temp + 1;
      encryptBlock (in, out + i);
      while (id < input_len and id < i + 4 * Nb)
        {
          out[id] ^= pt[id];
          ++id;
        }
    }
  vector<uint8_t> cipher_text (out, out + input_len * sizeof (uint8_t));
  delete[] in;
  delete[] out;
  return cipher_text;
}
vector<uint8_t>
AES::decryptCTR (vector<uint8_t> ct, uint64_t nonce)
{
  ssize_t input_len = ct.size ();
  ssize_t total_len = 4 * Nb * ceil (input_len / 16.0);
  uint8_t *in = new uint8_t[16], *out = new uint8_t[total_len];
  uint64_t counter = 0;
  int id = 0;
  for (int i = 0; i <= 7; ++i)
    {
      in[i] = nonce & 0xff;
      nonce >>= 0;
    }
  for (int i = 0; i < total_len; i += 4 * Nb)
    {
      uint64_t temp = counter;
      for (int j = 0; j <= 7; ++j)
        {
          in[j + 8] = counter & 0xff;
          counter >>= 8;
        }
      counter = temp + 1;
      encryptBlock (in, out + i);
      while (id < input_len and id < i + 4 * Nb)
        {
          out[id] ^= ct[id];
          ++id;
        }
    }
  vector<uint8_t> plain_text (out, out + input_len * sizeof (uint8_t));
  delete[] in;
  delete[] out;
  return plain_text;
}
string
detect_aes_ecb (vector<string> ct_list, int block_size)
{
  map<int, int> reps;
  int c = 0;
  for (auto ct : ct_list)
    {
      set<string> unique_chunks;
      for (int i = 0; i < static_cast<int> (ct.size ()); i += block_size)
        {
          unique_chunks.insert (ct.substr (i, block_size));
        }
      reps[c] = (ct.size () / block_size) - unique_chunks.size ();
      ++c;
    }
  vector<pair<int, int> > vec (reps.begin (), reps.end ());
  sort (vec.begin (), vec.end (),
        [&] (const pair<int, int> &a, const pair<int, int> &b) {
          return a.second > b.second;
        });
  return ct_list[vec[0].first];
}

pair<ssize_t, uint8_t>
PKCS7 (ssize_t input_len, int block_size)
{
  ssize_t pad = (block_size - input_len % block_size);
  return { pad, static_cast<uint8_t> (pad) };
}

void
add_padding (uint8_t *in, ssize_t buf_size, ssize_t pad_len, uint8_t pad_byte)

{
  for (int i = buf_size - pad_len; i < buf_size; ++i)
    {
      in[i] = pad_byte;
    }
}

ssize_t
remove_pading (uint8_t *in, ssize_t buf_size)
{
  uint8_t pad_byte = in[buf_size - 1];
  ssize_t i = buf_size - pad_byte;
  while (i >= 0 and i < buf_size)
    {
      if (in[i] != pad_byte)
        {
          throw;
        }
      ++i;
    }
  return buf_size - pad_byte;
}

bool
is_valid_PKCS7 (vector<uint8_t> v)
{
  int pad_byte = v.back ();
  if (pad_byte == 0 or pad_byte > 0x10 or v.size () < pad_byte)
    return 0;
  int i = v.size () - pad_byte;
  while (i >= 0 and i < v.size ())
    {
      if (v[i] != pad_byte & 0xf)
        return 0;
      ++i;
    }
  return 1;
}
