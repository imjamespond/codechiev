
#include <libev/TcpServer.hpp>
#include <libev/Signal.hpp>

static void signal_cb(evutil_socket_t, short, void *);

int main(int argc, char **argv)
{

    codechiev::libev::TcpServer server(12345);
    codechiev::libev::Signal signal(server.base, &signal_cb, server.base);
    server.start();

    return 0;
}


static void
signal_cb(evutil_socket_t sig, short events, void *user_data)
{
    struct event_base *base = reinterpret_cast<struct event_base *>(user_data);
    struct timeval delay = {2, 0};

    printf("Caught an interrupt signal; exiting cleanly in two seconds.\n");

    event_base_loopexit(base, &delay);
}
