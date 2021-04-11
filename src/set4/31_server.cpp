#include "../../include/disp.h"
#include "../../include/mac.h"
#include "../../include/md4.h"
#include "../../include/sha.h"
#include "../../include/utils.h"
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/core/error.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/core/string_type.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/http/field.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/parser.hpp>
#include <boost/beast/http/read.hpp>
#include <boost/beast/http/serializer.hpp>
#include <boost/beast/http/status.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/http/verb.hpp>
#include <boost/beast/version.hpp>

namespace be = boost::beast;
namespace asio = boost::asio;
using tcp = boost::asio::ip::tcp;
namespace http = boost::beast::http;
vector<uint8_t> secret_mac, key;
string secret;

bool
timing_unsafe_cmp (vector<uint8_t> &v, size_t microsleep)
{
  for (int i = 0; i < secret_mac.size (); ++i)
    {
      if (secret_mac[i] != v[i])
        return false;
      usleep (microsleep);
    }
  return true;
}
void
generate_secert_mac ()
{
  key = random_vec (64);
  secret_mac
      = HMAC (key, random_vec (random_long (0x1, 0xaaedf)), sha128sum, 64, 20);
  cout << "Secret valid hmac: " << secret_mac << "\n";
  secret = hex (to_string (secret_mac));
}

int
main (int argc, char *argv[])
{
  size_t microsleep = 50000;
  if (argc > 1)
    {
      microsleep = atoi (argv[1]);
    }
  generate_secert_mac ();
  auto address = asio::ip::make_address ("127.0.0.1");
  auto port = uint16_t (8080);
  auto handler = [&microsleep] (tcp::socket socket) {
    auto err = [] (be::error_code ec, char const *msg) {
      cerr << msg << ": " << ec.message () << "\n";
    };
    auto log
        = [] (http::request<http::string_body> sb) { cerr << sb << "\n"; };
    auto const not_allowed = [] (boost::beast::string_view why,
                                 http::request<http::string_body> req) {
      http::response<http::string_body> res{ http::status::method_not_allowed,
                                             req.version () };
      res.set (http::field::server, BOOST_BEAST_VERSION_STRING);
      res.set (http::field::content_type, "text/html");
      res.keep_alive (req.keep_alive ());
      res.body () = why.to_string ();
      res.prepare_payload ();
      return res;
    };

    auto handle_get = [&microsleep] (http::request<http ::string_body> req)
        -> http::response<http::string_body> {
      auto v = bytes (unhex (req.target ().data ()));
      bool ok = true;
      for (int i = 0; i < secret_mac.size (); ++i)
        {
          if (secret_mac[i] != v[i])
            {
              ok = false;
              break;
            }
          usleep (microsleep);
        }
      auto status = ok ? http::status::ok : http::status::not_acceptable;
      http::response<http::string_body> res (status, req.version ());
      res.set (http::field::server, BOOST_BEAST_VERSION_STRING);
      res.set (http::field::content_type, "text/html");
      res.content_length (secret.size ());
      res.keep_alive (req.keep_alive ());
      res.body () = secret;
      return res;
    };
    be::error_code ec;
    while (1)
      {
        be::flat_buffer buffer;
        http::request<http::string_body> req;
        http::read (socket, buffer, req, ec);
        http::request_parser<http::string_body> parser;
        if (ec == http::error::end_of_stream)
          break;
        if (ec)
          {
            return err (ec, "in reading req");
          }
        log (req);

        switch (req.method ())
          {
          case http::verb::get:
            {
              http::write (socket, handle_get (req), ec);
              break;
            }
          default:
            {
              http::write (socket, not_allowed ("Only get", req), ec);
            }
          }
        if (ec)
          {
            return err (ec, "Sending resp");
          }
        continue;
      }
    socket.shutdown (tcp::socket::shutdown_send, ec);
  };
  asio::io_context ioc{};
  tcp::acceptor acceptor{ ioc, { address, port } };
  for (;;)
    {
      auto socket = acceptor.accept (ioc);
      if (!socket.is_open ())
        break;
      std::thread{ handler, std::move (socket) }.detach ();
    }
  return 0;
}
