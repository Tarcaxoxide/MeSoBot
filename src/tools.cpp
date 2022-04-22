#include <tools.hpp>
#include <fstream>

namespace PROGRAM_NAME{
    std::string to_lower(std::string &target){
        for(size_t a=0;a<target.size();a++){
            unsigned char Char=target[a];
            target[a]=(char)std::tolower(Char);
        }
        return target;
    }

    void GenerateGraph(Sentence_st &SC){
        std::ofstream File("Graph.html");
        std::string graph = SC.Graph();
        File << graph << std::endl;
        File.close();
    }
};