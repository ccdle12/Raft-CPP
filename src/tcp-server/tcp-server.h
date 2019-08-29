#pragma once
#include <arpa/inet.h>
#include <string>
#include "../server.h"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>

class TCPServer : public Server {
    public:
      TCPServer(unsigned int port) : port_{port} {};
      ~TCPServer();
      void Listen() override;
      void Close() const override;

    private:
      // Aliases.
      typedef std::string ErrMsg;

      // Constants.
      // TODO: HOW CAN WE DECIDE BETWEEN IPV4 AND IPV6?
      const int kIPV_ = AF_INET;
      const int kProtocolType_ = SOCK_STREAM;
      
      // Member Variables.
      unsigned int port_;
      sockaddr_in hint_, client_;
      int sock_fd_, client_fd_, fcntl_flags_;
      char buffer_[1024];

      // Internal Methods.
      void Socket();
      void Hint();
      void Bind();
      void InitListen();
      void Accept();
};
