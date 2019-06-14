#pragma once
#include <arpa/inet.h>
#include <string>
#include "../server.h"
#include <iostream>
#include <unistd.h>

class TCPServer : public Server {
    public:
      TCPServer(unsigned int port) : port_{port} {};
      void Listen() override;

    private:
      // Constants.
      // TODO: HOW CAN WE DECIDE BETWEEN IPV4 AND IPV6?
      const int kIPV_ = AF_INET;
      const int kProtocolType_ = SOCK_STREAM;
      
      // Member Variables.
      unsigned int port_;
      sockaddr_in hint_, client_;
      int sock_;
      int client_conn_;

      // Internal Methods.
      void Socket();
      void Hint();
      void Bind();
      void InitListen();
      void Accept();
};
