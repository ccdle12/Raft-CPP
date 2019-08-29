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
   const int kProtocolType_ = SOCK_STREAM;

   // Member Variables.
   unsigned int port_;
   /* int sock_fd_, server_res_; */
   int server_res_;
   sockaddr_in server_addr_;
   char buffer_[1024];
   std::string address_;

   // Socket Variables.
   int socket_file_descriptor_;

   // Internal Methods.
   bool is_an_ipv(const int ip_version) const;
   void create_socket();
   bool is_socket_open(const int socket_fd) const;
   void Bind();
   void Connect();

   // NOTE: TEMP
   void SendMsg(const std::string& msg);
};
