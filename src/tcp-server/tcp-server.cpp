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
    if ((sock_fd_ = socket(kIPV_, kProtocolType_, 0)) == -1)
    {
        throw ErrMsg("Unable to create socket");
    }

    if ((fcntl_flags_ = fcntl(sock_fd_, F_GETFL) == -1))
    {
      throw ErrMsg("Unable to set fcntl flags");
    }


    if ((fcntl(sock_fd_, F_SETFL, fcntl_flags_ | O_NONBLOCK) == -1))
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
  if ((bind(sock_fd_, (sockaddr*)&hint_, sizeof(hint_)) == -1))
  {
    throw ErrMsg("Unable to bind hint to socket file descriptor");
  }
}

// InitListen will start the server to listen for connections.
void TCPServer::InitListen()
{
    // TODO: Set or use a maximum queue size, currently placeholder = 5;
    if ((listen(sock_fd_, 5)) == -1)
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
    if ((client_fd_ = accept(sock_fd_, (sockaddr*)&client_, &len) == -1))
    {
      if (errno == EWOULDBLOCK)
      {
        printf("No pending connections; sleeping for one second.\n");
        sleep(1);
      } else {
        throw ErrMsg("Error when accepting connection");
      }
    } else {
      // TODO: PLACEHOLDER
      // Echo a message back to the client.
      std::string msg = "Hello";
      int sent = send(client_fd_, &msg, msg.length(), 0);
      if (sent == -1)
      {
        std::cout << "Failed to send msg to client" << std::endl;
      } else {
        // NOTE: DEBUGGING
        printf("Sent %d bytes to client: %s\n", sent, inet_ntoa(client_.sin_addr));
        std::cout << "Message sent was: " << msg << std::endl;
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

