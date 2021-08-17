#ifndef Channel_hpp
#define Channel_hpp

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace learn_cpp
{

  class Channel;
  typedef boost::shared_ptr<Channel> ChannelPtr;

  class Channel : public boost::enable_shared_from_this<Channel>
  {
  public:
    typedef int t_fd;
    typedef boost::function<void(const ChannelPtr &, int)> HandleFunc;

    ~Channel();
    static Channel *Create(t_fd);
    inline t_fd GetFd() { return fd; }
    inline ChannelPtr GetPtr() { return shared_from_this(); }
    inline void Reset() { if(self) self.reset(); }

    void Close();
    void Shutdown();
    void SetNonblocking();

  protected:
    explicit Channel(int); // not allow implicit_conversion

    const t_fd fd;
    ChannelPtr self;

  public:
    HandleFunc handle;
  };

}

#endif