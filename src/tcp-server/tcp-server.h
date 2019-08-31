#pragma once
#include <arpa/inet.h>
#include <string>
#include "network_server.h"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>

class TCPServer : public NetworkServer {
    public:
      TCPServer(unsigned int port, int ip_version);
      ~TCPServer();
      void Listen() override;
      void Close() const override;

    // Address version, IPV4 or IPV6.
    int IPV_;

    private:
      // Constants.
      const int k_tcp_stream_ = SOCK_STREAM;
      // TODO(ccdle12): temp max size is 5.
      // also set the size via an overload constructor.
      const int k_max_queue_size_ = 5;
      
      // Member Variables.
      unsigned int port_;
      sockaddr_in hint_;
      int socket_file_descriptor_;
      uint8_t buffer_[1024];

      // Internal Methods.
      bool is_an_ipv(const int ip_version) const;

      void create_socket();
      bool is_socket_open(const int socket_fd) const;
      void set_socket_to_non_blocking(int socket);

      void create_server_hint();
      void bind_server_to_socket();
      bool is_server_bound_to_socket(const int bind_response) const;

      void listen_for_connections();

      void accept_connections();
      int create_client_file_descriptor();
      bool is_client_connection_open(const int client_fd);
      void read_to_buffer(const int file_descriptor);
};
