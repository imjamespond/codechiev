//
//  tcpserver_test.cpp
//  codechiev
//
//  Created by metasoft on 16/7/26.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#include <stdio.h>
#include <base/Logger.hpp>
#include <net/TcpEndpoint.hpp>
#include <net/TcpLengthCoder.h>
#include <base/BlockingQueue.hpp>
#include <base/Thread.hpp>
#include <boost/bind.hpp>
#include <errno.h>

using namespace codechiev::base;
using namespace codechiev::net;

int connNumber(1);
BlockingQueue<2> queue;
class MultiClient : public TcpClient
{
public:
    MultiClient():TcpClient("127.0.0.1", 9999){}

    void onClose(Channel* channel)
    {
        TcpEndpoint::onClose(channel);
        channels.erase(channel->getFd());
    }

    void writetoall(const char* msg)
    {
        for(channel_map::const_iterator it=channels.begin();
        it!=channels.end();
        it++)
        {
            channel_ptr chn = it->second;
            send(chn, msg);
        }
    }

    void connectall()
    {
        for(int i=0; i<connNumber; i++)
        {
            channel_ptr chn = connect();
            if(chn)
                channels[chn->getFd()]=chn;
        }

        this->start();
    }

    void closeall()
    {
        for(channel_map::const_iterator it=channels.begin();
        it!=channels.end();
        it++)
        {
            channel_ptr chn = it->second;
            this->shut(chn);
        }
    }

    channel_map channels;
};

void
onConnect(Channel* channel)
{
    LOG_DEBUG<<"onConnect\n fd:"<<channel->getFd()<<\
    /*", sendbuf size:"<<channel->getSendBufSize()<<\
   ", setbuf size"<<channel->setSendBufSize(0)<<\ */
    ", sendbuf size:"<<channel->getSendBufSize();
}
void
onMessage(const std::string& msg, int fd)
{
    LOG_DEBUG<<"onMessage:"<<msg;
}
void
onData(Channel* channel)
{
    LOG_DEBUG<<"onData:"<<channel->getReadBuf()->str();
    
    for(;;)
    {
        std::string msg;
        if(!TcpLengthCoder<4>::decode(channel, msg))
            break;
        queue.addJob(boost::bind(&onData, msg, channel->getFd()));
    }
}
void
onClose(Channel* channel)
{
    LOG_DEBUG<<"onClose fd:"<<channel->getFd();
}
int main(int argc, const char * argv[]) {

    if((sizeof argv)>1)
    {
        connNumber=::atoi(argv[1]);
    }

    MultiClient client;
    client.setOnConnect(boost::bind(&onConnect,_1));
    client.setOnData(boost::bind(&onData,_1));
    client.setOnClose(boost::bind(&onClose,_1));
    Thread t("", boost::bind(&MultiClient::connectall, &client));
    t.start();
    
    queue.commence();

    int c(0),i(0);
    char msg[128];
    ::memset(msg, 0, sizeof msg);
    do
    {
        c=getchar();
        if(c == 10)
        {
            client.writetoall(msg);
            i=0;
            ::memset(msg, 0, sizeof msg);
        }
        else
        {
            msg[i++]=c;
        }

    }while(c!='.');


    t.cancel();
    t.join();

    return 0;
}
