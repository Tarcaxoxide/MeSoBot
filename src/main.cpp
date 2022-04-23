#include <main.hpp>
#include <Misskey_Wrapper.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <cctype>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <tools.hpp>
#include <LangaugeProcessing.hpp>
#include <Credentials.hpp>


namespace PROGRAM_NAME{
    using namespace std::chrono_literals;
    struct ResponseMatrix_st{Sentence_st Key;Sentence_st Value;};
    size_t saySomethingRandomTriggerTimer=0; // the timer for the normal random text
    size_t saySomethingRandomTrigger=500;   // the trigger time for the normal text
    size_t minsaySomethingRandomTrigger=100; // min/max values for new trigger times
    size_t maxsaySomethingRandomTrigger=500; // min/max values for new trigger times
    std::deque<ResponseMatrix_st> ResponseMatrix;


    void learn(std::string A,std::string B){
        if(A.size() < 2)return;
        if(B.size() < 2)return;
        to_lower(A);
        to_lower(B);
        Sentence_st _A(A);
        Sentence_st _B(B);
        size_t has=0;
        for(size_t a=0;a<ResponseMatrix.size();a++){
            if((ResponseMatrix[a].Key == _A) > 95)has=a+1;
        }

        //ResponseMatrix_st Entry{_A,_B};
        if(has){
            ResponseMatrix[has-1].Value.AddSentence(B);
            std::cout<<"added '"<<B<<"' to '"<<A<<"'"<<std::endl;
        }else{
            ResponseMatrix.push_back({_A,_B});
            std::cout<<"added '"<<A<<"'"<<std::endl;
            std::cout<<"added '"<<B<<"' to '"<<A<<"'"<<std::endl;
        }
    }

    std::string reply(std::string A_Text,std::string B_Text){
        to_lower(A_Text);
        to_lower(B_Text);
        std::string Result="";
        std::cout<<"received ["<<A_Text<<"] & "<<"["<<B_Text<<"]"<<std::endl;
        if(A_Text.size() > 2 && B_Text.size() > 2)learn(A_Text,B_Text);
        double Highest_similarity=0.0;
        ResponseMatrix_st* BestMatch=NULL;
        Sentence_st _A(A_Text);
        Sentence_st _B(B_Text);
        for(size_t i=0;i<ResponseMatrix.size();i++){
            if((ResponseMatrix[i].Key == _A) > Highest_similarity){
                Highest_similarity=(ResponseMatrix[i].Key == _A);
                BestMatch = &ResponseMatrix[i];
            }
        }
        std::cout<<"?1";
        if(BestMatch == NULL){
            std::cout<<"?2";
            Result=reply("\\[RANDOM]\\","");
            std::cout<<"?4"<<std::endl;
            //learn()
        }else{
            std::cout<<"?3";
            Result=BestMatch->Value.Random(3000);
            std::cout<<"?6"<<std::endl;
        }
        std::cout<<"replied ["<<Result<<"]"<<std::endl;
        return Result;
    }

    int main(Arguments_t Arguments){

        srand (time(NULL));
        Misskey_Wrapper::MisskeyBot_cl Bot(Credentials.website_url,Credentials.bot_acct,Credentials.bot_id);
        Misskey_Wrapper::RequestBody_st MSG;
        MSG.clear();MSG.Data.ApiKey=Credentials.Api_Token;
        

        saySomethingRandomTrigger=(rand() % maxsaySomethingRandomTrigger)+minsaySomethingRandomTrigger;
        
        std::string old_id,old_text;

        int64_t average_sim=0;
        while(true){
            std::this_thread::sleep_for(500ms);
            Bot.notes.global_timeline(MSG);
            if(MSG.Data.id != old_id){
                std::string mention="";
                std::string A_Text="",B_Text="";
                {// Handle the normal replies
                    for(size_t i=0;i<MSG.Data.mentions_text.size();i++){
                        mention=MSG.Data.mentions_text[i];
                        if(Bot == mention && MSG.Data.userId != Bot.Id()){
                            
                            for(size_t a=mention.size()+1;a<MSG.Data.text.size();a++){
                                A_Text+=MSG.Data.text[a];
                            }
                            if(MSG.Data.replyId.size() > 1){
                                std::string Restore=MSG.Data.id;
                                MSG.Data.noteId=MSG.Data.replyId;
                                Bot.notes.show(MSG);
                                for(size_t a=mention.size()+1;a<MSG.Data.text.size();a++){
                                    B_Text+=MSG.Data.text[a];
                                }
                                MSG.Data.id=Restore;
                            }else{
                                B_Text=A_Text;
                            }
                            
                            MSG.Data.replyId=MSG.Data.id;
                            MSG.Data.text=reply(A_Text,B_Text);
                            Bot.notes.create(MSG);
                        }
                    }
                }// Handle the normal replies
                if(A_Text == "" && B_Text == "" && MSG.Data.userId != Bot.Id()){
                    std::string Iid=MSG.Data.id;
                    A_Text=MSG.Data.text;
                    if(MSG.Data.replyId.size() > 1){
                        MSG.Data.noteId=MSG.Data.replyId;
                        Bot.notes.show(MSG);
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
                    learn(B_Text_fixed,A_Text_fixed);
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
                        learn("\\[RANDOM]\\",its);
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
                std::string Said=reply("\\[RANDOM]\\","");
                MSG.Data.text=Said;
                //MSG.Data.cw=std::string("something random:");
                Bot.notes.create(MSG);
                std::cout<<" /RandomPost\\ "<< "[\n"<<Said<<"\n]"<<std::endl;
                
            }
        }
        
        return 0;
    }
};