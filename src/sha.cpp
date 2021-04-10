#include "../include/sha.h"

uint8_t amask[] = { 0x0, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe };
uint8_t omask[] = { 0x80, 0x40, 0x20, 0x10, 0x8, 0x4, 0x2, 0x1 };
void
sha1_transform (SHA1_CTX *ctx, const uint8_t *data)
{
  uint32_t a, b, c, d, e, i, j, t, m[80];

  for (i = 0, j = 0; i < 16; ++i, j += 4)
    m[i] = (data[j] << 24) + (data[j + 1] << 16) + (data[j + 2] << 8)
           + (data[j + 3]);
  for (; i < 80; ++i)
    {
      m[i] = (m[i - 3] ^ m[i - 8] ^ m[i - 14] ^ m[i - 16]);
      m[i] = (m[i] << 1) | (m[i] >> 31);
    }

  a = ctx->state[0];
  b = ctx->state[1];
  c = ctx->state[2];
  d = ctx->state[3];
  e = ctx->state[4];

  for (i = 0; i < 20; ++i)
    {
      t = ROTLEFT (a, 5) + ((b & c) ^ (~b & d)) + e + ctx->k[0] + m[i];
      e = d;
      d = c;
      c = ROTLEFT (b, 30);
      b = a;
      a = t;
    }
  for (; i < 40; ++i)
    {
      t = ROTLEFT (a, 5) + (b ^ c ^ d) + e + ctx->k[1] + m[i];
      e = d;
      d = c;
      c = ROTLEFT (b, 30);
      b = a;
      a = t;
    }
  for (; i < 60; ++i)
    {
      t = ROTLEFT (a, 5) + ((b & c) ^ (b & d) ^ (c & d)) + e + ctx->k[2]
          + m[i];
      e = d;
      d = c;
      c = ROTLEFT (b, 30);
      b = a;
      a = t;
    }
  for (; i < 80; ++i)
    {
      t = ROTLEFT (a, 5) + (b ^ c ^ d) + e + ctx->k[3] + m[i];
      e = d;
      d = c;
      c = ROTLEFT (b, 30);
      b = a;
      a = t;
    }

  ctx->state[0] += a;
  ctx->state[1] += b;
  ctx->state[2] += c;
  ctx->state[3] += d;
  ctx->state[4] += e;
}
void
sha1_init (SHA1_CTX *ctx)
{
  ctx->datalen = 0;
  ctx->bitlen = 0;
  ctx->state[0] = 0x67452301;
  ctx->state[1] = 0xEFCDAB89;
  ctx->state[2] = 0x98BADCFE;
  ctx->state[3] = 0x10325476;
  ctx->state[4] = 0xc3d2e1f0;
  ctx->k[0] = 0x5a827999;
  ctx->k[1] = 0x6ed9eba1;
  ctx->k[2] = 0x8f1bbcdc;
  ctx->k[3] = 0xca62c1d6;
}

void
sha1_update (SHA1_CTX *ctx, const uint8_t data[], size_t bitlen)
{
  size_t i = 0;
  ctx->bitlen += bitlen;
  for (i = 0; i < bitlen / 8; ++i)
    {
      ctx->data[ctx->datalen] = data[i];
      ctx->datalen++;
      if (ctx->datalen == 64)
        {
          sha1_transform (ctx, ctx->data);
          ctx->datalen = 0;
        }
    }

  if (bitlen & 0x7)
    {
      int id = bitlen & 0x7;
      ctx->data[ctx->datalen] = (data[i] & amask[id]) | omask[id];
      ctx->datalen++;
      if (ctx->datalen == 64)
        {
          sha1_transform (ctx, ctx->data);
          ctx->datalen = 0;
        }
    }
}
void
sha1_final (SHA1_CTX *ctx, uint8_t *hash)
{
  uint32_t i;

  i = ctx->datalen;

  // Pad whatever data is left in the buffer
  if (ctx->bitlen & 0x7)
    {
      if (ctx->datalen >= 57)
        {
          while (i < 64)
            ctx->data[i++] = 0x00;
          sha1_transform (ctx, ctx->data);
          memset (ctx->data, 0, 56);
        }
      else
        {
          while (i < 56)
            ctx->data[i++] = 0x00;
        }
    }
  else
    {
      if (ctx->datalen < 56)
        {
          ctx->data[i++] = 0x80;
          while (i < 56)
            ctx->data[i++] = 0x00;
        }
      else
        {
          ctx->data[i++] = 0x80;
          while (i < 64)
            ctx->data[i++] = 0x00;
          sha1_transform (ctx, ctx->data);
          memset (ctx->data, 0, 56);
        }
    }

  // Append to the padding the total message's length in bits and transform.
  ctx->data[63] = ctx->bitlen;
  ctx->data[62] = ctx->bitlen >> 8;
  ctx->data[61] = ctx->bitlen >> 16;
  ctx->data[60] = ctx->bitlen >> 24;
  ctx->data[59] = ctx->bitlen >> 32;
  ctx->data[58] = ctx->bitlen >> 40;
  ctx->data[57] = ctx->bitlen >> 48;
  ctx->data[56] = ctx->bitlen >> 56;
  sha1_transform (ctx, ctx->data);

  // Since this implementation uses little endian byte ordering and MD uses big
  // endian, reverse all the bytes when copying the final state to the output
  // hash.
  for (i = 0; i < 4; ++i)
    {
      hash[i] = (ctx->state[0] >> (24 - i * 8)) & 0x000000ff;
      hash[i + 4] = (ctx->state[1] >> (24 - i * 8)) & 0x000000ff;
      hash[i + 8] = (ctx->state[2] >> (24 - i * 8)) & 0x000000ff;
      hash[i + 12] = (ctx->state[3] >> (24 - i * 8)) & 0x000000ff;
      hash[i + 16] = (ctx->state[4] >> (24 - i * 8)) & 0x000000ff;
    }
}
vector<uint8_t>
sha128sum (vector<uint8_t> v, size_t bitlen)
{
  SHA1_CTX ctx;
  sha1_init (&ctx);
  size_t size = 0x100000, cur_block = 0, i = 0, j = 0;
  uint8_t *tmp = new uint8_t[size], *out = new uint8_t[20];
  size_t bytes = bitlen >> 3;
  bool ok = ((bitlen & 0x7) == 0);
  for (i = 0; i < bytes; i += size)
    {
      cur_block = 0;
      for (j = 0; j < size and j + i < bytes; ++j)
        {
          tmp[j] = v[i + j];
          cur_block += 8;
        }
      if (ok)
        {
          sha1_update (&ctx, tmp, cur_block);
        }
    }
  if (bitlen & 0x7)
    {
      tmp[j] = v[bytes];
      cur_block += (bitlen & 0x7);
      sha1_update (&ctx, tmp, cur_block);
    }
  sha1_final (&ctx, out);

  vector<uint8_t> b (out, out + 20);
  delete[] tmp;
  delete[] out;
  return b;
}

vector<uint8_t>
MAC (vector<uint8_t> secret_key, vector<uint8_t> msg, size_t bitlen,
     SHA1_CTX *ctx)
{
  if (!ctx)
    {
      ctx = new SHA1_CTX ();
      sha1_init (ctx);
    }
  size_t size = 0x100000, cur_block = 0, i = 0, j = 0;
  uint8_t *tmp = new uint8_t[size], *out = new uint8_t[20];
  for (int i = 0; i < 64; ++i)
    tmp[i] = secret_key[i];
  sha1_update (ctx, tmp, 512);
  size_t bytes = bitlen >> 3;
  bool ok = ((bitlen & 0x7) == 0);
  for (i = 0; i < bytes; i += size)
    {
      cur_block = 0;
      for (j = 0; j < size and j + i < bytes; ++j)
        {
          tmp[j] = msg[i + j];
          cur_block += 8;
        }
      if (ok)
        {
          sha1_update (ctx, tmp, cur_block);
        }
    }
  if (bitlen & 0x7)
    {
      tmp[j] = msg[bytes];
      cur_block += (bitlen & 0x7);
      sha1_update (ctx, tmp, cur_block);
    }
  sha1_final (ctx, out);
  return vector<uint8_t> (out, out + 20);
}
