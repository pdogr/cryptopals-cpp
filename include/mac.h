#include <bits/stdc++.h>
using namespace std;

vector<uint8_t> HMAC (vector<uint8_t> key, vector<uint8_t> msg,
                      function<vector<uint8_t> (vector<uint8_t>, size_t)> hash,
                      size_t block_size, size_t output_size);
