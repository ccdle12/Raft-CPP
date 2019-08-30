#include <string>

class NetworkClient {
  public:
    virtual void Send(const uint8_t msg) = 0;
    virtual const uint8_t* GetBuffer() const = 0;
};
