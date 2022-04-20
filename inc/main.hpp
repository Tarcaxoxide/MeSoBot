#pragma once
#include<PROGRAM_NAME.HPP>
#include<string>
#include<deque>

namespace PROGRAM_NAME{
    typedef std::deque<std::string> Arguments_t;
    int main(Arguments_t Arguments);
};