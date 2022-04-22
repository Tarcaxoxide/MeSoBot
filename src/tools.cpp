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

    void Load_Save(Sentence_st &SC,bool load){
        
        std::ifstream iRawFile("Raw.txt");
        std::string line;
        for(;!iRawFile.eof();std::getline(iRawFile,line)){
            SC.AddSentence(line);
        }
        iRawFile.close();


        std::ofstream oGraphFile("Graph.html");
        std::string graph = SC.Graph();
        oGraphFile << graph << std::endl;
        oGraphFile.close();
        std::ofstream oRawFile("Raw.txt");
        oRawFile<<SC.to_string();
        oRawFile.close();
    }
};