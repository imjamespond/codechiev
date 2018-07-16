#include <stdio.h>

#include <base/Logger.hpp>
#include <base/Time.hpp>
#include <base/Thread.hpp>
#include <base/Keyboard.hpp>
#include <libev/TcpClient.hpp>

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

using namespace codechiev::base;
using namespace codechiev::libev;

TcpEndpoint::bufev_struct *BufEv(NULL);
Channel *__client_channel__(NULL);

int onConnect(Channel *channel)
{
    // LOG_INFO << "";
    __client_channel__ = channel;
    return 0;
}

int onClose(Channel *channel)
{
    LOG_INFO;
    return 0;
}

int onRead(Channel *channel)
{
    // std::string msg((char *)data, len);
    // LOG_INFO << "read:" << len << "," << msg;

    // client->write(bev, msg.c_str(), len);//within recursive locks
    return 0;
}

int onWrite(Channel *channel)
{
    // LOG_INFO << "";
    return 0;
}

void run_client(int argc, const char *argv[])
{
    const char *hostname = argc > 2 ? argv[2] : "127.0.0.1:12345";
    TcpClient client(hostname);

    client.onConnect = boost::bind(&onConnect, _1);
    client.onClose = boost::bind(&onClose, _1);
    client.onRead = boost::bind(&onRead, _1);
    client.onWrite = boost::bind(&onWrite, _1);

    int num = argc > 1 ? boost::lexical_cast<int>(argv[1]) : 10;
    for (int i = 0; i < num; i++)
    {
        client.connect();
    }
    client.start();
}

int main(int argc, const char *argv[])
{

    Thread threadCli("cli", boost::bind(&run_client, argc, argv));
    threadCli.start();

    while (1)
    {
        char buf[256];
        int code = keyboard::getchar();
        if (code == keycode::a)
        {
            ::memset(buf, 0, sizeof buf);
            // ::sprintf(buf, );
            for (int i = 0; i < 1000000; i++)
            {

                __client_channel__->send("Returns a reference to the character at specified location pos."
                                         " Bounds checking is performed, exception of type std::out_of_range will be thrown on invalid access");
            }
        }
    }

    threadCli.join();
    return 0;
}
