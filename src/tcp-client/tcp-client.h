#include <arpa/inet.h>
#include <unistd.h>
#include "../client.h"
#include <string>
#include <iostream>

class TCPClient : public Client {
  public:
   TCPClient(const std::string& address, unsigned int port, int ip_version);
   ~TCPClient();
   void Send(const std::string& msg) override; 
   const std::string& GetBuffer() const override;

   // Member Variables.
   // The Internet Protocol Version for the address of the connection.
   int IPV_;
  
  private:
   // Aliases.
   typedef std::string ErrMsg;

   // Constants.
   const int k_tcp_stream_ = SOCK_STREAM;

   // Member Variables.
   char buffer_[1024];

   // Socket Variables.
   int socket_file_descriptor_;

   // Server Variables.
   int server_res_;
   std::string address_;
   unsigned int port_;
   sockaddr_in socket_server_address_;

   // Internal Methods.
   bool is_an_ipv(const int ip_version) const;

   void create_socket();
   bool is_socket_open(const int socket_fd) const;

   void serialize_server_address();
   int initialize_socket_server_address(sockaddr_in *sock_addr);
   bool is_socket_address_serialized(const int socket_address_result) const;

   void connect_to_server();

   // NOTE: TEMP
   void SendMsg(const std::string& msg);
};
