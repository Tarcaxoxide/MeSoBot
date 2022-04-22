#include<LangaugeProcessing.hpp>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <tools.hpp>

namespace PROGRAM_NAME{
    void Sentence_st::add(std::string target_word,std::string next_word){
        size_t hasWord=0;
        if(target_word.size() < 1)return;
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
                if(NextWord.likelihood == 0)return;
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
        bool first=true;
        for(size_t i=0;i<new_string.size();i++){
            if(buffer.size() < 1)buffer="\b";
            if(new_string[i] != '\n'){
                if(new_string[i] == ' '){
                    if(oldBuffer.size() < 1 && !first){
                        add(buffer,oldBuffer);
                    }else if(first){
                        add("[Start of Sentence]",buffer);
                    }else if(buffer.size() > 1){
                        add(oldBuffer,buffer);
                    }
                    oldBuffer=buffer;
                    buffer="";
                    first=false;
                }else{
                    if(buffer == "\b")buffer="";
                    buffer+=new_string[i];
                }
            }
        }
        add(oldBuffer,"\b");
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
    std::string Sentence_st::Random(std::deque<std::string> &rList){
        std::string result;
        if(rList.size() < 1)return "\b";
        size_t rn = rand() % rList.size();
        result=rList[rn];
        return result;
    }
    std::string Sentence_st::Random(size_t SentenceSize){
        std::string buffer="",NextWord="",PreviewsWord="";
        size_t oldIndex=0;
        for(size_t a=0;(buffer.size()+NextWord.size()+1)<SentenceSize;a++){
            if(a){
                if(NextWord == "\b")break;
                PreviewsWord=NextWord;
                buffer+=NextWord+std::string(" ");
            }
            if(buffer == ""){
                std::deque<std::string> rList;
                for(size_t a=0;a<words[0].next_words.size();a++){
                    for(size_t b=0;b<words[0].next_words[a].likelihood;b++){
                        rList.push_back(words[0].next_words[a].word);
                    }
                }
                NextWord=Random(rList);
            }else{
                size_t this_index=0;
                for(size_t a=0;a<words.size();a++){
                    if(words[a].word == PreviewsWord)this_index=a+1;
                }
                if(this_index){
                    std::deque<std::string> rList;
                    for(size_t a=0;a<words[this_index-1].next_words.size();a++){
                        for(size_t b=0;b<words[this_index-1].next_words[a].likelihood;b++){
                            rList.push_back(words[this_index-1].next_words[a].word);
                        }
                    }
                    NextWord=Random(rList);
                }else{
                    NextWord="\b";
                }
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
    std::string Sentence_st::Graph(){
        std::string results="";
        results+=std::string("<ul>");
        for(size_t a=0;a<words.size();a++){
            results+=std::string("\t<li>'")+words[a].word+std::string("'\n");
            results+=std::string("\t\t<ul>");
            for(size_t b=0;b<words[a].next_words.size();b++){
                if(words[a].next_words[b].word != "\b"){
                    if(words[a].next_words[b].word.size()){
                        results+=std::string("\t\t\t<li>")+std::to_string(words[a].next_words[b].likelihood)+std::string(":'")+words[a].next_words[b].word+std::string("'</li>\n");
                    }else{
                        results+=std::string("\t\t\t<li>")+std::to_string(words[a].next_words[b].likelihood)+std::string(":'\\0")+std::string("'</li>\n");
                    }
                }else{
                    results+=std::string("\t\t\t<li>")+std::to_string(words[a].next_words[b].likelihood)+std::string(":'[End of sentence]")+std::string("'</li>\n");
                }
            }
            results+=std::string("\t\t</ul>\n");
            results+=std::string("\t</li>\n");
        }
        results+=std::string("</ul>");
        return results;
    }
};