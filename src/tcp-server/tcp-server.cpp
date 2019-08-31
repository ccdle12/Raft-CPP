#include "tcp-server.h"

TCPServer::TCPServer(unsigned int port, int ip_version) : port_{port}
{
    if (!is_an_ipv(ip_version))
        throw std::runtime_error("ip_verison must be either: AF_INET (ipv4) or AF_INET6 (ipv6)");

    IPV_ = ip_version;
}; 

// Checks if a passed ip_version conforms to IPV4 or IPV6.
bool TCPServer::is_an_ipv(const int ip_version) const
{
    return ip_version == AF_INET || ip_version == AF_INET6;
} 

// Listen is the method implementation from NetworkServer interface. 
// It runs the  server by listening on the port specified in the constructor.
void TCPServer::Listen()
{
    try {
      create_socket();
      create_server_hint();
      bind_server_to_socket();
      listen_for_connections();
      accept_connections();
    } catch (std::string e) {
      std::cout << e << std::endl;
      exit(1);
    }
}

// Creates a non-blocking socket.
void TCPServer::create_socket()
{
    socket_file_descriptor_ = socket(IPV_, k_tcp_stream_, 0);
    if (!is_socket_open(socket_file_descriptor_))
    {
        throw std::runtime_error("Failed to create socket");
    }

    try {
        set_socket_to_non_blocking(socket_file_descriptor_);
    } catch (std::runtime_error exception) {
        throw exception;
    }
}

// Checks if a socket was succesfully created.
bool TCPServer::is_socket_open(const int socket_fd) const 
{
    return -1 != socket_fd;
}

// Sets the flags of a given socket file descriptor to non-blocking. 
void TCPServer::set_socket_to_non_blocking(int socket) 
{

    int file_control_flags = fcntl(socket, F_GETFL);
    if (-1 == file_control_flags)
    {
        throw std::runtime_error("Failed to get socket flags");
    }

    if (-1 == fcntl(socket, F_SETFL, file_control_flags | O_NONBLOCK))
    {
        throw std::runtime_error("Failed to set non-blocking flag for socket");
    }
}

// Assigns the rules for reaching the socket server.
// INADDR_ANY = accessible by any address.
void TCPServer::create_server_hint()
{
  hint_.sin_family = IPV_;
  hint_.sin_port = htons(port_);
  hint_.sin_addr.s_addr = INADDR_ANY;
  /* bzero(&TCPServer::hint_.sin_zero, 8); // Pad with zeroes. */
}

// Binds the server hint with the socket.
void TCPServer::bind_server_to_socket()
{
  int response = bind(socket_file_descriptor_, (sockaddr*)&hint_, sizeof(hint_));
  if (!is_server_bound_to_socket(response))
  {
    throw std::runtime_error("Failed to bind hint to socket file descriptor");
  }
}

// Checks if the server was bound to the socket.
bool TCPServer::is_server_bound_to_socket(const int bind_response) const 
{
    return -1 != bind_response;
}

// Listens on the socket for new connections.
void TCPServer::listen_for_connections()
{
    if ( -1 == (listen(socket_file_descriptor_, k_max_queue_size_)) )
    {
        throw std::runtime_error("Failed to listen on the socket.");
    }
}

// Accept will accept a new connection on the socket.
void TCPServer::accept_connections()
{
  while(true)
  {
    int client_file_descriptor = create_client_file_descriptor();
    if (is_client_connection_open(client_file_descriptor))
    {
      read_to_buffer(client_file_descriptor);

      if (-1 == send(client_file_descriptor, &buffer_, sizeof(buffer_), 0))
      {
        perror("SERVER: send failed");
        std::cout << "SERVER: Failed to send msg to client\n" << std::endl;
      }

      close(client_file_descriptor);
    }

    sleep(1);
  }
}

// Creates a client file descriptor when accepting a connection.
int TCPServer::create_client_file_descriptor()
{
    sockaddr_in client_socket_address;
    unsigned int socket_addr_size = sizeof(sockaddr_in);
    int client_file_descriptor = accept(
        socket_file_descriptor_, (sockaddr*)&client_socket_address, &socket_addr_size
    );
    
    return client_file_descriptor;
}

// Checks that a client connection is open.
bool TCPServer::is_client_connection_open(const int client_fd) 
{
    return -1 != client_fd; 
}

// Reads a message from a file_descriptor to the buffer.
void TCPServer::read_to_buffer(const int file_descriptor)
{
    read(file_descriptor, buffer_, 1024);
}

// Close will force close the tcp-server.
void TCPServer::Close() const
{
  close(socket_file_descriptor_);
}

TCPServer::~TCPServer()
{
  std::cout << "Closing file_descriptor" << std::endl;
  close(socket_file_descriptor_);
}
