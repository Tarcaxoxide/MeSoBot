#pragma
#include<PROGRAM_NAME.HPP>
#include<string>
#include <curl/curl.h>
namespace PROGRAM_NAME{
    namespace Network{
        class REST_cl{
            CURL *curl;
            CURLcode curl_response;
            std::string url;
            std::string header;
            public:
                REST_cl();
                ~REST_cl();
                void SetUrl(std::string strUrl);
                void SetHeader(std::string Header);
                std::string Get(std::string endpoint);
                std::string Post(std::string endpoint,std::string RequestBody);
        };
    };
};