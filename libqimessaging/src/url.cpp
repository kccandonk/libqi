/*
**  Copyright (C) 2012 Aldebaran Robotics
**  See COPYING for the license
*/
#include <qimessaging/url.hpp>
#include <sstream>

namespace qi {


static void split_me(const std::string &url, unsigned short &_port, std::string &_host, std::string &_protocol)
{
  std::string    protocol;
  std::string    host;
  unsigned short port;

  size_t begin = 0;
  size_t end   = 0;
  end = url.find(":");
  if (end == std::string::npos)
    return;
  protocol = url.substr(begin, end);

  if (protocol.empty())
    return;

  begin = end + 3;
  end = url.find_last_of(":");
  if (end == std::string::npos || end < begin)
    return;
  host = url.substr(begin, end - begin);
  begin = end + 1;
  std::stringstream ss(url.substr(begin));
  ss >> port;

  _port = port;
  _host = host;
  _protocol = protocol;
}

Url::Url(const std::string &url)
  : _url(url)
  , _port(0)
  , _host("")
  , _protocol()
  , _reserved(0)
{
  split_me(_url, _port, _host, _protocol);
}

Url::Url()
  : _url()
  , _port(0)
  , _host()
  , _protocol()
  , _reserved(0)
{
}

Url::Url(const char *url)
  : _url(url)
  , _port(0)
  , _host()
  , _protocol()
  , _reserved(0)
{
  split_me(_url, _port, _host, _protocol);
}

Url::Url(const qi::Url& url)
  : _url(url._url)
  , _port(url._port)
  , _host(url._host)
  , _protocol(url._protocol)
  , _reserved(0)
{
}

//Url &Url::operator=(const std::string& rhs)
//{
//  _url = rhs;
//  split_me(_url, _port, _host, _protocol);
//  return *this;
//}

bool Url::isValid() const {
  return !_protocol.empty();
}

}

