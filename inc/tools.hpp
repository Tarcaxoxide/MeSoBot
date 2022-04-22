#pragma once
#include<PROGRAM_NAME.HPP>
#include<string>
#include<deque>
#include<LangaugeProcessing.hpp>


namespace PROGRAM_NAME{
    std::string to_lower(std::string &target);
    void Load_Save(Sentence_st &SC,bool load=false);
};