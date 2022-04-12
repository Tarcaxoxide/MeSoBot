#pragma once

#define PY_SSIZE_T_CLEAN
#include</usr/include/python3.9/Python.h>
#include<stdio.h>
#include<errno.h>
#include<string>
#include<vector>
#include<iostream>


namespace Lython{
    typedef std::vector<std::string> args_t;
    class Lython_C{
        wchar_t* PI{NULL};
        public:
        Lython_C(args_t Arguments);
        ~Lython_C();
        std::string Run(std::string str);
        void Import(std::string Module);
        void Import(std::string From,std::string Module);
    };
};