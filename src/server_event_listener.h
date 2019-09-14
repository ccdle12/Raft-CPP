#pragma once

class ServerEventListener {
  public:
    virtual void MsgReceived() const = 0;
};
