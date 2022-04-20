#include<Misskey_Wrapper.hpp>

#include<iostream>
#include<jsoncpp/json/value.h>
#include<jsoncpp/json/json.h>


namespace PROGRAM_NAME{
    namespace Misskey_Wrapper{
        std::string jsonify_value(std::string value,bool key,bool more){
            std::string Ret=std::string("\"")+value+std::string("\"");
            if(key)Ret+=":";
            if(more)Ret+=",";
            return Ret;
        }
        std::string jsonify_value(bool value,bool key,bool more){
            std::string Ret;
            if(value)Ret+="true"; else Ret+="false";
            //if(key)Ret+=":";
            if(more)Ret+=",";
            return Ret;
        }
        std::string jsonify_value(int64_t value,bool key,bool more){
            std::string Ret;
            Ret+=std::to_string(value);
            //if(key)Ret+=":";
            if(more)Ret+=",";
            return Ret;
        }
        std::string jsonify_pair(std::string key,std::string value,bool more){
            std::string Ret=jsonify_value(key,true)+jsonify_value(value);
            if(more)Ret+=",";
            return Ret;
        }
        std::string jsonify_pair(std::string key,bool value,bool more){
            std::string Ret=jsonify_value(key,true)+jsonify_value(value);
            if(more)Ret+=",";
            return Ret;
        }
        std::string jsonify_pair(std::string key,int64_t value,bool more){
            std::string Ret=jsonify_value(key,true)+jsonify_value(value);
            if(more)Ret+=",";
            return Ret;
        }
        std::string jsonify_array(std::string key,std::deque<std::string> value,bool more){
            std::string Ret;
            Ret+=jsonify_value(key,true);
            Ret+="[";
            for(size_t i=0;i<value.size();i++){
                Ret+=jsonify_value(value[i],false,(bool)(i != value.size()-1));
            }
            Ret+="]";
            if(more)Ret+=",";
            return Ret;
        }
    };
    namespace Misskey_Wrapper{
        std::string RequestBody_st::to_string(){
            std::string Ret;
            Ret+="{";
            if(Data.visibility != "")Ret+=jsonify_pair("visibility",Data.visibility,true);
            if(Data.visibleUserIds.size())Ret+=jsonify_array("visibleUserIds",Data.visibleUserIds,true);
            Ret+=jsonify_pair("text",Data.text,true);
            if(Data.cw != "")Ret+=jsonify_pair("cw",Data.cw,true);
            Ret+=jsonify_pair("localOnly",Data.localOnly,true);
            Ret+=jsonify_pair("noExtractMentions",Data.noExtractMentions,true);
            Ret+=jsonify_pair("noExtractHashtags",Data.noExtractHashtags,true);
            Ret+=jsonify_pair("noExtractEmojis",Data.noExtractEmojis,true);
            if(Data.fileIds.size())Ret+=jsonify_array("fileIds",Data.fileIds,true);
            if(Data.replyId != "")Ret+=jsonify_pair("replyId",Data.replyId,true);
            if(Data.renoteId != "")Ret+=jsonify_pair("renoteId",Data.renoteId,true);
            if(Data.noteId != "")Ret+=jsonify_pair("noteId",Data.noteId,true);
            if(Data.channelId != "")Ret+=jsonify_pair("channelId",Data.channelId,true);
            // add polls
            Ret+=jsonify_pair("withFiles",Data.withFiles,true);
            Ret+=jsonify_pair("limit",Data.limit,true);
            if(Data.sinceId != "")Ret+=jsonify_pair("sinceId",Data.sinceId,true);
            if(Data.untilId != "")Ret+=jsonify_pair("untilId",Data.untilId,true);
            Ret+=jsonify_pair("sinceDate",Data.sinceDate,true);
            Ret+=jsonify_pair("untilDate",Data.untilDate,true);
            //apikey always goes last, just because that's how i want it!
            Ret+=jsonify_pair("i",Data.ApiKey,false);
            Ret+="}";
            return Ret;
        }
        void  RequestBody_st::clear(){
            //Data.ApiKey="Token";
            Data.id="";
            Data.createdAt="";
            Data.userId="";
            Data.replyId="";
            Data.renoteId="";
            Data.uri="";
            Data.url="";
            Data.text="text";
            Data.cw="content warning";
            Data.visibility="public";
            Data.localOnly=false;
            Data.limit=1;
            Data.sinceDate=0;
            Data.untilDate=0;
            Data.tags.clear();
            Data.mentions_text.clear();
            Data.mentions.clear();
            Data.noteId="";
        }
        void RequestBody_st::from_string(std::string jsonString){
            Json::Value Jzon;
            Json::Reader reader;
            reader.parse(jsonString.c_str(),Jzon);

            if(Jzon.isArray()){
                Data.id=Jzon[0]["id"].asString();
                Data.createdAt=Jzon[0]["createdAt"].asString();
                Data.text=Jzon[0]["text"].asString();
                Data.cw=Jzon[0]["cw"].asString();
                Data.userId=Jzon[0]["userId"].asString();
                Data.replyId=Jzon[0]["replyId"].asString();
                Data.renoteId=Jzon[0]["renoteId"].asString();
                Data.noteId=Jzon[0]["noteId"].asString();
                // add reply
                // add renote
                Data.isHidden=Jzon[0]["isHidden"].asBool();
                Data.visibility=Jzon[0]["visibility"].asString();
                for(int i=0;i<Jzon[0]["mentions"].size();i++){
                    Data.mentions.push_back(Jzon[0]["mentions"][i].asString());
                }
                // add visibleUserIds
                // add fileIds
                // add files
                for(int i=0;i<Jzon[0]["tags"].size();i++){
                    Data.tags.push_back(Jzon[0]["tags"][i].asString());
                }
                // add poll
                Data.channelId=Jzon[0]["channelId"].asString();
                // add channel
                Data.localOnly=Jzon[0]["localOnly"].asBool();
                // add emojis
                // add reactions
                Data.renoteCount=Jzon[0]["renoteCount"].asInt64();
                Data.repliesCount=Jzon[0]["repliesCount"].asInt64();
                Data.uri=Jzon[0]["uri"].asString();
                Data.url=Jzon[0]["url"].asString();
                // add myReaction
                // User
                Data.user.id=Jzon[0]["user"]["id"].asString();
                Data.user.name=Jzon[0]["user"]["name"].asString();
                Data.user.username=Jzon[0]["user"]["username"].asString();
                Data.user.host=Jzon[0]["user"]["host"].asString();
                Data.user.avatarUrl=Jzon[0]["user"]["avatarUrl"].asString();
                Data.user.isAdmin=Jzon[0]["user"]["isAdmin"].asBool();
                Data.user.isModerator=Jzon[0]["user"]["isModerator"].asBool();
                Data.user.isBot=Jzon[0]["user"]["isBot"].asBool();
                Data.user.isCat=Jzon[0]["user"]["isCat"].asBool();
                // add emoji
                Data.user.onlineStatus=Jzon[0]["user"]["onlineStatus"].asString();
            }else{
                Data.id=Jzon["id"].asString();
                Data.createdAt=Jzon["createdAt"].asString();
                Data.text=Jzon["text"].asString();
                Data.cw=Jzon["cw"].asString();
                Data.userId=Jzon["userId"].asString();
                // add user
                Data.replyId=Jzon["replyId"].asString();
                Data.renoteId=Jzon["renoteId"].asString();
                Data.noteId=Jzon["noteId"].asString();
                // add reply
                // add renote
                Data.isHidden=Jzon["isHidden"].asBool();
                Data.visibility=Jzon["visibility"].asString();
                for(int i=0;i<Jzon["mentions"].size();i++){
                    Data.mentions.push_back(Jzon["mentions"][i].asString());
                }
                // add visibleUserIds
                // add fileIds
                // add files
                
                for(int i=0;i<Jzon["tags"].size();i++){
                    Data.tags.push_back(Jzon["tags"][i].asString());
                }
                // add poll
                Data.channelId=Jzon["channelId"].asString();
                // add channel
                Data.localOnly=Jzon["localOnly"].asBool();
                // add emojis
                // add reactions
                Data.renoteCount=Jzon["renoteCount"].asInt64();
                Data.repliesCount=Jzon["repliesCount"].asInt64();
                Data.uri=Jzon["uri"].asString();
                Data.url=Jzon["url"].asString();
                // add myReaction
            }
                {//Mentions in text
                    bool trig=false;
                    std::string Mention="";
                    for(size_t i=0;i<Data.text.size();i++){
                        if(Data.text[i] == '@'){trig=true;}
                        if(Data.text[i] == ' '){
                            trig=false;
                            if(Mention.size() > 1){
                                Data.mentions_text.push_back(Mention);
                                Mention="";
                            }
                        }
                        if(trig)Mention+=Data.text[i];
                    }
                }//Mentions in text
        }
    };
    namespace Misskey_Wrapper{
        MisskeyBot_cl::MisskeyBot_cl(std::string uri,std::string BotName,std::string userId){
            bool trig=false;
            for(size_t i=1;i<uri.size();i++){
                if(trig)BotUrl+=uri[i];
                if( uri[i] == '/' && uri[i-1] == '/' )trig=true;
            }
            this->BotName=BotName;
            BotId=userId;
            Rest.SetUrl(uri);
            Rest.SetHeader("Content_Type: application/json");
            notes.init(&Rest);
        }
        void MisskeyBot_cl::notes::init(Network::REST_cl* r){
            Rest=r;
        }
        void MisskeyBot_cl::notes::create(RequestBody_st &Request){
            Request.from_string(Rest->Post("/api/notes/create",Request.to_string()));
        }
        void MisskeyBot_cl::notes::global_timeline(RequestBody_st &Request){
            Request.from_string(Rest->Post("/api/notes/global-timeline",Request.to_string()));
        }
        void MisskeyBot_cl::ping(RequestBody_st &Request){
            Request.from_string(Rest.Post("/api/ping",Request.to_string()));
        }
        

        bool MisskeyBot_cl::operator==(std::string mention){
            if(mention == std::string("@")+BotName)return true;
            if(mention == std::string("@")+BotName+std::string("@")+BotUrl)return true;
            return false;
        }
        std::string MisskeyBot_cl::Id(){
            return BotId;
        }
    };
};

