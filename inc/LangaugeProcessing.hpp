#pragma once
#include<PROGRAM_NAME.HPP>
#include<string>
#include<deque>
#include<iostream>


namespace PROGRAM_NAME{
    struct Sentence_st{
        struct next_word_st{
            std::string word;
            int64_t likelihood;
        };
        struct word_st{
            std::string word;
            int64_t amount; // i don't think i use this anywhere.
            std::deque<next_word_st> next_words;
        };
        std::deque<word_st> words; // the words in the sentence.

        void add(std::string target_word,std::string next_word="");
        Sentence_st(std::string new_string);
        void AddSentence(std::string new_string);
        std::string to_string();
        int64_t findWordAmount(std::string target);
        std::string findWordString(size_t index);
        std::string Random(std::deque<std::string> &rList);
        std::string Random(size_t SentenceSize);
        unsigned int operator==(Sentence_st &other);
        std::string Graph();
    };
};