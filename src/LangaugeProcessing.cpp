#include<LangaugeProcessing.hpp>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <tools.hpp>

namespace PROGRAM_NAME{
    void Sentence_st::add(std::string target_word,std::string next_word){

        if(((unsigned int)target_word[target_word.size()-1]) > 255)return;
        if(((unsigned int)next_word[next_word.size()-1]) > 255)return;
        
        size_t target_index=0;
        for(size_t a=0;a<words.size();a++){
            if(words[a].word == target_word)target_index=a+1;
        }
        if(target_index == 0){
            words.push_back({target_word,{},1});
            target_index=words.size();
        }{
            words[target_index-1].count++;
        }
        if(next_word.size() < 1)return;
        size_t next_index=0;
        for(size_t a=0;a<words[target_index-1].next_words.size();a++){
            if(words[target_index-1].next_words[a].word == next_word)next_index=a+1;
        }
        if(next_index == 0){
            words[target_index-1].next_words.push_back({next_word,1,1});
        }else{
            words[target_index-1].next_words[next_index-1].count++;
        }
        for(size_t a=0;a<words[target_index-1].next_words.size();a++){
            double _A=(words[target_index-1].next_words[a].count+0.00000000000000001)/words[target_index-1].next_words.size();
            double _B=_A;
            double _C=words[target_index-1].next_words.size()*_B;
            words[target_index-1].next_words[a].likelihood=1+(((size_t)(int)_C)%100); // 100 cap
        }
        // Add words to flat word bank.
        if(target_word != "[Start of sentence]"){
            size_t has=0;
            for(size_t i=0;i<wordBank.size();i++){
                if(wordBank[i].word == target_word)has=i+1;
            }
            if(has){
                wordBank[has-1].count++;
            }else{
                wordBank.push_back({target_word,1});
            }
        }else{
            size_t has=0;
            for(size_t i=0;i<wordBank.size();i++){
                if(wordBank[i].word == next_word)has=i+1;
            }
            if(has){
                wordBank[has-1].count++;
            }else{
                wordBank.push_back({next_word,1});
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
                    if(oldBuffer == ""){
                        oldBuffer="[Start of sentence]";
                    }
                    add(oldBuffer,buffer);
                    
                    oldBuffer=buffer;
                    //oRawFile<<oldBuffer;
                    buffer="";
                }else{
                    buffer+=new_string[i];
                }
            }
            bool haz=false;
            for(size_t i=0;i<RawLines.size();i++){
                if(RawLines[i] == new_string)haz=true;
            }
            if(!haz){
                RawLines.push_back(new_string);
            }
        }
        add(oldBuffer,"\b");
    }
    std::string Sentence_st::to_string(){
        std::string buffer="";
        for(size_t i=0;i<wordBank.size();i++){
            buffer+=wordBank[i].word+" ";
        }
        return buffer;
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
                        for(size_t b=0;b<words[0].next_words[a].likelihood;b++){
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
    std::string Sentence_st::Graph(){
        std::string results="";
        results+=std::string("<ul>");
        for(size_t a=0;a<words.size();a++){
            results+=std::string("\t<li>")+std::string(":'")+words[a].word+std::string("'\n");
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
    double Sentence_st::operator==(Sentence_st &Other){
        //fill the word lists;
        std::deque<std::string> OtherWords,MyWords;
        for(size_t a=0;a<wordBank.size();a++){
            for(size_t b=0;b<wordBank[a].count;b++){
                MyWords.push_back(wordBank[a].word);
            }
        }
        for(size_t a=0;a<Other.wordBank.size();a++){
            for(size_t b=0;b<Other.wordBank[a].count;b++){
                OtherWords.push_back(Other.wordBank[a].word);
            }
        }
        //ensure the word lists are the same size so we don't get an out of bounds index
        while(OtherWords.size() < MyWords.size()){
            OtherWords.push_back(" ");
        }
        while(OtherWords.size() > MyWords.size()){
            MyWords.push_back(" ");
        }
        //calculate the differences
        int64_t Total=0;
        int64_t Difference=0;
        for(size_t a=0;a<MyWords.size();a++){
            Total++;
            if(MyWords[a] != OtherWords[a])Difference++;
        }
        //calculate the similarity
        double _Difference=Difference+0.00000000000000000000; // convert this stupid int into a double XD
        double _Total=Total+0.00000000000000000000; // convert this stupid int into a double XD
        double _Ratio=_Difference/_Total;
        double _Similarity=1-_Ratio;
        double Similarity=_Similarity*100;
        return Similarity;
    }
};