#pragma once

class NetworkServer {
  public:
    virtual void Listen() = 0;
    virtual void Close() const = 0;
    virtual void SendResponse(const int fd, uint8_t message) = 0;
};
