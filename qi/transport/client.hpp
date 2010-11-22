/*
** Author(s):
**  - Cedric GESTES <gestes@aldebaran-robotics.com>
**
** Copyright (C) 2010 Aldebaran Robotics
*/

#ifndef   __QI_TRANSPORT_CLIENT_HPP__
#define   __QI_TRANSPORT_CLIENT_HPP__

#include <qi/log.hpp>
#include <qi/transport/buffer.hpp>
#include <qi/transport/detail/client_impl.hpp>
#include <qi/transport/detail/zmq/zmq_client_impl.hpp>

namespace qi {
  namespace transport {

    template<typename TRANSPORT, typename BUFFER>
    class GenericTransportClient {
    public:
      GenericTransportClient() : isInitialized(false) {}

      bool connect(const std::string &address) {
        try {
          qisInfo << "* GenericTransportClient:connect " << address << std::endl;
          _client = new TRANSPORT(address);
          isInitialized = true;
        } catch(const std::exception& e) {
          qisDebug << "GenericClient failed to create client for address \""
            << address << "\" Reason: " << e.what() << std::endl;
        }
         return isInitialized;
      }

      void send(const BUFFER &def, BUFFER &result)
      {
        if (!isInitialized) {
          qisError << "Attempt to use an unitialized client." << std::endl;
        }
        _client->send(def, result);
      }

      bool isInitialized;
    protected:
      qi::transport::detail::ClientImpl<BUFFER> *_client;
    };

    typedef GenericTransportClient<
      qi::transport::detail::ZMQClientImpl,
      qi::transport::Buffer
    > ZMQTransportClient;

    typedef ZMQTransportClient Client;
  }
}

#endif // __QI_TRANSPORT_CLIENT_HPP__
