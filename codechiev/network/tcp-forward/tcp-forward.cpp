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

  LOG_INFO << "host: " << host << ", port: " << port;

  TcpServer serv1(servPort, "0.0.0.0", true); //avoid listen channel destroy before loop joined
  TcpClient client(true);

  serv1.setCreateChannel(boost::bind(&createPipeChannel, _1));
  serv1.setOnConnect(boost::bind(&onConnect, _1, &serv1, &client));
  serv1.setOnClose(boost::bind(&onClose, _1));

  client.setCreateChannel(boost::bind(&createPipeChannel, _1));
  client.setOnConnect(boost::bind(&onCliConnect, _1, &client));
  client.setOnClose(boost::bind(&onCliClose, _1));

  {
    TcpServer::Loop serv1Loop;
    TcpClient::Loop cliLoop;

    serv1.start(&serv1Loop);

    Time::SleepMillis(1000l);
    
    client.start(&cliLoop);
  }

}
