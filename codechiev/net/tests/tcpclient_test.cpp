//
//  tcpserver_test.cpp
//  codechiev
//
//  Created by metasoft on 16/7/26.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#include <stdio.h>
#include <net/TcpEndpoint.hpp>
#include <net/Timer.hpp>
#include <base/AtomicNumber.h>
#include <base/Logger.hpp>
#include <base/Thread.hpp>
#include <base/Time.hpp>
#include <boost/bind.hpp>
#include <errno.h>

using namespace codechiev::base;
using namespace codechiev::net;

TimerQueue timerq;
AtomicNumber<int64_t> count(0);

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

    void multiConnect(int num)
    {
        for(int i=0; i<num; i++)
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
            this->shut(chn.get());
        }
    }

    channel_map channels;
};

void onConnect(Channel* channel)
{
    LOG_DEBUG<<"onConnect\n fd:"<<channel->getFd()<<\
    /*", sendbuf size:"<<channel->getSendBufSize()<<\
   ", setbuf size"<<channel->setSendBufSize(0)<<\ */
    ", sendbuf size:"<<channel->getSendBufSize();
}
void onMessage(Channel* channel)
{
    LOG_DEBUG<<"onMessage:"<<channel->getReadBuf()->str();
}
void onClose(Channel* channel)
{
    LOG_DEBUG<<"onClose fd:"<<channel->getFd();
}

MultiClient client;
void timerSend(const std::string &msg, int count)
{
    if(--count)
    {
        Time now=Time::Now();
        timerq.addTask(now.getMillis() + 50, boost::bind(&timerSend, msg, count));
        client.writetoall(msg.c_str());
    }
}

char msg[128];
int main(int argc, const char * argv[]) {
    
    setLoggerLevel(Logger::Info);
    
    int num(0);
    if((sizeof argv)>1)
    {
        num=::atoi(argv[1]);
    }

    client.setOnConnect(boost::bind(&onConnect,_1));
    client.setOnData(boost::bind(&onMessage,_1));
    client.setOnClose(boost::bind(&onClose,_1));
    Thread t("Client", boost::bind(&MultiClient::multiConnect, &client, num));
    t.start();
    
    Thread tt("Timer", boost::bind(&TimerQueue::commence, &timerq));
    tt.start();

    int c(0),i(0);
    ::memset(msg, 0, sizeof msg);
    do
    {
        c=getchar();
        if(c == 10)
        {
            Time now=Time::Now();
            std::string m(msg);
            timerq.addTask(now.getMillis() + 50, boost::bind(&timerSend, m, 999));
            i=0;
            ::memset(msg, 0, sizeof msg);
        }
        else
        {
            msg[i++]=c;
        }

    }while(c!='.');

    t.cancel();
    tt.cancel();
    t.join();
    tt.join();
    return 0;
}
