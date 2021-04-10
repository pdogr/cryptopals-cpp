#include <bits/stdc++.h>
#include <boost/lexical_cast.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/miller_rabin.hpp>
#include <boost/random/mersenne_twister.hpp>
using namespace std;
using namespace boost::multiprecision;
using namespace boost::random;

cpp_int binpow (cpp_int a, cpp_int b);
cpp_int binpow (cpp_int a, cpp_int b, cpp_int md);

cpp_int get_prime ();
cpp_int randbint ();
