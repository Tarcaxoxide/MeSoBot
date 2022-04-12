#include <Lython.hpp>


namespace Lython{
    Lython_C::Lython_C(args_t Arguments){
        PI=Py_DecodeLocale(Arguments[0].c_str(),NULL);
        if(PI == NULL){perror("failed to init python");exit(EXIT_FAILURE);}
        Py_SetProgramName(PI);
        Py_Initialize();
    }
    Lython_C::~Lython_C(){
        Py_Finalize();
        if(Py_FinalizeEx() < 0){
            exit(120);
        }
        PyMem_RawFree(PI);
    }
    std::string Lython_C::Run(std::string str){
        std::string result;
        PyRun_SimpleString(str.c_str());
        return result;
    }
    void Lython_C::Import(std::string Module){
        std::string _Import="import ";
        _Import+=Module;
        Run(_Import);
    }
    void Lython_C::Import(std::string From,std::string Module){
        std::string _Import="from ";
        _Import+=From;
        _Import+=" import ";
        _Import+=Module;
        Run(_Import);
    }
};

