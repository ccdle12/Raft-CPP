#include "tcp-client.h"

TCPClient::TCPClient(const std::string& address, unsigned int port, int ip_version) : port_{port}, address_{address} 
{
    if (!is_an_ipv(ip_version))
        throw std::runtime_error("ip_verison must be either: AF_INET (ipv4) or AF_INET6 (ipv6)");

    IPV_ = ip_version;
}; 

// Checks if a passed ip_version conforms to IPV4 or IPV6.
bool TCPClient::is_an_ipv(const int ip_version) const
{
    return ip_version == AF_INET || ip_version == AF_INET6;
} 

// Send is the interface implementation of Client. This will send a message to a 
// specified server.
void TCPClient::Send(const std::string& msg)
{
    try 
    {
      create_socket();    
      serialize_server_address();
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
  socket_file_descriptor_ = socket(IPV_, k_tcp_stream_, 0);
  if (!is_socket_open(socket_file_descriptor_))
      throw std::runtime_error("Failed to create socket connection");
}

// Checks if a socket was succesfully created.
bool TCPClient::is_socket_open(const int socket_fd) const 
{
    return -1 != socket_fd;
}

// Serializes the IPV_ address to binary, specifically to network byte order.
void TCPClient::serialize_server_address()
{
  int result = initialize_socket_server_address(&socket_server_address_);
  if (!is_socket_address_serialized(result))
  {
    throw std::runtime_error("Failed to serialize server address");
  }
}

// Checks if a socket server address was serialized successfully.
bool TCPClient::is_socket_address_serialized(const int socket_address_result) const
{
    return socket_address_result == 1;
}

// Converts the socket address given - IPV, port number, server address
// to network bytes. 
// inet_pton() returns 1 if address was successfully serialized to network 
// byte order.
int TCPClient::initialize_socket_server_address(sockaddr_in *sock_addr) 
{
    sock_addr->sin_family = IPV_;
    sock_addr->sin_port = htons(port_); 

    return inet_pton(IPV_, address_.c_str(), &sock_addr->sin_addr);
}

void TCPClient::Connect()
{
  if (connect(socket_file_descriptor_, (sockaddr*)&socket_server_address_, sizeof(socket_server_address_)))
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
