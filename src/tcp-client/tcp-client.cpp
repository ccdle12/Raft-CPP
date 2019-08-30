#include "tcp-client.h"

TCPClient::TCPClient(const std::string& address, unsigned int port, 
        int ip_version) : server_address_{address}, server_port_{port}
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
void TCPClient::Send(const uint8_t msg)
{
    try 
    {
      create_socket();    
      serialize_server_address();
      connect_to_server();
      send_bytes(msg);
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

// Converts the socket address given - IPV, port number, server address
// to network bytes. 
// inet_pton() returns 1 if address was successfully serialized to network 
// byte order.
int TCPClient::initialize_socket_server_address(sockaddr_in *sock_addr) 
{
    sock_addr->sin_family = IPV_;
    sock_addr->sin_port = htons(server_port_); 

    return inet_pton(IPV_, server_address_.c_str(), &sock_addr->sin_addr);
}

// Checks if a socket server address was serialized successfully.
bool TCPClient::is_socket_address_serialized(const int socket_address_result) const
{
    return 1 == socket_address_result;
}

// Connects to the server.
void TCPClient::connect_to_server()
{
  if (connect(socket_file_descriptor_, (sockaddr*)&socket_server_address_, sizeof(socket_server_address_)))
  {
    throw std::runtime_error("Connection attempt to the server failed.");
  }
}

void TCPClient::send_bytes(const uint8_t msg)
{
  int response = send(socket_file_descriptor_, &msg, sizeof(msg), 0);
  if (!is_message_sent(response))
  {
    throw std::runtime_error("Connection attempt to the server failed.");
  }

  server_response_ = read(socket_file_descriptor_, buffer_, 1024);
}

bool TCPClient::is_message_sent(int message_response) const
{
    return 1 == message_response;
}

const uint8_t* TCPClient::get_buffer() const
{
  return buffer_;
}

TCPClient::~TCPClient()
{
  std::cout << "Closing file descriptor" << std::endl;
  close(socket_file_descriptor_);
}
