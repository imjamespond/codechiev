#ifndef TcpServer_hpp
#define TcpServer_hpp

#include <boost/noncopyable.hpp>

namespace codechiev
{
namespace libev
{

class TcpServer : public boost::noncopyable
{
  public:
    TcpServer();
    ~TcpServer();

    void start();
};

} // namespace libev
} // namespace codechiev

#endif /* TcpServer_hpp */
