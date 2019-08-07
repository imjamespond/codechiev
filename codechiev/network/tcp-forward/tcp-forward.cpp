#include <signal.h>
#include "tcp-forward.impl.hpp"

int main(int argc, const char **args)
{ 
  if (argc > 1)
  {
    host = args[1];
  }
  if (argc > 2)
  {
    port = args[2];
  }
  if (argc > 3)
  {
    servPort = args[3];
  }

  struct sigaction sa;
  sigaction(SIGPIPE, &sa, NULL);

  LOG_INFO << "host: " << host << ", port: " << port;

  TcpServer serv1(servPort, "0.0.0.0", false); //level triggered
  TcpClient client(false);                     //level triggered

  serv1.setCreateChannel(boost::bind(&createPipeChannel, _1));
  serv1.setOnConnect(boost::bind(&onConnect, _1, &serv1, &client));
  serv1.setOnRead(boost::bind(&onRead, _1, _2, _3, &serv1, &client));
  serv1.setEndReading(boost::bind(&onEndReading, _1, &serv1, &client));
  serv1.setEndWriting(boost::bind(&onEndWriting, _1, &serv1, &client));
  serv1.setOnClose(boost::bind(&onClose, _1, &serv1, &client));

  client.setCreateChannel(boost::bind(&createPipeChannel, _1));
  client.setOnConnect(boost::bind(&onCliConnect, _1, &client));
  client.setOnRead(boost::bind(&onCliRead, _1, _2, _3, &serv1, &client));
  client.setEndReading(boost::bind(&onCliEndReading, _1, &serv1, &client));
  client.setEndWriting(boost::bind(&onCliEndWriting, _1, &serv1, &client));
  client.setOnClose(boost::bind(&onCliClose, _1, &serv1, &client));

  {
    Channel::Loop loop;

    serv1.init(&loop);

    Time::SleepMillis(1000l);
    
    client.init(&loop);

    loop.loop();
  }

}
