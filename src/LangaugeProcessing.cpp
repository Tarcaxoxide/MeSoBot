#include<LangaugeProcessing.hpp>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

namespace PROGRAM_NAME{
    void Sentence_st::add(std::string target_word,std::string next_word){
        size_t hasWord=0;
        for(size_t i=0;i<words.size();i++){
            if(words[i].word == target_word)hasWord=i+1; 
        }
        next_word_st NextWord;
        if(next_word.size()){
            NextWord={next_word,1};
        }else{
            NextWord={next_word,0};// 0% likely , aka not possible.
        }
        if(hasWord == 0){
            words.push_back({target_word,1,{NextWord}});
        }else{
            words[hasWord-1].amount++;
            size_t hasNextWord=0;
            for(size_t i=0;i<words[hasWord-1].next_words.size();i++){
                if(words[hasWord-1].next_words[i].word == next_word)hasNextWord=i+1;
            }
            if(hasNextWord == 0){
                words[hasWord-1].next_words.push_back(NextWord);
            }else{
                words[hasWord-1].next_words[hasNextWord-1].likelihood++;
            }
        }
    }
    Sentence_st::Sentence_st(std::string new_string){
        AddSentence(new_string);
    }
    void Sentence_st::AddSentence(std::string new_string){
        if(new_string[new_string.size()-1] != ' ')new_string+=' ';
        std::string buffer="";
        std::string oldBuffer="";
        for(size_t i=0;i<new_string.size();i++){
            if(new_string[i] != '\n'){
                if(new_string[i] == ' '){
                    add(oldBuffer,buffer);
                    oldBuffer=buffer;
                    buffer="";
                }else{
                    buffer+=new_string[i];
                }
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
        if(target == " ")return 0;
        for(size_t i=0;i<words.size();i++){
            if(words[i].word == target)return words[i].amount;
        }
        return 0;
    }
    std::string Sentence_st::findWordString(size_t index){
        if(index > words.size()-1)return " ";
        return words[index].word;
    }
    std::string Sentence_st::Random(size_t SentenceSize){
        std::string buffer="",NextWord="",PreviewsWord="";
        size_t oldIndex=0;
        for(size_t a=0;(buffer.size()+NextWord.size()+1)<SentenceSize;a++){
            if(a != 0){
                PreviewsWord=NextWord;
                buffer+=NextWord+std::string(" ");
            }


            //ra = rand() % words.size();
            struct wordPat{std::string word;size_t Index;};
            std::deque<wordPat> rList;
            for(size_t z=0;z<words[oldIndex].next_words.size();z++){
                for(size_t x=0;x<words[oldIndex].next_words[z].likelihood;x++){
                    rList.push_back({words[oldIndex].next_words[z].word,z});
                }
            }

            if(rList.size()){
                size_t ra = rand() % rList.size();
                NextWord=rList[ra].word;
                oldIndex=rList[ra].Index;
            }else{
                break;
            }
        }
        
        return buffer;
    }
    unsigned int Sentence_st::operator==(Sentence_st &other){
        int64_t buffer=0;
        int64_t MaxSize=words.size();
        if(other.words.size() > MaxSize)MaxSize=other.words.size();
        for(size_t i=0;i<MaxSize;i++){
            std::string OtherWord=other.findWordString(i);
            std::string MyWord=findWordString(i);
            int64_t OtherAmount=other.findWordAmount(OtherWord);
            int64_t MyAmount=findWordAmount(OtherWord);
            buffer+=(OtherWord != MyWord);
            if(OtherWord == MyWord){
                buffer+=(OtherAmount != MyAmount);
            }
        }
        double dec_buffer=buffer;
        double dec_result;
        if(dec_buffer != 0){
            dec_result=1-(dec_buffer/MaxSize);
        }else{
            dec_result=1;
        }
        unsigned int whole_result=(unsigned int)(dec_result*100);
        return whole_result-1;
    }
};