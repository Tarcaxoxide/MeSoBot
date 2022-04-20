#include<main.hpp>
#include<Misskey_Wrapper.hpp>
#include<iostream>


namespace PROGRAM_NAME{
    int main(Arguments_t Arguments){

        Misskey_Wrapper::MisskeyBot_cl Bot("https://www.loganjohndarylgraham.xyz");
        Misskey_Wrapper::RequestBody_st test;
        test.clear();test.Data.ApiKey="1SHZofptzVo5RI79eBwIDaOUfijycWLk";
        
        std::string old_id;
        while(true){
            Bot.notes.global_timeline(test);
            test.Data.localOnly=true;
            bool Do=false;
            
            if(test.Data.id != old_id){
                for(size_t i=0;i<test.Data.mentions.size();i++){
                    if(test.Data.mentions[i] == "8yjrcrzohg")Do=true;
                }
                for(size_t i=0;i<test.Data.mentions.size();i++){
                    if(test.Data.userId == "8yjrcrzohg")Do=false;
                }
                old_id=test.Data.id;
                test.Data.replyId=test.Data.id;
                if(Do){
                    Bot.notes.create(test);
                }
                test.clear();
            }
        }
        
        return 0;
    }
};