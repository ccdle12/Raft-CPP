#include "tcp-client.h"

// Send is the interface implementation of Client. This will send data to a 
// specified server.
void TCPClient::Send()
{

    try 
    {
      Socket();    
      Bind();
      Connect();

      // NOTE: TEMP
      SendMsg();
    } catch (std::string e) {
      std::cout << e << std::endl;
      exit(1);
    }
}

void TCPClient::Socket()
{
  if ((sock_fd_ = socket(kIPV_, kProtocolType_, 0)) == -1)
  {
     throw ErrMsg("Unable to create socket"); 
  }
}

void TCPClient::Bind()
{
  server_addr_.sin_family = kIPV_; 
  server_addr_.sin_port = htons(port_); 

  if (inet_pton(kIPV_, address_.c_str(), &server_addr_.sin_addr) <= 0)
  {
    throw ErrMsg("invalid address");
  }
}

void TCPClient::Connect()
{
  if (connect(sock_fd_, (sockaddr*)&server_addr_, sizeof(server_addr_)))
  {
    throw ErrMsg("Connection attempt failed");
  }
}

void TCPClient::SendMsg()
{
  std::string msg = "Hello, this is the client";
  // int sent = send(sock_fd_, &msg, msg.length(), 0);
  printf("CLIENT: Sending msg to server\n");
  send(sock_fd_, &msg, msg.length(), 0);
  // if (sent == -1)
  // {
    // std::cout << "Failed to send messge" << std::endl;
  // }

  server_res_ = read(sock_fd_, buffer, 1024);
  printf("CLIENT: Received response from server: %s\n", buffer);
}
