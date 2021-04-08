#include "../../include/aes.h"
#include "../../include/disp.h"
#include "../../include/utils.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

uint8_t key[16];
map<string, string>
parse_cookie (string c)
{
  map<string, string> m;
  int i = 0, j = 0;
  while (1)
    {
      if (i == c.size () or c[i] == '&')
        {
          int k = j;
          while (k < i and c[k] != '=')
            ++k;
          if (k == i or j == k)
            throw;
          string key = c.substr (j, k - j),
                 value = c.substr (k + 1, i - (k + 1));
          if (m.find (key) != m.end ())
            throw;
          m[key] = value;
          j = i + 1;
          if (i == c.size ())
            return m;
        }
      ++i;
    }
}
string
profile_for (string email)
{
  string safe_email;
  int i = 0;
  while (i < email.size ())
    {
      if (email[i] != '=' and email[i] != '&')
        safe_email += email[i];
      ++i;
    }
  return string ("email=" + safe_email + "&uid=10&role=user");
}
string test_cookie = "foo=bar&baz=quiz&zap=zazzle";
int
main ()
{
  int fd = open ("/dev/urandom", O_RDONLY);
  read (fd, key, 16);
  AES a (key, 128);
  cout << "Test cookie parsed " << parse_cookie (test_cookie) << "\n";
  cout << "Profile for " << profile_for ("foo@bar.com") << "\n";
  string target_email = "foo@bar.com";
  vector<uint8_t> admin_block;
  string payload;
  payload += "AAAAAAAAAA";

  // generate admin block
  payload += "admin";
  for (int i = 0; i < 11; ++i)
    {
      payload += char (0xb);
    }
  payload += target_email;
  auto b = a.encryptECB (bytes (profile_for (payload)));
  for (int i = 16; i < 32; ++i)
    {
      admin_block.push_back (b[i]);
    }

  // align role=|user to boundary and get 2nd last block
  payload = "";
  payload = "foo@bar99.com";
  b = a.encryptECB (bytes (profile_for (payload)));
  // drop last block and add admin block
  //
  vector<uint8_t> final (48);
  for (int i = 0; i < 32; ++i)
    {
      final[i] = b[i];
    }
  for (int i = 0; i < 16; ++i)
    {
      final[32 + i] = admin_block[i];
    }
  auto mp = parse_cookie (to_string (a.decryptECB (final)));
  cout << mp << "\n";
  assert (mp["role"] == "admin");
  return 0;
}