#include "../include/utils.h"

mt19937_64 rng (chrono::steady_clock::now ().time_since_epoch ().count ());

int64_t
random_long (long long l, long long r)
{
  uniform_int_distribution<int64_t> generator (l, r); // closed interval
  return generator (rng);
}
string
b64encode (string b64)
{
  using namespace boost::archive::iterators;
  using It = base64_from_binary<
      transform_width<std::string::const_iterator, 6, 8> >;
  auto tmp = std::string (It (std::begin (b64)), It (std::end (b64)));
  return tmp.append ((3 - b64.size () % 3) % 3, '=');
}

string
b64decode (string text)
{
  using namespace boost::archive::iterators;
  using It
      = transform_width<binary_from_base64<std::string::const_iterator>, 8, 6>;
  return boost::algorithm::trim_right_copy_if (
      std::string (It (std::begin (text)), It (std::end (text))),
      [] (char c) { return c == '\0'; });
}
string
unhex (string hex)
{
  return boost::algorithm::unhex (hex);
}

string
hex (string str)
{
  return boost::algorithm::hex (str);
}

vector<uint8_t>
bytes (string str)
{
  vector<uint8_t> v (str.size ());
  for (int i = 0; i < str.size (); ++i)
    {
      v[i] = static_cast<uint8_t> (str[i]);
    }
  return v;
}
string
to_string (vector<uint8_t> v)
{
  return string (v.begin (), v.end ());
}

string
hex (vector<uint8_t> v)
{
  return hex (to_string (v));
}

bool
eq (string a, string b)
{
  return boost::iequals (a, b);
}

bool
printable (vector<uint8_t> v)
{
  for (int i = 0; i < v.size (); ++i)
    {
      if (!isprint (v[i]))
        return false;
    }
  return true;
}

string
readall (char *filename)
{
  fstream f (filename);
  string t;
  string s;
  while (f >> t)
    {
      s += t;
    }
  return s;
}

void
random_bytes (uint8_t *out, ssize_t l)
{
  ifstream urandom ("/dev/urandom", ios::in | ios::binary);
  if (urandom)
    {
      urandom.read (reinterpret_cast<char *> (out), l * sizeof (uint8_t));
    }
}