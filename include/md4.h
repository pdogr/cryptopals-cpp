#include <bits/stdc++.h>
using namespace std;

struct MD4_CTX
{
  uint8_t data[64];
  uint32_t datalen;
  uint64_t bitlen;
  uint32_t state[4];
};

#define FF(X, Y, Z) ((X & Y) | ((~X) & Z))
#define GG(X, Y, Z) ((X & Y) | (Y & Z) | (X & Z))
#define HH(X, Y, Z) (X ^ Y ^ Z)

#define ROTLEFT(a, b) ((a << b) | (a >> (32 - b)))
#define R1(A, B, C, D, X, s) (A = ROTLEFT ((A + FF (B, C, D) + X), s))
#define R2(A, B, C, D, X, s)                                                  \
  (A = ROTLEFT ((A + GG (B, C, D) + X + 0x5a827999), s))
#define R3(A, B, C, D, X, s)                                                  \
  (A = ROTLEFT ((A + HH (B, C, D) + X + 0x6ed9eba1), s))

void md4_init (MD4_CTX *ctx);
void md4_update (MD4_CTX *ctx, const uint8_t *data, size_t len, bool = false);
void md4_final (MD4_CTX *ctx, uint8_t *hash);
vector<uint8_t> md4sum (vector<uint8_t>, size_t bitlen);
MD4_CTX hash2MD4ctx (vector<uint8_t> hash, size_t bitlen);
vector<uint8_t> MD4MAC (vector<uint8_t> secret_key, vector<uint8_t> msg,
                        size_t bitlen, MD4_CTX * = 0);
