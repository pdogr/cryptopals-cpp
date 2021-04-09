#include <bits/stdc++.h>
using namespace std;

// taken from https://github.com/B-Con/crypto-algorithms
//
#define ROTLEFT(a, b) ((a << b) | (a >> (32 - b)))
struct SHA1_CTX
{
  uint8_t data[64];
  uint32_t datalen;
  size_t bitlen;
  uint32_t state[5];
  uint32_t k[4];
};

void sha1_init (SHA1_CTX *ctx);
void sha1_update (SHA1_CTX *ctx, const uint8_t *data, size_t len);
void sha1_final (SHA1_CTX *ctx, uint8_t *hash);

vector<uint8_t> sha128sum (vector<uint8_t> v, size_t bitlen);
