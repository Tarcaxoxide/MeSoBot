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
            struct Data{
                std::deque<choices_tt> choices;
                bool multiple;
                int64_t expiresAt;
                int64_t expiredAfter;
                std::string ApiKey;
                std::string visibility;
                std::deque<std::string> visibleUserIds;
                std::string text;
                std::string cw;
                bool localOnly;
                bool noExtractMentions;
                bool noExtractHashtags;
                bool noExtractEmojis;
                bool isAdmin;
                bool isModerator;
                bool isBot;
                bool isCat;
                std::deque<std::string> fileIds;
                //std::deque<std::string> mediaIds; //deprecated
                // "avatarBlurhash": null,
                // "avatarColor": null,
                std::string replyId;
                std::string renoteId;
                std::string noteId;
                std::string channelId;
                std::string onlineStatus
                Data poll;
                bool withFiles;
                int64_t limit;
                std::string sinceId;
                std::string untilId;
                int64_t sinceDate;
                int64_t untilDate;
                std::string id;
                std::string name;
                std::string host;
                std::string username;
                std::string createdAt;
                std::string userId;
                std::string avatarUrl;
                Data user;
                std::deque<Data> reply;
                std::deque<Data> renote;
                bool isHidden;
                std::deque<std::string> mentions;
                std::deque<std::string> mentions_text;
                std::deque<Data> files;
                std::deque<std::string> tags;
                Data channel;
                std::deque<Data> emojis;
                Data reactions;
                int64_t renoteCount;
                int64_t repliesCount;
                std::string uri,url;
                Data myReaction;
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
            }notes;
            void ping(RequestBody_st &Request);
            bool operator==(std::string mention);
            std::string Id();
        };
    };
};
