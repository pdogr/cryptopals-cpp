#include <bits/stdc++.h>

#include <boost/algorithm/hex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

#define all(x) x.begin (), x.end ()
#define F first
#define S second

using namespace std;
using namespace __gnu_pbds;

int64_t random_long (long long, long long);
string b64encode (string);
string b64decode (string);
string unhex (string);
string hex (string);

vector<uint8_t> bytes (string);
string to_string (uint8_t[], ssize_t);
string hex (vector<uint8_t>);
vector<uint8_t> bytes (string);
string to_string (vector<uint8_t>);

bool eq (string, string);
bool printable (vector<uint8_t>);

string readall (char *);

void random_bytes(uint8_t *,ssize_t);