#include "../../include/disp.h"
#include "../../include/sha.h"
#include "../../include/utils.h"
#include <string.h>

uint8_t
unhex (char a)
{
  if (a >= '0' && a <= '9')
    return (uint8_t) (a - '0');
  return (uint8_t) (10 + a - 'a');
}
ssize_t
min_bytes (ssize_t bits)
{
  ssize_t bytes = bits >> 3;
  if (bits & 7)
    ++bytes;
  return bytes;
}
void
hex2bytes (char *hex_text, ssize_t bytes_len, uint8_t *bytes_out)
{
  for (int i = 0; i < bytes_len; ++i)
    {
      bytes_out[i] = (uint8_t) ((unhex (hex_text[2 * i]) << 4)
                                + unhex (hex_text[2 * i + 1]));
    }
}
void
test_file (char *filename, const ssize_t md_bytes)
{
  FILE *fp = fopen (filename, "r");
  int total = 0, correct = 0;
  while (!feof (fp))
    {
      ssize_t bitlen;
      fscanf (fp, "Len = %zd\n", &bitlen);
      ssize_t bytes = min_bytes (bitlen);
      if (!bytes)
        {
          bytes++;
        }
      char *hex_msg = (char *)malloc (2 * bytes + 1);
      fscanf (fp, "Msg = %s\n", hex_msg);
      char *hex_md = (char *)malloc (2 * md_bytes + 1);
      fscanf (fp, "MD = %s\n", hex_md);

      uint8_t *msg = (uint8_t *)malloc (bytes);
      uint8_t *md = (uint8_t *)malloc (md_bytes);
      uint8_t *out_md = (uint8_t *)malloc (md_bytes);
      hex2bytes (hex_msg, bytes, msg);
      hex2bytes (hex_md, md_bytes, md);

      auto b = sha128sum (vector<uint8_t> (msg, msg + bytes), bitlen);
      for (int i = 0; i < 20; ++i)
        out_md[i] = b[i];
      int pass = !memcmp (md, out_md, 20);
      auto result = (pass) ? "Test passed" : "Test failed";
      if (!pass)
        {
          fprintf (stderr, "L = %d\n", bitlen);
          fprintf (stderr, "MSG = %s\n", hex_msg);
          fprintf (stderr, "File: %s\n", filename);
          fprintf (stderr, "Expected:\t");
          for (int i = 0; i < md_bytes; ++i)
            {
              fprintf (stderr, "%02x", md[i]);
            }
          fprintf (stderr, "\n");
          fprintf (stderr, "Calculated:\t");
          for (int i = 0; i < md_bytes; ++i)
            {
              fprintf (stderr, "%02x", out_md[i]);
            }
          fprintf (stderr, "\n%s\n", result);
          exit (0);
        }

#ifdef DEBUG
      fprintf (stderr, "File: %s\n", filename);
      fprintf (stderr, "L = %d\n", bitlen);
      fprintf (stderr, "MSG = %s\n", hex_msg);
      fprintf (stderr, "File: %s\n", filename);
      fprintf (stderr, "Expected:\t");
      for (int i = 0; i < md_bytes; ++i)
        {
          fprintf (stderr, "%02x", md[i]);
        }
      fprintf (stderr, "\n");
      fprintf (stderr, "Calculated:\t");
      for (int i = 0; i < md_bytes; ++i)
        {
          fprintf (stderr, "%02x", out_md[i]);
        }
      fprintf (stderr, "\n%s\n", result);
#endif
      free (out_md);
      free (md);
      free (msg);
      free (hex_msg);
      free (hex_md);
      ++total;
      correct += pass;
    }
  fprintf (stderr, "File: %s\tTotal: %d\tCorrect: %d\n", filename, total,
           correct);
}

int
main ()
{
  test_file ("../test_vecs/byte/SHA1ShortMsg.rsp", 20);
  test_file ("../test_vecs/byte/SHA1LongMsg.rsp", 20);
  test_file ("../test_vecs/bit/SHA1ShortMsg.rsp", 20);
  test_file ("../test_vecs/bit/SHA1LongMsg.rsp", 20);
  vector<uint8_t> K = random_vec (0x1111);
  vector<uint8_t> M = random_vec (0xaaaeff);
  cout << "Size of M: " << M.size () << "\n";

  cout << MAC (K, M, M.size () * 8) << "\n";

  for (auto &x : M)
    K.push_back (x);
  cout << sha128sum (K, 8 * K.size ()) << "\n";
}
