#include <signal.h>
#include "tcp-forward.impl.hpp"

void sigpipe_handler(int unused)
{
}

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

  struct sigaction st[] = {SIG_IGN};
  sigaction(SIGPIPE, st, NULL);

  LOG_INFO << "host: " << host << ", port: " << port;

  TcpServer serv1(servPort, "0.0.0.0");  
  TcpClient client = TcpClient();                      

  serv1.createChannel = (boost::bind(&createPipeChannel, _1));
  serv1.onConnect = (boost::bind(&onConnect, _1, &serv1, &client));
  serv1.onRead = (boost::bind(&onRead, _1, _2, _3, &serv1, &client));
  serv1.onReadEnd = (boost::bind(&onEndReading, _1, &serv1, &client));
  serv1.onWritten = (boost::bind(&onEndWriting, _1, &serv1, &client));
  serv1.onClose = (boost::bind(&onClose, _1, &serv1, &client));

  client.createChannel = (boost::bind(&createPipeChannel, _1));
  client.onConnect = (boost::bind(&onCliConnect, _1, &client));
  client.onRead = (boost::bind(&onCliRead, _1, _2, _3, &serv1, &client));
  client.onReadEnd = (boost::bind(&onCliEndReading, _1, &serv1, &client));
  client.onWritten = (boost::bind(&onCliEndWriting, _1, &serv1, &client));
  client.onClose = (boost::bind(&onCliClose, _1, &serv1, &client));

  {
    Channel::Loop loop;

    serv1.init(&loop);

    Time::SleepMillis(1000l);
    
    client.init(&loop);

    loop.loop();
  }

}
