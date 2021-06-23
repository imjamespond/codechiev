#ifndef Channel_hpp
#define Channel_hpp
 
#include <boost/function.hpp>

namespace learn_cpp {

class Channel {
public:
  typedef int t_fd;
  typedef boost::function<void(int)> t_func;
  
  explicit Channel(int); // not allow implicit_conversion
  ~Channel();
  static Channel* Create(t_fd);
  inline t_fd getFd() { return fd; }

  void close();
  void shutdown();
  void setNonblocking();
protected:
  const t_fd fd;
public:
  t_func func;
};

}

#endif 