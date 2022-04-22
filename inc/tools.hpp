#pragma once
#include<PROGRAM_NAME.HPP>
#include<string>
#include<deque>
#include<LangaugeProcessing.hpp>


namespace PROGRAM_NAME{
    std::string to_lower(std::string &target);
    void GenerateGraph(Sentence_st &SC);
};