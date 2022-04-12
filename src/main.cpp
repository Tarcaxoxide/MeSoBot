#include<Mi.hpp>
#include "Credentials.h"

int main(int argc,char** argv){
    Lython::args_t Arguments;
    for(int i=0;i<argc;i++){
        Arguments.push_back(argv[i]);
    }
    
    Mi::Mi_C mi(Arguments,SITE_URI,SITE_TOKEN,{"global"});

    mi.SetUp();
    mi.Run();
}