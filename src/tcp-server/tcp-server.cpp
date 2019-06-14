#include "tcp-server.h"

// Listen is the method implementation of the Server interface. It takes a port
// number and runs the server to listen on the specified port.
void TCPServer::Listen()
{
    Socket();
    Hint();
    Bind();
    InitListen();
    Accept();
}

// Socket is an internal method to initialise the socket.
void TCPServer::Socket()
{
    if ((sock_ = socket(kIPV_, kProtocolType_, 0)) == -1)
    {
      // TODO: PLACEHOLDER
      throw 10;     
    }
}

// InitHint is an internal method to initialise the hint.
void TCPServer::Hint()
{
  hint_.sin_family = kIPV_;
  hint_.sin_port = htons(port_);

  // TODO: NEED TO FIX INADDR_ANY TO SOMETHING SAFER.
  hint_.sin_addr.s_addr = INADDR_ANY;
  bzero(&TCPServer::hint_.sin_zero, 8); // Pad with zeroes.
}

// Bind will attach the IP and Port.
void TCPServer::Bind()
{
  if ((bind(sock_, (sockaddr*)&hint_, sizeof(sockaddr_in)) == -1))
  {
    // TODO: PLACEHOLDER
    throw 11; 
  }
}

// InitListen will start the server to listen for connections.
void TCPServer::InitListen()
{
    // TODO: Set or use a maximum queue size, currently placeholder = 5;
    if ((listen(sock_, 5)) == -1)
    {
      // TODO: PLACEHOLDER
      throw 12;
    }
}

// Accept will accept a new connection on the socket.
void TCPServer::Accept()
{
  unsigned int len = sizeof(sockaddr_in);
  
  if ((client_conn_ = accept(sock_, (sockaddr*)&client_, &len) == -1)) 
  {
    // TODO: PLACEHOLDER
    throw 14;
  }

  // TODO: PLACEHOLDER
  // Echo a message back to the client.
  std::string msg = "Hello";
  int sent = send(client_conn_, &msg, msg.length(), 0);

  // NOTE: DEBUGGING
  printf("Sent %d bytes to client: %s\n", sent, inet_ntoa(client_.sin_addr));
  std::cout << "Message sent was: " << msg << std::endl;

  close(client_conn_);
}

