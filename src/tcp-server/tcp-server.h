#pragma once
#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>
#include "network_server.h"
#include <string>
#include <poll.h>
#include <sys/types.h>
#include <unistd.h>

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
      int m_listening_socket_;
      uint8_t m_buffer_[1024];

      // Internal Methods.
      inline bool is_an_ipv(const int ip_version) const;

      void create_socket();
      inline bool is_socket_open(const int socket_fd) const;
      void set_socket_address_reusable(const int socket);
      void set_socket_to_non_blocking(const int socket);

      void create_server_hint();
      void bind_server_to_socket();
      inline bool is_server_bound_to_socket(const int bind_response) const;

      void listen_for_connections();

      void accept_connections();
      int create_client_file_descriptor();
      inline bool is_client_connection_open(const int client_fd);
      int read_to_buffer(const int file_descriptor);
};
