#pragma once

class ServerEventListener {
  public:
   virtual void MsgReceived(const int fd, const uint8_t *message) const = 0;
};
