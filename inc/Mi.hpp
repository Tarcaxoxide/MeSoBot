#pragma once

#include<iostream>
#include <Lython.hpp>
#include<string>
#include<vector>

namespace Mi{
    class Mi_C{
        Lython::Lython_C lython;
        std::vector<std::string> Channels;
        std::string uri,token;
        std::string _exec;
        public:
            Mi_C(Lython::args_t Arguments,std::string uri,std::string token,std::vector<std::string> Channels);
            void SetUp();
            void Run();
    };
};