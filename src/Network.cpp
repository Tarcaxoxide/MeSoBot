#include <Network.hpp>
#include <stdio.h>
#include <errno.h>

#include<iostream>




static std::string readBuffer;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{ 
    size_t realsize = size * nmemb;
    readBuffer=std::string((char*)contents);
    return realsize;
}




namespace PROGRAM_NAME{
    namespace Network{
        REST_cl::REST_cl(){
            curl_global_init(CURL_GLOBAL_ALL);
            curl = curl_easy_init();
            if(!curl){
                perror("Failed to set up curl");
                exit(1);
            }
        }
        REST_cl::~REST_cl(){
            curl_easy_cleanup(curl);
            curl_global_cleanup();
        }
        void REST_cl::SetUrl(std::string strUrl){
            url=strUrl;
        }
        void REST_cl::SetHeader(std::string Header){
            header=Header;
        }
        std::string REST_cl::Get(std::string endpoint){
            curl_easy_setopt(curl,CURLOPT_URL, std::string(url+endpoint).c_str());
            curl_response = curl_easy_perform(curl);

            if(curl_response != CURLE_OK){
                perror("Request Failed!");
            }else{
                return std::to_string(curl_response);
            }
            return std::string("\0");
        }
        std::string REST_cl::Post(std::string endpoint,std::string RequestBody){
            readBuffer.clear();
            struct curl_slist *chunk = NULL;
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            chunk = curl_slist_append(chunk, header.c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
            curl_easy_setopt(curl,CURLOPT_URL, std::string(url+endpoint).c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, RequestBody.c_str());
            curl_response = curl_easy_perform(curl);
            if(curl_response != CURLE_OK){
                perror("Request Failed!");
            }else{
                return std::string(readBuffer);
            }
            return std::string("\0");
        }
    };
};