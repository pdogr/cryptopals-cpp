#include "../include/md4.h"

static uint8_t amask[] = { 0x0, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe };
static uint8_t omask[] = { 0x80, 0x40, 0x20, 0x10, 0x8, 0x4, 0x2, 0x1 };
void
md4_transform (MD4_CTX *ctx, const uint8_t *data)
{
  uint32_t a, b, c, d;
  uint32_t *d32 = (uint32_t *)data;
  a = ctx->state[0];
  b = ctx->state[1];
  c = ctx->state[2];
  d = ctx->state[3];
  R1 (a, b, c, d, d32[0], 3);
  R1 (d, a, b, c, d32[1], 7);
  R1 (c, d, a, b, d32[2], 11);
  R1 (b, c, d, a, d32[3], 19);
  R1 (a, b, c, d, d32[4], 3);
  R1 (d, a, b, c, d32[5], 7);
  R1 (c, d, a, b, d32[6], 11);
  R1 (b, c, d, a, d32[7], 19);
  R1 (a, b, c, d, d32[8], 3);
  R1 (d, a, b, c, d32[9], 7);
  R1 (c, d, a, b, d32[10], 11);
  R1 (b, c, d, a, d32[11], 19);
  R1 (a, b, c, d, d32[12], 3);
  R1 (d, a, b, c, d32[13], 7);
  R1 (c, d, a, b, d32[14], 11);
  R1 (b, c, d, a, d32[15], 19);
  R2 (a, b, c, d, d32[0], 3);
  R2 (d, a, b, c, d32[4], 5);
  R2 (c, d, a, b, d32[8], 9);
  R2 (b, c, d, a, d32[12], 13);
  R2 (a, b, c, d, d32[1], 3);
  R2 (d, a, b, c, d32[5], 5);
  R2 (c, d, a, b, d32[9], 9);
  R2 (b, c, d, a, d32[13], 13);
  R2 (a, b, c, d, d32[2], 3);
  R2 (d, a, b, c, d32[6], 5);
  R2 (c, d, a, b, d32[10], 9);
  R2 (b, c, d, a, d32[14], 13);
  R2 (a, b, c, d, d32[3], 3);
  R2 (d, a, b, c, d32[7], 5);
  R2 (c, d, a, b, d32[11], 9);
  R2 (b, c, d, a, d32[15], 13);
  R3 (a, b, c, d, d32[0], 3);
  R3 (d, a, b, c, d32[8], 9);
  R3 (c, d, a, b, d32[4], 11);
  R3 (b, c, d, a, d32[12], 15);
  R3 (a, b, c, d, d32[2], 3);
  R3 (d, a, b, c, d32[10], 9);
  R3 (c, d, a, b, d32[6], 11);
  R3 (b, c, d, a, d32[14], 15);
  R3 (a, b, c, d, d32[1], 3);
  R3 (d, a, b, c, d32[9], 9);
  R3 (c, d, a, b, d32[5], 11);
  R3 (b, c, d, a, d32[13], 15);
  R3 (a, b, c, d, d32[3], 3);
  R3 (d, a, b, c, d32[11], 9);
  R3 (c, d, a, b, d32[7], 11);
  R3 (b, c, d, a, d32[15], 15);
  ctx->state[0] += a;
  ctx->state[1] += b;
  ctx->state[2] += c;
  ctx->state[3] += d;
}

void
md4_init (MD4_CTX *ctx)
{
  ctx->datalen = 0;
  ctx->bitlen = 0;
  ctx->state[0] = 0x67452301;
  ctx->state[1] = 0xefcdab89;
  ctx->state[2] = 0x98badcfe;
  ctx->state[3] = 0x10325476;
}

void
md4_update (MD4_CTX *ctx, const uint8_t data[], size_t bitlen, bool fin)
{
  size_t i = 0;
  for (i = 0; i < bitlen / 8; ++i)
    {
      ctx->data[ctx->datalen] = data[i];
      ctx->datalen++;
      ctx->bitlen += 8;
      if (ctx->datalen == 64)
        {
          md4_transform (ctx, ctx->data);
          ctx->datalen = 0;
        }
    }

  if (fin and bitlen & 0x7)
    {
      int id = bitlen & 0x7;
      ctx->data[ctx->datalen] = (data[i] & amask[id]) | omask[id];
      ctx->datalen++;
      ctx->bitlen += (bitlen & 0x7);
      if (ctx->datalen == 64)
        {
          md4_transform (ctx, ctx->data);
          ctx->datalen = 0;
        }
    }
}
void
md4_final (MD4_CTX *ctx, uint8_t *hash)
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
          md4_transform (ctx, ctx->data);
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
          md4_transform (ctx, ctx->data);
          memset (ctx->data, 0, 56);
        }
    }
  // Append to the padding the total message's length in bits and transform.
  ctx->data[56] = ctx->bitlen;
  ctx->data[57] = ctx->bitlen >> 8;
  ctx->data[58] = ctx->bitlen >> 16;
  ctx->data[59] = ctx->bitlen >> 24;
  ctx->data[60] = ctx->bitlen >> 32;
  ctx->data[61] = ctx->bitlen >> 40;
  ctx->data[62] = ctx->bitlen >> 48;
  ctx->data[63] = ctx->bitlen >> 56;
  md4_transform (ctx, ctx->data);
  // Since this implementation uses little endian byte ordering and MD uses big
  // endian, reverse all the bytes when copying the final state to the output
  // hash.
  memcpy (hash, ctx->state, 16);
}

MD4_CTX
hash2MD4ctx (vector<uint8_t> hash, size_t bitlen)
{
  MD4_CTX ctx;
  md4_init (&ctx);
  ctx.bitlen = bitlen;
  ctx.datalen = 0;
  memset (ctx.state, 0, sizeof (ctx.state));
  for (int i = 0; i < 4; ++i)
    {
      ctx.state[0] |= (hash[i] << (8 * i));
      ctx.state[1] |= (hash[i + 4] << (8 * i));
      ctx.state[2] |= (hash[i + 8] << (8 * i));
      ctx.state[3] |= (hash[i + 12] << (8 * i));
    }
  return ctx;
}
vector<uint8_t>
md4sum (vector<uint8_t> v, size_t bitlen)
{
  MD4_CTX ctx;
  md4_init (&ctx);
  size_t size = 0x100000, cur_block = 0, i = 0, j = 0;
  uint8_t *tmp = new uint8_t[size], *out = new uint8_t[16];
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
          md4_update (&ctx, tmp, cur_block);
        }
    }
  if (bitlen & 0x7)
    {
      tmp[j] = v[bytes];
      cur_block += (bitlen & 0x7);
      md4_update (&ctx, tmp, cur_block, true);
    }
  md4_final (&ctx, out);

  vector<uint8_t> b (out, out + 16);
  delete[] tmp;
  delete[] out;
  return b;
}

vector<uint8_t>
MD4MAC (vector<uint8_t> secret_key, vector<uint8_t> msg, size_t bitlen,
        MD4_CTX *ctx)
{
  bool remove = false;
  if (!ctx)
    {
      remove = true;
      ctx = new MD4_CTX ();
      md4_init (ctx);
    }
  size_t size = 0x100000, cur_block = 0, i = 0, j = 0;
  uint8_t *tmp = new uint8_t[size], *out = new uint8_t[16];
  size_t bytes = bitlen >> 3;
  if (secret_key.size ())
    {
      for (j = 0; j < secret_key.size (); ++j)
        {
          tmp[j] = secret_key[j];
        }
      md4_update (ctx, tmp, 8 * secret_key.size ());
    }

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
          md4_update (ctx, tmp, cur_block);
        }
    }
  if (bitlen & 0x7)
    {
      tmp[j] = msg[bytes];
      cur_block += (bitlen & 0x7);
      md4_update (ctx, tmp, cur_block, true);
    }
  md4_final (ctx, out);
  if (remove)
    {
      free (ctx);
    }
  return vector<uint8_t> (out, out + 16);
}
