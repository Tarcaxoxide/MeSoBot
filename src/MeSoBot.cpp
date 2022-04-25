#include<MeSoBot.hpp>


namespace PROGRAM_NAME{
    void MeSoBot_cl::learn(std::string A,std::string B){
        if(A.size() < 2)return;
        if(B.size() < 2)return;
        if(A == B)return;
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
    std::string MeSoBot_cl::reply(std::string A_Text,std::string B_Text){
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
    
    void MeSoBot_cl::save(){
        std::filesystem::create_directory("Data/");
        std::ofstream Keys("Data/Keys.txt");
        std::ofstream Values("Data/Values.txt");
        for(size_t a=0;a<ResponseMatrix.size();a++){
            Keys<<ResponseMatrix[a].Key.to_string()<<std::endl;
            Values<<ResponseMatrix[a].Value.to_string()<<std::endl;
        }
        Keys.close();
        Values.close();
    }
    void MeSoBot_cl::load(){
        std::filesystem::create_directory("Data/");
        std::ifstream Keys("Data/Keys.txt");
        std::ifstream Values("Data/Values.txt");
        for(;!(Values.eof() || Keys.eof());){
            std::string Line_A;
            std::getline(Values,Line_A);
            std::string Line_B;
            std::getline(Keys,Line_B);
            learn(Line_B,Line_A);
        }
        Keys.close();
        Values.close();
    }
    std::string MeSoBot_cl::_ParseFileName(std::string FileName){
        std::string Result="";
        bool trg=false;
        for(size_t i=0;i<FileName.size();i++){
            if(trg)Result+=FileName[i];
            if(FileName[i] == '.')trg=true;
        }
        return Result;
    }
};