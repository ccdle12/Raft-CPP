#include "tcp-client.h"

// Send is the interface implementation of Client. This will send data to a 
// specified server.
void TCPClient::Send(const std::string& msg)
{
    try 
    {
      Socket();    
      Bind();
      Connect();

      // NOTE: TEMP
      SendMsg(msg);
    } catch (std::string e) {
      std::cout << e << std::endl;
      exit(1);
    }
}

void TCPClient::Socket()
{
  if ( -1 == (sock_fd_ = socket(kIPV_, kProtocolType_, 0)))
  {
     throw ErrMsg("Unable to create socket\n"); 
  }
}

void TCPClient::Bind()
{
  server_addr_.sin_family = kIPV_; 
  server_addr_.sin_port = htons(port_); 

  if (inet_pton(kIPV_, address_.c_str(), &server_addr_.sin_addr) <= 0)
  {
    throw ErrMsg("invalid address\n");
  }
}

void TCPClient::Connect()
{
  if (connect(sock_fd_, (sockaddr*)&server_addr_, sizeof(server_addr_)))
  {
    throw ErrMsg("Connection attempt failed\n");
  }
}

void TCPClient::SendMsg(const std::string& msg)
{
  int sent = send(sock_fd_, &msg, msg.length(), 0);
  if (-1 == sent)
  {
    std::cout << "Failed to send messge\n" << std::endl;
  }

  server_res_ = read(sock_fd_, buffer_, 1024);
  printf("CLIENT: Received response from server: %s\n", buffer_);
}
