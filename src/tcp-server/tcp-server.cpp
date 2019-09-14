#include "tcp-server.h"


TCPServer::TCPServer(unsigned int port, int ip_version) : port_{port}
{
    if (!is_an_ipv(ip_version))
        throw std::runtime_error("ip_verison must be either: AF_INET (ipv4) or AF_INET6 (ipv6)");

    IPV_ = ip_version;
}; 

// Checks if a passed ip_version conforms to IPV4 or IPV6.
inline bool TCPServer::is_an_ipv(const int ip_version) const
{
    return ip_version == AF_INET || ip_version == AF_INET6;
} 

// Listen is the method implementation from NetworkServer interface. 
// It runs the  server by listening on the port specified in the constructor.
void TCPServer::Listen()
{
    try {
      create_socket();
      set_socket_address_reusable(m_listening_socket_);
      set_socket_to_non_blocking(m_listening_socket_);
      create_server_hint();
      bind_server_to_socket();
      listen_for_connections();
      accept_connections();
    } catch (std::string e) {
      close(m_listening_socket_);
      std::cout << e << std::endl;
      exit(1);
    }
}

// Creates a non-blocking socket.
void TCPServer::create_socket()
{
    m_listening_socket_ = socket(IPV_, k_tcp_stream_, 0);
    if (!is_socket_open(m_listening_socket_))
    {
        throw std::runtime_error("Failed to create socket");
    }
}

// Checks if a socket was succesfully created.
inline bool TCPServer::is_socket_open(const int socket_fd) const 
{
    return -1 != socket_fd;
}

// Sets the socket address to be reusable... (ccdle12) WHY???
void TCPServer::set_socket_address_reusable(int socket)
{
    int on = 1;
    int response = setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
    if (-1 == response)
    {
      throw std::runtime_error("Failed to set socket to reusable.");
    }
}

// Sets the flags of a given socket file descriptor to non-blocking. 
void TCPServer::set_socket_to_non_blocking(int socket) 
{
    // Set socket to be non-blocking.
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
  bzero(&TCPServer::hint_.sin_zero, 8); // Pad with zeroes.
}

// Binds the server hint with the socket.
void TCPServer::bind_server_to_socket()
{
  int response = bind(m_listening_socket_, (sockaddr*)&hint_, sizeof(hint_));
  if (!is_server_bound_to_socket(response))
  {
    throw std::runtime_error("Failed to bind hint to socket file descriptor");
  }
}

// Checks if the server was bound to the socket.
inline bool TCPServer::is_server_bound_to_socket(const int bind_response) const 
{
    return -1 != bind_response;
}

// Listens on the socket for new connections.
void TCPServer::listen_for_connections()
{
    if ( -1 == (listen(m_listening_socket_, k_max_queue_size_)) )
    {
        throw std::runtime_error("Failed to listen on the socket.");
    }
}

// Accept will accept a new connection on the socket.
// TODO(ccdle12): DOES TWO THINGS, create a socket connection and sends a message.
// Spliy into x2 functions - create socket connection and send message.
void TCPServer::accept_connections()
{
    // Creates a poll descriptor set of size 200.
    struct pollfd fds[200];

    // Creates the listener on item 0.
    fds[0].fd = m_listening_socket_;
    fds[0].events = POLLIN;

    // Sets timeout.
    int timeout = 30000;

    // Number of file descriptors.
    int nfds = 1;

    // New Connection flag.
    int new_sd = -1;

    // End server flag.
    bool end_server = false;
    
    // 1. Loop that polls.
    // 2. Loop over nfds to find a readable connection. fds[i].
    do
    {
      // Poll the file descriptors set for activity.
      int response = poll(fds, nfds, timeout);

      if (response < 0)
      {
        perror(" poll() failed");
        break;
      }

      // Timeout expires, restart.
      if (0 == response)
      {
        std::cout << "Timeout has expired" << std::endl;
        break;
      }
      
      // A descriptor is readable, need to find which one it is.
      for (int i = 0; i < nfds; i++)
      {
          // No events on this file descriptor, skip.
          if (fds[i].revents == 0)
              continue;

          // If the event isn't a POLLIN, there was an error.
          if (fds[i].revents != POLLIN)
          {
            std::cout << " Error! revents = " << fds[i].revents << std::endl;
            break;
          }

          // The file descriptor index is the listening socket.
          if (fds[i].fd == m_listening_socket_)
          {
            std::cout << " Listening socket is readable" << std::endl;

              // Accept all queued connections on the socket.
              do
              {
                // Accepts the connection and assigns the response on the socket
                // connection to `new_sd`.
                new_sd = create_client_file_descriptor(); 

                // The connection failed since < 0.
                if (new_sd < 0)
                {
                  if (errno != EWOULDBLOCK)
                  {
                    perror(" accept() failed");
                    end_server = true;
                  }
                  break;
                } 

                // Add the new connection to pollfd structure.
                fds[nfds].fd = new_sd;
                fds[nfds].events = POLLIN;
                nfds++;
            
              } while(new_sd != -1);
          }
          else
          {
            // We've received a message.
            bool close_conn = false;

            do
            {
              // Read the message from the file descriptor to the buffer.
              if (read_to_buffer(fds[i].fd) < 0)
              {
                if (errno != EWOULDBLOCK)
                {
                  perror("  recv() failed");
                  close_conn = true;
                }
                break;
              } 

              if (response == 0)
              {
                std::cout << "Connection closed" << std::endl;
                close_conn = true;
                break;
              }

              /* read_to_buffer(response); */
              // Send the response back, which is the buffer contents.
              if (-1 == send(fds[i].fd, &m_buffer_, sizeof(m_buffer_), 0))
              {
                 perror("SERVER: send failed");
                 std::cout << "SERVER: Failed to send msg to client\n" << std::endl;
                 close_conn = true;
                 break;
              }

            } while(close_conn == false);
            
            // Close the socket connection and set it in the array to -1.
            if (close_conn)
            {
              close(fds[i].fd);
              fds[i].fd = -1;
            }
          }
    }

  } while(end_server == false);
}

// Creates a client file descriptor when accepting a connection.
int TCPServer::create_client_file_descriptor()
{
    sockaddr_in client_socket_address;
    unsigned int socket_addr_size = sizeof(sockaddr_in);
    int client_file_descriptor = accept(
        m_listening_socket_, (sockaddr*)&client_socket_address, &socket_addr_size
    );
    
    return client_file_descriptor;
}

// Checks that a client connection is open.
inline bool TCPServer::is_client_connection_open(const int client_fd) 
{
    return -1 != client_fd; 
}

// Reads a message from a file_descriptor to the buffer.
int TCPServer::read_to_buffer(const int file_descriptor)
{
    return recv(file_descriptor, m_buffer_, sizeof(m_buffer_), 0);
}

// Close will force close the tcp-server.
void TCPServer::Close() const
{
  close(m_listening_socket_);
}

TCPServer::~TCPServer()
{
  std::cout << "Closing file_descriptor" << std::endl;
  close(m_listening_socket_);
}
