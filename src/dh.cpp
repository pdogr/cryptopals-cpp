#include "../include/dh.h"

cpp_int
binpow (cpp_int a, cpp_int b)
{
  cpp_int r (1);
  while (b)
    {
      if (b & 1)
        r = r * a;
      a = a * a;
      b /= 2;
    }
  return r;
}

cpp_int
binpow (cpp_int a, cpp_int b, cpp_int m)
{

  cpp_int r (1);
  while (b)
    {
      if (b & 1)
        r = r * a % m, r %= m;
      a = a * a % m;
      b /= 2;
    }
  return r;
}

cpp_int
get_prime ()
{
  mt11213b base_gen (clock ());
  boost::random::independent_bits_engine<mt11213b, 512, cpp_int> gen (
      base_gen);

  cpp_int n;
  do
    {
      n = gen ();
    }
  while (!miller_rabin_test (n, 25));
  return n;
}
cpp_int
randbint ()
{
  mt11213b base_gen (clock ());
  boost::random::independent_bits_engine<mt11213b, 512, cpp_int> gen (
      base_gen);
  return gen ();
}
