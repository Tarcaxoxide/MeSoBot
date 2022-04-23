#include <main.hpp> /* header file for this c++ file */
#include <Misskey_Wrapper.hpp> /* Misskey_Wrapper::MisskeyBot_cl; this is network interface for communicating with misskey server. */ 
#include <iostream> /* printing to terminal for debugging */
#include <chrono>
#include <thread>

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <tools.hpp> /* basic tools for string formating */
#include <Credentials.hpp> /* File containing credentials for communicating with the misskey server */
#include <MeSoBot.hpp> /* header file for the bot itself */


namespace PROGRAM_NAME{
    using namespace std::chrono_literals;
    size_t saySomethingRandomTriggerTimer=0; // the timer for the normal random text
    size_t saySomethingRandomTrigger=500;   // the trigger time for the normal text
    size_t minsaySomethingRandomTrigger=100; // min/max values for new trigger times
    size_t maxsaySomethingRandomTrigger=500; // min/max values for new trigger times


    int main(Arguments_t Arguments){

        srand (time(NULL));
        Misskey_Wrapper::MisskeyBot_cl Interface(Credentials.website_url,Credentials.bot_acct,Credentials.bot_id);
        Misskey_Wrapper::RequestBody_st MSG; // message object used by the interface to create and receive notes.
        MSG.clear();//Clear the message object of any junk data it may contain.
        MSG.Data.ApiKey=Credentials.Api_Token; // set the token so the interface can authenticate with the misskey server.
        
        MeSoBot_cl Bot; // creating an instance of the bot;
        Bot.load();

        saySomethingRandomTrigger=(rand() % maxsaySomethingRandomTrigger)+minsaySomethingRandomTrigger;
        
        std::string old_id,old_text; // string variables for helping stop cyclictic issues (replying to the same message repeatedly till the end of time)

        int64_t average_sim=0;
        while(true){
            std::this_thread::sleep_for(500ms); // delay 500ms so we don't overwelm the misskey server.
            Interface.notes.global_timeline(MSG);// grab note from global timeline.
            if(MSG.Data.id != old_id){
                std::string mention="";
                std::string A_Text="",B_Text="";
                {// Handle the normal replies
                    for(size_t i=0;i<MSG.Data.mentions_text.size();i++){
                        mention=MSG.Data.mentions_text[i];
                        if(Interface == mention && MSG.Data.userId != Interface.Id()){
                            
                            for(size_t a=mention.size()+1;a<MSG.Data.text.size();a++){
                                A_Text+=MSG.Data.text[a];
                            }
                            if(MSG.Data.replyId.size() > 1){
                                std::string Restore=MSG.Data.id;
                                MSG.Data.noteId=MSG.Data.replyId;
                                Interface.notes.show(MSG);
                                for(size_t a=mention.size()+1;a<MSG.Data.text.size();a++){
                                    B_Text+=MSG.Data.text[a];
                                }
                                MSG.Data.id=Restore;
                            }else{
                                B_Text=A_Text;
                            }
                            
                            MSG.Data.replyId=MSG.Data.id;
                            MSG.Data.text=Bot.reply(A_Text,B_Text);
                            Interface.notes.create(MSG);
                        }
                    }
                }// Handle the normal replies
                if(A_Text == "" && B_Text == "" && MSG.Data.userId != Interface.Id()){
                    std::string Iid=MSG.Data.id;
                    A_Text=MSG.Data.text;
                    if(MSG.Data.replyId.size() > 1){
                        MSG.Data.noteId=MSG.Data.replyId;
                        Interface.notes.show(MSG);
                        B_Text+=MSG.Data.text;
                    }
                    std::string B_Text_fixed,A_Text_fixed;
                    to_lower(B_Text);
                    to_lower(A_Text);
                    bool tb=false;
                    for(size_t i=0;i<B_Text.size();i++){
                        if(B_Text[i] == '@')tb=true;
                        if(B_Text[i] == ' ')tb=false;
                        if(!tb)B_Text_fixed+=B_Text[i];
                    }
                    tb=false;
                    for(size_t i=0;i<A_Text.size();i++){
                        if(B_Text[i] == '@')tb=true;
                        if(B_Text[i] == ' ')tb=false;
                        if(!tb)A_Text_fixed+=A_Text[i];
                    }
                    Bot.learn(B_Text_fixed,A_Text_fixed);
                    MSG.Data.id=Iid;
                    if(B_Text_fixed.size() == 0){
                        std::string its="";
                        bool itz=false;
                        for(size_t i=0;i<A_Text_fixed.size();i++){
                            if(A_Text_fixed[i] == '@')itz=true;
                            if(A_Text_fixed[i] == ' ')itz=false;
                            if(!itz){its+=A_Text_fixed[i];}
                        }
                        if(its.size() < 2)continue;
                        Bot.learn("\\[RANDOM]\\",its);
                    }
                }
            }

            { // end of message sets
                old_id=MSG.Data.id;
                MSG.clear();
            }
            saySomethingRandomTriggerTimer++;
            if(saySomethingRandomTriggerTimer >= saySomethingRandomTrigger){
                saySomethingRandomTrigger=(rand() % maxsaySomethingRandomTrigger)+minsaySomethingRandomTrigger;
                saySomethingRandomTriggerTimer=0;
                MSG.Data.replyId=MSG.Data.id;
                std::string Said=Bot.reply("\\[RANDOM]\\","");
                MSG.Data.text=Said;
                MSG.Data.cw="";
                Interface.notes.create(MSG);
            }
        }
        
        return 0;
    }
};