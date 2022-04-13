#include<Networking.hpp>

namespace pring{
    namespace Networking{
        const size_t Connection_st::Socket::BufferSize;
        Connection_st::Connection_st(int domain,int type,int protocol,uint16_t port){
            Socket.Domain=domain;
            Socket.Type=type;
            Socket.Protocol=protocol;
            Socket.Good=true;
            Socket.Ready=false;
            Socket.Port=port;
            memset(Socket.Host,0,NI_MAXHOST);
            memset(Socket.Serv,0,NI_MAXSERV);
            ClearBuffer();
        }
        Connection_st::Connection_st(int domain,int HostConnection){
            Socket.Size = sizeof(Socket.hint);
            Socket.Connection = accept(HostConnection,(sockaddr*)&Socket.hint,(socklen_t *)&Socket.Size);
            Socket.Good=!(Socket.Connection < 0);
            memset(Socket.Host,0,NI_MAXHOST);
            memset(Socket.Serv,0,NI_MAXSERV);
            ClearBuffer();
            Socket.Domain=domain;

            //test get name
            int result = getnameinfo((sockaddr*)&Socket.hint,Socket.Size,Socket.Host,NI_MAXHOST,Socket.Serv,NI_MAXSERV,0);
            if(result){
                std::cout << Socket.Host << " Connected on " << Socket.Serv << std::endl;
            }else{
                inet_ntop(Socket.Domain,&Socket.hint.sin_addr,Socket.Host,NI_MAXHOST);
                std::cout << Socket.Host << " Connected on " << ntohs(Socket.hint.sin_port) << std::endl;
            }
        }
        bool Connection_st::CreateConnection(){
            Socket.Connection = socket(Socket.Domain,Socket.Type,Socket.Protocol);
            Socket.Good=!(Socket.Connection < 0);
            return Socket.Good;
        }
        bool Connection_st::BindConnection(){
            if(!Socket.Good)return false;
            switch(Socket.Domain){
                case AF_INET:{
                    Socket.hint.sin_family = Socket.Domain;
                    Socket.hint.sin_port = htons(Socket.Port);
                    inet_pton(Socket.Domain,Socket.IpAddress,&Socket.hint.sin_addr);
                    if(bind(Socket.Connection,(sockaddr*)&Socket.hint,sizeof(Socket.hint)) < 0)return false;
                    std::cout << "Bound to " << Socket.IpAddress << ":" << Socket.Port << std::endl;
                }break;
                default:{
                    return false;
                }break;
            }
            Socket.Ready=true;
            return true;
        }
        bool Connection_st::Good(){
            return (Socket.Good && Socket.Ready);
        }
        bool Connection_st::unlisten(){

            return true;
        }
        bool Connection_st::listen(){
            if(::listen(Socket.Connection,SOMAXCONN) < 0){
                Socket.Good=false;
                return false;
            }
            return true;
        }
        void Connection_st::ClearBuffer(){
            memset(Socket.Buffer,0,pring::Networking::Connection_st::Socket::BufferSize);
        }
        char* Connection_st::Read(){
            return (char*)Socket.Buffer;
        }
        void Connection_st::Recv(){
            ClearBuffer();
            Socket.BytesRecv = recv(Socket.Connection,Socket.Buffer,pring::Networking::Connection_st::Socket::BufferSize-1,0);

            std::cout << "::" << strlen(Socket.Buffer) << "/" << pring::Networking::Connection_st::Socket::BufferSize-1 << "::In ::\n" << "'" << Read() << "'" << std::endl;
        }
        void Connection_st::Write(char msg[pring::Networking::Connection_st::Socket::BufferSize]){
            size_t sz=0;
            while(*(msg+(sz++)));
            if(sz > 4096)return;
            size_t Csz = send(Socket.Connection,msg,sz,0);
            std::cout << "::" << Csz << "/" << sz << "::Out::\n" << "'" << msg << "'" << std::endl;
        }
        Connection_st::~Connection_st(){
            close(Socket.Connection);
            Socket.Ready=false;
            Socket.Good=false;
            Socket.Connection=0;
        }


        NetworkManager_ct::NetworkManager_ct(callback Main,int domain,int type,int protocol,int port,const char* IP)
        :Host_Connection(domain,type,protocol,port){
            this->Main=Main;
            size_t x=strlen(IP);
            for(size_t i=0;i<x;i++){
                Host_Connection.Socket.IpAddress[i]=IP[i];
            }
            if(!Host_Connection.CreateConnection()){
                perror("CreateConnection failed!");
                return;
            }
            if(!Host_Connection.BindConnection()){
                perror("BindConnection failed!");
                return;
            }
            if(!Host_Connection.listen()){
                perror("listen failed!");
                return;
            }
        }
        void NetworkManager_ct::WaitForClient(){
            Connection_st* Client = new Connection_st(Host_Connection.Socket.Domain,Host_Connection.Socket.Connection);

            if(Client->Socket.Good){
                Clients.push_back(Client);
            }
        }
        void NetworkManager_ct::CloseHost(){
            close(Host_Connection.Socket.Connection);
        }
        void NetworkManager_ct::GetMsgs(){
            for(size_t I=0;I<Clients.size();I++){
                Connection_st* client = Clients[I];
                client->Recv();
                if(client->Socket.BytesRecv <= 0){
                    (*client).~Connection_st();
                    Clients.erase(Clients.begin()+I);
                }else{
                    (*Main)(client);
                }
            }
        }
    };
};