#include<Mi.hpp>


namespace Mi{
    Mi_C::Mi_C(Lython::args_t Arguments,std::string uri,std::string token,std::vector<std::string> Channels)
    :lython(Arguments){
        for(size_t i=0;i<Channels.size();i++){
            this->Channels.push_back(Channels[i]);
        }
        this->uri=uri;
        this->token=token;
        lython.Import("asyncio");
        lython.Import("mi","*");
    }
    void Mi_C::SetUp(){
        _exec+="uri =\"";
        _exec+=uri;
        _exec+="\"\n";
        _exec+="token=\"";
        _exec+=token;
        _exec+="\"\n";
        _exec+="Channels=[";
        for(size_t i=0;i<Channels.size();i++){
            _exec+="\"";
            _exec+=Channels[i];
            _exec+="\"";
            if((Channels.size()-1) != i){
                _exec+=",";
            }
        }
        _exec+="]\n";
        _exec+="class MyBot(commands.Bot):";_exec+="\n";
        _exec+="    def __init__(self):";_exec+="\n";
        _exec+="        super().__init__()";_exec+="\n";
        _exec+="    @tasks.loop(60)";_exec+="\n";
        _exec+="    async def task(self):";_exec+="\n";
        _exec+="        print(\"I will loop\")";_exec+="\n";
        _exec+="    async def on_ready(self, ws):";_exec+="\n";
        _exec+="        print(\"work on my machine\")";_exec+="\n";
        _exec+="        await Router(ws).connect_channel(Channels) ";_exec+="\n";
        _exec+="        self.task.start()";_exec+="\n";
        _exec+="        res = await self.client.note.send('hello world')";_exec+="\n";
        _exec+="        print(res.content)";_exec+="\n";
        _exec+="        self.task.stop()";_exec+="\n";
        _exec+="    async def on_message(self, note: Note):";_exec+="\n";
        _exec+="        instance_name = note.author.instance.name if note.author.instance else 'local'";_exec+="\n";
        _exec+="        username = note.author.nickname or note.author.name";_exec+="\n";
//        _exec+="        ResultFromCPP = CPP_Function(username,instance_name,note.content)";_exec+="\n"; //<<How to implement this?
        _exec+="        if note.renote is None:";_exec+="\n";
        _exec+="            print(f'{instance_name} | {username}: {note.content}')";_exec+="\n";
        _exec+="        else:";_exec+="\n";
        _exec+="            renote_name = note.renote.user.name";_exec+="\n";
        _exec+="            print(f'{instance_name} | {username}: {note.content}    {renote_name}: {note.renote.content}')";_exec+="\n";
    }
    void Mi_C::Run(){
        _exec+="asyncio.run(MyBot().start(uri, token))";_exec+="\n";
        lython.Run(_exec);
    }
};