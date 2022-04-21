#include<PROGRAM_NAME.HPP>
#include<string>
#include<deque>
#include<iostream>
#include <tools.hpp>

namespace PROGRAM_NAME{
    struct Sentence_st{
        struct word_st{
            std::string word; // the word itself.
            int64_t amount; // the amount of times this word occurs.
        };
        std::deque<word_st> words; // the words in the sentence.

        void add(std::string target_word);
        Sentence_st(std::string new_string);
        std::string to_string();
        int64_t findWordAmount(std::string target);
        std::string findWordString(size_t index);
        unsigned int operator==(Sentence_st &other);
    };
};