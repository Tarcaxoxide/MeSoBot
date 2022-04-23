#include <tools.hpp>

namespace PROGRAM_NAME{
    std::string to_lower(std::string &target){
        for(size_t a=0;a<target.size();a++){
            unsigned char Char=target[a];
            target[a]=(char)std::tolower(Char);
        }
        return target;
    }
};