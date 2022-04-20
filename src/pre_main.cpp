#include<main.hpp>
#include<PROGRAM_NAME.HPP>

int main(int argc,char** argv){
    PROGRAM_NAME::Arguments_t Arguments;
    for(size_t i=0;i<argc;i++)
        Arguments.push_back(argv[i]);
    int Ret=PROGRAM_NAME::main(Arguments);
    return Ret;
}