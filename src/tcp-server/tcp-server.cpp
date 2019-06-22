#include "tcp-server.h"

// Listen is the method implementation of the Server interface. It runs the 
// server by listening on the Port specified via construction.
void TCPServer::Listen()
{
    try {
      Socket();
      Hint();
      Bind();
      InitListen();
      Accept();
    } catch (std::string e) {
      std::cout << e << std::endl;
      exit(1);
    }
}

// Socket is an internal method to initialise a non-blocking socket.
void TCPServer::Socket()
{
    if ( -1 == (sock_fd_ = socket(kIPV_, kProtocolType_, 0)))
    {
        throw ErrMsg("Unable to create socket");
    }

    if ( (fcntl_flags_ = fcntl(sock_fd_, F_GETFL)) == -1)
    {
      throw ErrMsg("Unable to set fcntl flags");
    }


    if ( (fcntl(sock_fd_, F_SETFL, fcntl_flags_ | O_NONBLOCK)) == -1)
    {
      throw ErrMsg("Unable to set non-blocking flags to socket file descriptor");
    }
}

// InitHint is an internal method to initialise the hint.
void TCPServer::Hint()
{
  hint_.sin_family = kIPV_;
  hint_.sin_port = htons(port_);
  hint_.sin_addr.s_addr = INADDR_ANY;
  bzero(&TCPServer::hint_.sin_zero, 8); // Pad with zeroes.
}

// Bind will attach the IP and Port.
void TCPServer::Bind()
{
  if ( -1 == (bind(sock_fd_, (sockaddr*)&hint_, sizeof(hint_))))
  {
    throw ErrMsg("Unable to bind hint to socket file descriptor");
  }
}

// InitListen will start the server to listen for connections.
void TCPServer::InitListen()
{
    // TODO: Set or use a maximum queue size, currently placeholder = 5;
    if ( -1 == (listen(sock_fd_, 5)))
    {
      throw ErrMsg("Unable to listen on socket");
    }
}

// Accept will accept a new connection on the socket.
void TCPServer::Accept()
{
  unsigned int len = sizeof(sockaddr_in);
  
  while(true)
  {
    if ( -1 == (client_fd_ = accept(sock_fd_, (sockaddr*)&client_, &len)))
    {
      if (errno == EWOULDBLOCK)
      {
        printf("SERVER: No pending connections; sleeping for one second.\n");
        sleep(1);
      } else {
        perror("SERVER: accept error ");
        throw ErrMsg("SERVER: Error when accepting connection\n");
      }
    } else {
      // TODO: PLACEHOLDER
      // Move buffer as a private variable.
      int n = read(client_fd_, buffer_, 1024);
      std::cout << "Msg from client: " << buffer_ << "\n" << std::endl;

      int sent = send(client_fd_, &buffer_, sizeof(buffer_), 0);
      if (-1 == sent)
      {
        perror("SERVER: send failed");
        std::cout << "SERVER: Failed to send msg to client\n" << std::endl;
      }

      close(client_fd_);
    }
  }
}

// Close will force close the tcp-server.
void TCPServer::Close() const
{
  close(sock_fd_);
}

