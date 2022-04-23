#include<MeSoBot.hpp>


namespace PROGRAM_NAME{
    void MeSoBot_cl::learn(std::string A,std::string B){
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
        save();
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
    void  MeSoBot_cl::_save(std::string Set,std::string Key,std::string Value){
        std::filesystem::create_directory("Data/");
        std::string FileName=std::string("Data/")+Set+std::string(".")+Key;
        std::ofstream File(FileName.c_str());
        File << Value;
        File.close();
    }
    std::string MeSoBot_cl::_load(std::string Set,std::string Key){
        std::filesystem::create_directory("Data/");
        std::string FileName=std::string("Data/")+Set+std::string(".")+Key;
        std::ifstream File(FileName.c_str());
        std::string Line="",Result="";
        for(;!File.eof();std::getline(File,Line)){
            Result+=Line;
        }
        File.close();
        return Result;
    }
    void MeSoBot_cl::save(){
        for(size_t a=0;a<ResponseMatrix.size();a++){
            _save("Key",std::to_string(a),ResponseMatrix[a].Key.to_string());
            _save("Value",std::to_string(a),ResponseMatrix[a].Value.to_string());
        }
    }
    void MeSoBot_cl::load(){
        std::filesystem::create_directory("Data/");
        for (const auto & file : std::filesystem::directory_iterator("Data/")){
            std::string DataIndex = _ParseFileName(file.path());
            std::string KeyValue = _load("Key",DataIndex);
            std::string ValueValue = _load("Value",DataIndex);
            learn(KeyValue,ValueValue);
        }
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