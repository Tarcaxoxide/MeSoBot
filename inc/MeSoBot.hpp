#pragma once
#include <PROGRAM_NAME.HPP>
#include <LangaugeProcessing.hpp>
#include <string>
#include <tools.hpp>
#include <fstream>
#include <filesystem>
#include <sstream>

namespace PROGRAM_NAME{
    struct ResponseMatrix_st{Sentence_st Key;Sentence_st Value;};
    class MeSoBot_cl{
        std::deque<ResponseMatrix_st> ResponseMatrix;
        public:
        void learn(std::string A,std::string B);
        void _save(std::string Set,std::string Key,std::string Value);
        std::string _ParseFileName(std::string FileName);
        std::string _load(std::string Set,std::string Key);
        std::string reply(std::string A_Text,std::string B_Text);
        void save();
        void load();
    };
};