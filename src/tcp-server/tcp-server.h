#pragma once
#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <poll.h>
#include <sys/types.h>
#include <unistd.h>
#include "network_server.h"
#include "server_event_listener.h"

// TODO(ccdle12):
// - rename all member variables with `m_` prefix.
class TCPServer : public NetworkServer {
    public:
      TCPServer(unsigned int port, int ip_version, const ServerEventListener &server_listener);
      ~TCPServer();

      // NetworkServer interface implementations.
      void Listen() override;
      void Close() const override;
      void SendResponse(const int fd, uint8_t message) override;

    // Address version, IPV4 or IPV6.
    int IPV_;

    private:
      // Constants.
      const int kTCPStream = SOCK_STREAM;
      // TODO(ccdle12): temp max size is 5.
      // also set the size via an overload constructor.
      const int kMaxQueueSize = 5;
      
      // Member Variables.
      unsigned int port_;
      // TODO(ccdle12) do I need hint to be a member variable?
      sockaddr_in hint_;
      int listening_socket_;
      //TODO(ccdle12): remove buffer.
      uint8_t buffer_[1024];
      struct pollfd fds_[200]; /* Array of opened connections */

      // Event Listener Interfaces.
      // NOTE(ccdle12): A little bit dangerous as can be a nullptr.
      const ServerEventListener *server_listener_;

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
