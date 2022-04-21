#include<LangaugeProcessing.hpp>

namespace PROGRAM_NAME{
    void Sentence_st::add(std::string target_word){
        size_t hasWord=0;
        for(size_t i=0;i<words.size();i++){
            if(words[i].word == target_word)hasWord=i+1; 
        }
        if(hasWord == 0){
            words.push_back({target_word,1});
        }else{
            words[hasWord-1].amount++;
        }
    }
    Sentence_st::Sentence_st(std::string new_string){
        if(new_string[new_string.size()-1] != ' ')new_string+=' ';
        std::string buffer="";
        for(size_t i=0;i<new_string.size();i++){
            if(new_string[i] == ' '){
                add(buffer);
                buffer="";
            }else{
                buffer+=new_string[i];
            }
        }
    }
    std::string Sentence_st::to_string(){
        std::string buffer="";
        for(size_t i=0;i<words.size();i++){
            buffer+=words[i].word+std::string(":")+std::to_string(words[i].amount)+std::string(" ");
        }
        return buffer;
    }
    int64_t Sentence_st::findWordAmount(std::string target){
        if(target == "\0")return 0;
        for(size_t i=0;i<words.size();i++){
            if(words[i].word == target)return words[i].amount;
        }
        return 0;
    }
    std::string Sentence_st::findWordString(size_t index){
        if(index > words.size()-1)return " ";
        return words[index].word;
    }
    unsigned int Sentence_st::operator==(Sentence_st &other){
        int64_t buffer=0;
        int64_t MaxSize=words.size();
        if(other.words.size() > MaxSize)MaxSize=other.words.size();
        for(size_t i=0;i<MaxSize;i++){
            std::string OtherWord=other.findWordString(i);
            std::string MyWord=findWordString(i);
            buffer+=(OtherWord != MyWord);
        }
        double dec_buffer=buffer;
        double dec_result=1-(dec_buffer/MaxSize);
        unsigned int whole_result=(unsigned int)(dec_result*100);
        return whole_result;
    }
};