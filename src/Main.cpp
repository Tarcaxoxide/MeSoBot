#define PY_SSIZE_T_CLEAN
#include<python3.9/Python.h>
#include<Main.hpp>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>
#include<thread>

namespace pring{
    void MessageReceive(pring::Networking::Connection_st* Client){
        
        Client->Write((char*)Client->Read());
    }


    int main(pring::Typez::Arguments_t Arguments){
    int port=54000;
    if(Arguments.size() > 1){
        port=std::stoi(Arguments[1]);
    }
    pring::Networking::NetworkManager_ct NetworkingManager(&MessageReceive,AF_INET,SOCK_STREAM,0,5400,"127.1.0.1");

    NetworkingManager.WaitForClient();
    NetworkingManager.CloseHost();

    while(true){
        NetworkingManager.GetMsgs();
    }
    return 0;}
};

int main(int argument_count,char** argument_values){
    pring::Typez::Arguments_t Arguments;
    for(int Ai=0;Ai<argument_count;Ai++){
        Arguments.push_back(argument_values[Ai]);
    }
    
    pring::main(Arguments);
}