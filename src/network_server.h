class NetworkServer {
  public:
    virtual void Listen() = 0;
    virtual void Close() const = 0;
};
