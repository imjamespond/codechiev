#ifndef PIPE_HPP
#define PIPE_HPP

#include "TcpEndpoint.hpp" 

#include <base/Error.hpp>
#include <base/Logger.hpp>
#include <boost/bind.hpp>

namespace codechiev
{
namespace net
{

class Pipe : public TcpEndpoint
{
public:
    Pipe();
    ~Pipe();

    void init(Channel::Loop *loop);
    void notify(const void *data = NULL, int len = 0);

private:
    Channel* p0;
    Channel* p1;
};

} // namespace net
} // namespace codechiev

#endif /* PIPE_HPP */