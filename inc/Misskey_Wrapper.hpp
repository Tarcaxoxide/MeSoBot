#pragma once
#include<PROGRAM_NAME.HPP>
#include<Network.hpp>
#include<string>
#include<deque>

namespace PROGRAM_NAME{
    namespace Misskey_Wrapper{
        typedef struct {std::string key,value;} key_pair;
        std::string jsonify_value(std::string value,bool key=false,bool more=false);
        std::string jsonify_value(bool value,bool key=false,bool more=false);
        std::string jsonify_value(int64_t value,bool key=false,bool more=false);
        std::string jsonify_pair(std::string key,std::string value,bool more);
        std::string jsonify_pair(std::string key,bool value,bool more);
        std::string jsonify_pair(std::string key,int64_t value,bool more);
        std::string jsonify_array(std::string key,std::deque<std::string> value,bool more);
        

        struct RequestBody_st{
            struct choices_tt{};
            struct user_tt{};
            struct file_tt{};
            struct poll_tt{
                std::deque<choices_tt> choices;
                bool multiple;
                int64_t expiresAt;
                int64_t expiredAfter;
            };
            struct channel_tt{};
            struct emoji_tt{};
            struct reaction_tt{};
            struct Data{
                std::string ApiKey;
                std::string visibility;
                std::deque<std::string> visibleUserIds;
                std::string text;
                std::string cw;
                bool localOnly;
                bool noExtractMentions;
                bool noExtractHashtags;
                bool noExtractEmojis;
                std::deque<std::string> fileIds;
                //std::deque<std::string> mediaIds; //deprecated
                std::string replyId;
                std::string renoteId;
                std::string channelId;
                poll_tt poll;
                bool withFiles;
                int64_t limit;
                std::string sinceId;
                std::string untilId;
                int64_t sinceDate;
                int64_t untilDate;
                std::string id;
                std::string createdAt;
                std::string userId;
                user_tt user;
                std::deque<Data> reply;
                std::deque<Data> renote;
                bool isHidden;
                std::deque<std::string> mentions;
                std::deque<file_tt> files;
                std::deque<std::string> tags;
                channel_tt channel;
                std::deque<emoji_tt> emojis;
                reaction_tt reactions;
                int64_t renoteCount;
                int64_t repliesCount;
                std::string uri,url;
                reaction_tt myReaction;
                std::string message;
                std::string code;
            }Data;
            std::string to_string();
            void from_string(std::string jsonString);
            void clear();
        };
    };
    namespace Misskey_Wrapper{

        class MisskeyBot_cl{
            Network::REST_cl Rest;
            public:
            MisskeyBot_cl(std::string uri);
            struct notes{
                Network::REST_cl* Rest;
                void init(Network::REST_cl* r);
                void create(RequestBody_st &Request);
                void global_timeline(RequestBody_st &Request);
            }notes;
            void ping(RequestBody_st &Request);
            
        };
    };
};
