#include "../../include/dh.h"
#include "../../include/disp.h"
#include "../../include/utils.h"

int
main ()
{
  string p_s = "0xffffffffffffffffc90fdaa22168c234c4c6628b80dc1cd129024e088a67"
               "cc74020bbe"
               "a6"
               "3b139b22514a08798e3404ddef9519b3cd3a431b302b0a6df25f14374fe135"
               "6d6d51c245"
               "e485b576625e7ec6f44c42e9a637ed6b0bff5cb6f406b7edee386bfb5a899f"
               "a5ae9f2411"
               "7c4b1fe649286651ece45b3dc2007cb8a163bf0598da48361c55d39a69163f"
               "a8fd24cf5f"
               "83655d23dca3ad961c62f356208552bb9ed529077096966d670c354e4abc98"
               "04f1746c08"
               "ca237327ffffffffffffffff";
  cpp_int p (p_s);
  cpp_int g (2);
  cpp_int a = randbint ();
  cpp_int A = binpow (g, a, p);
  cpp_int b = randbint ();
  cpp_int B = binpow (g, b, p);

  cout << A << " " << B << "\n";
  assert (binpow (A, b, p) == binpow (B, a, p));
  cout << "DH success\n";
}
