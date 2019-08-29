#include "tcp-client.h"

TCPClient::TCPClient(const std::string& address, unsigned int port, int ip_version) : port_{port}, address_{address} 
{
    if (is_an_ipv(ip_version))
        throw std::runtime_error("ip_verison must be either: AF_INET (ipv4) or AF_INET6 (ipv6)");

    IPV_ = ip_version;
}; 

// Checks if a passed ip_version conforms to IPV4 or IPV6.
bool TCPClient::is_an_ipv(const int ip_version) const
{
    return ip_version != AF_INET && ip_version != AF_INET6;
} 

// Send is the interface implementation of Client. This will send a message to a 
// specified server.
void TCPClient::Send(const std::string& msg)
{
    try 
    {
      create_socket();    
      Bind();
      Connect();
      SendMsg(msg);
    } catch (std::string e) {
      std::cout << e << std::endl;
      exit(1);
    }
}

// Creates a socket using the standard socket() system call.
void TCPClient::create_socket()
{
  socket_file_descriptor_ = socket(IPV_, kProtocolType_, 0);
  if (!is_socket_open(socket_file_descriptor_))
      throw std::runtime_error("Failed to create socket connection");
}

// Checks if a socket was succesfully created.
bool TCPClient::is_socket_open(const int socket_fd) const 
{
    return -1 != socket_fd;
}

void TCPClient::Bind()
{
  server_addr_.sin_family = IPV_;
  server_addr_.sin_port = htons(port_); 

  if (inet_pton(IPV_, address_.c_str(), &server_addr_.sin_addr) <= 0)
  {
    throw ErrMsg("invalid address\n");
  }
}

void TCPClient::Connect()
{
  if (connect(socket_file_descriptor_, (sockaddr*)&server_addr_, sizeof(server_addr_)))
  {
    throw ErrMsg("Connection attempt failed\n");
  }
}

void TCPClient::SendMsg(const std::string& msg)
{
  int sent = send(socket_file_descriptor_, &msg, msg.length(), 0);
  if (-1 == sent)
  {
    std::cout << "Failed to send messge\n" << std::endl;
  }

  server_res_ = read(socket_file_descriptor_, buffer_, 1024);
  printf("CLIENT: Received response from server: %s\n", buffer_);
}

const std::string& TCPClient::GetBuffer() const
{
  return buffer_;
}


TCPClient::~TCPClient()
{
  std::cout << "Closing file descriptor" << std::endl;
  close(socket_file_descriptor_);
}
