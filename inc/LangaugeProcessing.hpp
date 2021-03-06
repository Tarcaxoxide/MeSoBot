#pragma once
#include<PROGRAM_NAME.HPP>
#include<string>
#include<deque>
#include<iostream>


namespace PROGRAM_NAME{
    struct Sentence_st{
        struct next_word_st{ //represents the next word in the generating sentence sequences.
            std::string word;
            int64_t count;
            int64_t likelihood;
        };
        struct word_st{ // used for generating sentences
            std::string word;
            std::deque<next_word_st> next_words;
            int64_t count;
        };
        struct TmpWord_st{ // used for comparing the current sentence object with another.
            std::string word;
            int64_t count;
        };
        std::deque<word_st> words; // the words in the sentence.
        std::deque<std::string> RawLines;
        std::deque<TmpWord_st> wordBank; // flat bank of words in the sentence for comparing.

        void add(std::string target_word,std::string next_word="");
        Sentence_st(std::string new_string);
        void AddSentence(std::string new_string);
        std::string to_string();
        std::string findWordString(size_t index);
        std::string Random(std::deque<std::string> &rList);
        std::string Random(size_t SentenceSize);
        std::string Graph();
        double operator==(Sentence_st &Other);
    };
};