#pragma once

#include<Typez.hpp>
#include<deque>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<iostream>

namespace pring{
    namespace Networking{
        struct Connection_st{
            struct Socket{
                int Domain,Type,Protocol;
                int Connection;
                bool Good;
                bool Ready;
                sockaddr_in hint;
                uint16_t Port;
                char IpAddress[16] {'0'};
                socklen_t Size;
                char Host[NI_MAXHOST];
                char Serv[NI_MAXSERV];
                static const size_t BufferSize=4096;
                int BytesRecv=0;
                char Buffer[BufferSize];
                }Socket;
            Connection_st(int domain,int type,int protocol,uint16_t port);
            Connection_st(int domain,int HostConnection);
            ~Connection_st();
            bool CreateConnection();
            bool BindConnection();
            bool unlisten();
            bool listen();
            bool Good();
            void ClearBuffer();
            char* Read();
            void Recv();
            void Write(char msg[pring::Networking::Connection_st::Socket::BufferSize]);
        };
        typedef void (*callback)(pring::Networking::Connection_st* Client);
        class NetworkManager_ct{
            Connection_st Host_Connection;
            std::deque<Connection_st*> Clients;
            callback Main;
            public:
            NetworkManager_ct(callback Main,int domain=AF_INET,int type=SOCK_STREAM,int protocol=0,int port=54001,const char* IP="0.0.0.0");
            void WaitForClient();
            void CloseHost();
            void GetMsgs();
        };
    };
};