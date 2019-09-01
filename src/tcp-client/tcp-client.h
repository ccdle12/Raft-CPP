#include <arpa/inet.h>
#include <iostream>
#include "network_client.h"
#include <string>
#include <unistd.h>

class TCPClient : public NetworkClient {
  public:
   TCPClient(const std::string& address, unsigned int port, int ip_version);
   ~TCPClient();
   void Send(const uint8_t msg) override; 
   const uint8_t* get_buffer() const override;

   // Member Variables.
   // The Internet Protocol Version for the address of the connection.
   int IPV_;
  
  private:
   // Constants.
   const int k_tcp_stream_ = SOCK_STREAM;

   // Member Variables.
   uint8_t buffer_[1024];

   // Socket Variables.
   int socket_file_descriptor_;

   // Server Variables.
   int server_response_;
   std::string server_address_;
   unsigned int server_port_;
   sockaddr_in socket_server_address_;

   // Internal Methods.
   bool is_an_ipv(const int ip_version) const;

   void create_socket();
   bool is_socket_open(const int socket_fd) const;

   void serialize_server_address();
   int initialize_socket_server_address(sockaddr_in *sock_addr);
   bool is_socket_address_serialized(const int socket_address_result) const;

   void connect_to_server();

   void send_bytes(const uint8_t msg);
   bool is_message_sent(int message_response) const;

};
