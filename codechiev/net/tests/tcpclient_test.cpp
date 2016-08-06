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
#include <base/Thread.hpp>
#include <boost/bind.hpp>
#include <errno.h>

using namespace codechiev::base;
using namespace codechiev::net;

int connNumber(1);
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
int main(int argc, const char * argv[]) {

    if((sizeof argv)>1)
    {
        connNumber=::atoi(argv[1]);
    }

    MultiClient client;
    client.setOnConnect(boost::bind(&onConnect,_1));
    client.setOnData(boost::bind(&onMessage,_1));
    client.setOnClose(boost::bind(&onClose,_1));
    Thread t("", boost::bind(&MultiClient::connectall, &client));
    t.start();

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
        }else if(c == '1')
        {
            const char *http = "GET / HTTP/1.1\r\n\
Accept: text/html, application/xhtml+xml, image/jxr, */*\r\n\
Accept-Language: zh-Hans-CN,zh-Hans;q=0.5\r\n\
User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64; Trident/7.0; rv:11.0) like Gecko\r\n\
Accept-Encoding: gzip, deflate\r\n\
Host: 127.0.0.1:9999\r\n\
Connection: Keep-Alive\r\n\r\n";
            client.writetoall(http);
        }else if(c == '2')
        {
            const char *http = "GET / HTTP/1.1\r\n\
Accept: text/html, application/xhtml+xml, image/jxr, */*\r\n\
Accept-Language: zh-Hans-CN,zh-Hans;q=0.5\r\n\
User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64; Trident/7.0; rv:11.0) like Gecko\r\n\
Accept-Encoding: gzip, deflate\r\n\
Host: 127.0.0.1:9999\r\n\
Connection: Keep-Alive\r";
            client.writetoall(http);
        }else if(c == '3')
        {
            const char *http = "\n\r\n";
            client.writetoall(http);
        }else if(c == '4')
        {
            const char *http = "GET / HTTP/1.1\r\n\
Accept: text/html, application/xhtml+xml, image/jxr, */*\r\n\
Accept-Language: zh-Hans-CN,zh-Hans;q=0.5\r\n\
User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64; Trident/7.0; rv:11.0) like Gecko\r\n\
Accept-Encoding: gzip, deflate\r\n\
Host: 127.0.0.1:9999\r\n\
Connection: Keep-Alive\r\n\r\nGET / HTTP/1.1\r\n\
Accept: text/html, application/xhtml+xml, image/jxr, */*\r\n\
Accept-Language: zh-Hans-CN,zh-Hans;q=0.5\r\n\
User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64; Trident/7.0; rv:11.0) like Gecko\r\n\
Accept-Encoding: gzip, deflate\r\n\
Host: 127.0.0.1:9999\r\n\
Connection: Keep-Alive\r";
            client.writetoall(http);
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
