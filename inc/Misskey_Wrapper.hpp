#pragma once
#include<PROGRAM_NAME.HPP>
#include<Network.hpp>
#include<string>
#include<deque>

namespace PROGRAM_NAME{
    namespace Misskey_Wrapper{
        typedef struct {std::string Key,Value;} key_pair;
        std::string jsonify_value(std::string value,bool key=false,bool more=false);
        std::string jsonify_value(bool value,bool key=false,bool more=false);
        std::string jsonify_value(int64_t value,bool key=false,bool more=false);
        std::string jsonify_pair(std::string key,std::string value,bool more);
        std::string jsonify_pair(std::string key,bool value,bool more);
        std::string jsonify_pair(std::string key,int64_t value,bool more);
        std::string jsonify_array(std::string key,std::deque<std::string> value,bool more);
        

        struct RequestBody_st{
            struct Data_t{
                #include"Misskey_DataStruct.hpp"
            };
            struct {
                #include"Misskey_DataStruct.hpp"
                std::deque<Data_t> choices;
                Data_t poll;
                Data_t user;
                std::deque<Data_t> reply;
                std::deque<Data_t> renote;
                std::deque<Data_t> files;
                Data_t channel;
                std::deque<Data_t> emojis;
                Data_t reactions;
                Data_t myReaction;
            }Data;
            std::string to_string();
            void from_string(std::string jsonString);
            void clear();
        };
    };
    namespace Misskey_Wrapper{

        class MisskeyBot_cl{
            Network::REST_cl Rest;
            std::string BotName;
            std::string BotUrl;
            std::string BotId;
            public:
            MisskeyBot_cl(std::string uri,std::string BotName,std::string userId);
            struct notes{
                Network::REST_cl* Rest;
                void init(Network::REST_cl* r);
                void create(RequestBody_st &Request);
                void global_timeline(RequestBody_st &Request);
                void show(RequestBody_st &Request);
            }notes;
            void ping(RequestBody_st &Request);
            bool operator==(std::string mention);
            std::string Id();
        };
    };
};
