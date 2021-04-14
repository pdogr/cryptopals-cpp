#include "../../include/disp.h"
#include "../../include/mac.h"
#include "../../include/md4.h"
#include "../../include/sha.h"
#include "../../include/utils.h"
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/core/error.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/status.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/http/verb.hpp>
#include <boost/beast/version.hpp>

namespace be = boost::beast;
namespace asio = boost::asio;
using tcp = boost::asio::ip::tcp;
namespace http = boost::beast::http;

auto host = "127.0.0.1";
auto port = "8080";

int
main (int argc, char *argv[])
{
  int tries = 10;
  if (argc == 2)
    {
      tries = stoi (argv[1]);
      assert (tries > 0);
    }
  using namespace std::chrono;
  const auto send = [] (vector<uint8_t> &v) {
    asio::io_context ioc{};
    tcp::resolver resolver{ ioc };
    tcp::socket socket{ ioc };
    auto const results = resolver.resolve (host, port);
    asio::connect (socket, results.begin (), results.end ());
    string target = hex (to_string (v)); // dont wanna write a uri parser
    http::request<http::string_body> req{ http::verb::get, target, 11 };
    req.set (http::field::host, host);
    req.set (http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    http::write (socket, req);
    be::flat_buffer buffer;
    http::response_parser<http::string_body> res;
    http::read (socket, buffer, res);
    cout << res.get () << "\n";
    boost::system::error_code ec;
    socket.shutdown (tcp::socket::shutdown_both, ec);
  };
  long double avg_prev_time = 0;
  vector<uint8_t> v (20);
  for (int i = 0; i < 20; ++i)
    {
      map<uint8_t, long double> timings;
      cout << "Doing for i: " << i << "\n";
      for (int g = 0; g < 0x100; ++g)
        {
          v[i] = (uint8_t) (g & 0xff);
          long count = 0;
          for (int j = 0; j < tries; ++j)
            {
              auto start = high_resolution_clock::now ();
              send (v);
              auto stop = high_resolution_clock::now ();
              long double time
                  = duration_cast<nanoseconds> (stop - start).count ();
              if (time < avg_prev_time)
                {
                  continue;
                }
              ++count;
              timings[v[i]] += time;
            }
          timings[v[i]] /= count;
        }
      auto max_time = timings[0];

      vector<uint8_t> guesses;
      avg_prev_time = 0;
      for (auto [c, t] : timings)
        {
          max_time = max (max_time, t);
          avg_prev_time += t;
        }
      for (auto [c, t] : timings)
        {
          if (t == max_time)
            guesses.push_back (c);
        }
      avg_prev_time -= max_time;
      avg_prev_time /= 255.0;
      v[i] = guesses[0];

      cout << v << "\n";
    }
  cout << "Guess hmac: " << v << "\n";
  send (v);
}
