#include <string>

class Client {
  public:
    virtual void Send(const std::string& msg) = 0;
    virtual const std::string& GetBuffer() const = 0;
};
