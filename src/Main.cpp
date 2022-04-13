#define PY_SSIZE_T_CLEAN
#include<python3.9/Python.h>
#include<Main.hpp>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>
#include<thread>
#include "Credentials.h"

namespace pring{
    void MessageReceive(pring::Networking::Connection_st* Client){
        static int toggle=0;
        static std::string instance_name;
        static std::string user_name;
        static std::string author_name;
        std::string message=Client->Read();

        if(message.size() < 1)return;
        if(message == "init"){
            std::cout << "##Init of Message##" << std::endl;
            Client->Write((char*)"OK");
            toggle=0;
            return;
        }

        switch(toggle){
            case 0:{
                toggle=1;
                instance_name=message;
                Client->Write((char*)"instance_name");
            }return;
            case 1:{
                toggle=2;
                user_name=message;
                Client->Write((char*)"user_name");
            }return;
            case 2:{
                toggle=3;
                author_name=user_name=message;
                Client->Write((char*)"author_name");
            }return;
            case 3:{
                toggle=0;
                std::cout << "@" << user_name << "@" << instance_name << ":" << message << std::endl;
                if(author_name == "MeSoBot"){
                    Client->Write((char*)"NULL");
                }else{
                    Client->Write((char*)"Hello, this is this is a test.");
                }
                
            }return;
        }
        
        Client->Write((char*)"NULL");
    }


    int main(pring::Typez::Arguments_t Arguments){
    int port=54000;
    if(Arguments.size() > 1){
        port=std::stoi(Arguments[1]);
    }
    pring::Networking::NetworkManager_ct NetworkingManager(&MessageReceive,AF_INET,SOCK_STREAM,0,LOCAL_LOOPBACK_PORT,LOCAL_LOOPBACK_IP);

    NetworkingManager.WaitForClient();
    NetworkingManager.CloseHost();

    while(true){
        NetworkingManager.GetMsgs();
    }
    return 0;}
};




namespace MiWW{
    struct G{ 
        std::string _exec;
        std::string ip;
        int port;
        std::string uri;
        std::string token;
        std::vector<std::string> Channels;
        std::string Name;
    }G;
    void Botset(std::string ip,int port,std::string uri,std::string token,std::vector<std::string> Channels,std::string Name){
        std::string _exec;
        G.ip=ip;
        G.port=port;
        G.uri=uri;
        G.token=token;
        G.Channels=Channels;
        G.Name=Name;
    // Imports
        _exec+="import socket";_exec+="\n";
        _exec+="import asyncio";_exec+="\n";
        _exec+="from mi import *";_exec+="\n";
        _exec+="import time";_exec+="\n";
    // Delay for a few seconds for the server to start
        _exec+="time.sleep(10)";_exec+="\n";
    // Setup for communicating with the bot's c++ code
        _exec+="IP='";_exec+=ip;_exec+="'";_exec+="\n";
        _exec+="PORT=";_exec+=std::to_string(port);_exec+="\n";
        _exec+="ADDR=(IP,PORT)";_exec+="\n";
        _exec+="print(f'Connecting to socket {ADDR}...')";_exec+="\n";
        _exec+="zclient = socket.socket(socket.AF_INET,socket.SOCK_STREAM)";_exec+="\n";
        _exec+="zclient.connect(ADDR)";_exec+="\n";
    //  Send
        _exec+="async def send(msg):";_exec+="\n";
        _exec+="    RETURN_MESSAGE='?'";_exec+="\n";
        _exec+="    zclient.send(msg.encode('utf-8'))";_exec+="\n";
        _exec+="    ret = zclient.recv(4096)";_exec+="\n";
        _exec+="    RETURN_MESSAGE=str(ret.decode('utf-8'))";_exec+="\n";
        _exec+="    print(f'?sending?{RETURN_MESSAGE}?sending?')";_exec+="\n";
        _exec+="    return RETURN_MESSAGE;";_exec+="\n";
    // Bot Setup
        _exec+="uri='";_exec+=uri;_exec+="'";_exec+="\n";
        _exec+="token='";_exec+=token;_exec+="'";_exec+="\n";
        _exec+="Channels=[";
        for(size_t i=0;i<Channels.size();i++){
            _exec+="'";
            _exec+=Channels[i];
            _exec+="'";
            if(i != (Channels.size()-1))_exec+=",";
        }
        _exec+="]";_exec+="\n";
    // Bot class
        _exec+="class TheBot(commands.Bot):";_exec+="\n";
        _exec+="    def __init__(self):";_exec+="\n";
        _exec+="        super().__init__()";_exec+="\n";
        _exec+="    @tasks.loop(60)";_exec+="\n";
        _exec+="    async def task(self):";_exec+="\n";
        _exec+="        print('loop')";_exec+="\n";
        _exec+="    async def on_ready(self, ws):";_exec+="\n";
        _exec+="        print('work on my machine')";_exec+="\n";
        _exec+="        await Router(ws).connect_channel(Channels)";_exec+="\n";
        _exec+="        self.task.start()";_exec+="\n";

//        _exec+="        res = await self.client.note.send('hello world')";_exec+="\n";
//        _exec+="        print(res.content)";_exec+="\n";

        _exec+="        self.task.stop()";_exec+="\n";
        _exec+="    async def on_message(self, note: Note):";_exec+="\n";
        _exec+="        await send('init')";_exec+="\n";
        _exec+="        instance_name = note.author.instance.name if note.author.instance else 'local'";_exec+="\n";
        _exec+="        username = note.author.nickname or note.author.name";_exec+="\n";
        
        _exec+="        rets = await send(f'{instance_name}')";_exec+="\n";//first we send the instance name
        _exec+="        if str(f'{rets}').strip() != str('instance_name'):";_exec+="\n";
        _exec+="            print(f'ERROR: \\'{rets}\\' not instance_name');";_exec+="\n";

        _exec+="        rets = await send(f'{username}')";_exec+="\n";//than we send the user name
        _exec+="        if str(rets).strip() != str('user_name'):";_exec+="\n";
        _exec+="            print(f'ERROR: \\'{rets}\\' not user_name');";_exec+="\n";

        _exec+="        rets = await send(f'{note.author.name}')";_exec+="\n";//than we send the author name
        _exec+="        if str(rets).strip() != str('author_name'):";_exec+="\n";
        _exec+="            print(f'ERROR: \\'{rets}\\' not author_name');";_exec+="\n";

        _exec+="        if note.author.name != 'MeSoBot':";_exec+="\n";
        _exec+="            rs = await send(f'{note.content}')";_exec+="\n";
        _exec+="            res = await note.reply(rs)";_exec+="\n";
        _exec+="            print(str(res))";_exec+="\n";



    // execute(on the python side)
        _exec+="asyncio.run(TheBot().start(uri, token))";_exec+="\n";
        G._exec=_exec;
    }
    void Botrun(){
    // Setup variables and python interpreter object 
        wchar_t* PythonInterpreter = Py_DecodeLocale(G.Name.c_str(),NULL);
        if(PythonInterpreter == NULL){
            perror("Failed to setup python interpreter object");
            exit(1);
        }
        Py_SetProgramName(PythonInterpreter);
        Py_Initialize();
    // execute(on the c++ side)
        PyRun_SimpleString(G._exec.c_str());
    // Cleanup
        if(Py_FinalizeEx() < 0){
            exit(120);
        }
        PyMem_RawFree(PythonInterpreter);
    }
};

int main(int argument_count,char** argument_values){
    pring::Typez::Arguments_t Arguments;
    for(int Ai=0;Ai<argument_count;Ai++){
        Arguments.push_back(argument_values[Ai]);
    }
    MiWW::Botset(LOCAL_LOOPBACK_IP,LOCAL_LOOPBACK_PORT,SITE_URI,SITE_TOKEN,{"local"},Arguments[0]);
    std::thread BotWorker(MiWW::Botrun);
    pring::main(Arguments);
}