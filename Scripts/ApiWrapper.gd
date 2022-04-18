extends Node

export var API_DATA:Dictionary = {
	"Settings":{
			"Uri":"",
			"Token":"",
			"localOnly":false,
			"BotName":"",
			"OwnerName":""
		},
		"Result":{},
		"httpActivity":3
}

onready var Settings = API_DATA["Settings"]

func apiRequest(endpoint,SendBody:Dictionary):
	var Query = JSON.print(SendBody)
	var Headers=["Content-Type:application/json"]
	var RequestUri="https://"+Settings["Uri"]+"/api"+endpoint
	$HTTPRequest.request(RequestUri,Headers,true,HTTPClient.METHOD_POST,Query)

func Generate_body(param:Dictionary) -> Dictionary:
	var req_body:Dictionary
	match param["type"]:
		'CreateNote':
			req_body={
				"text":param["text"],
				"visibility":"public",
				"cw":null,
				"localOnly":Settings["localOnly"],
				"replyId":null,
				"renoteId":null,
				"channelId":null,
				"poll":null,
				"i":Settings["Token"]
			}
		'GetNote':
			req_body={
				"withFiles":false,
				"limit":1,
				"i":Settings["Token"]
			}
		'CreateReplyNote':
			req_body={
				"text":param["text"],
				"visibility":"public",
				"cw":null,
				"localOnly":Settings["localOnly"],
				"replyId":param["id"],
				"renoteId":null,
				"channelId":null,
				"poll":null,
				"i":Settings["Token"]
			}
	return req_body


func R_ping():
	apiRequest("/ping",{})
	
func R_endpoints():
	apiRequest("/endpoints",{})

func R_CreateNote(msg_txt):
	apiRequest("/notes/create",Generate_body({'type':'CreateNote','text':msg_txt}))

func R_GetNote():
	API_DATA["httpActivity"]=1
	apiRequest("/notes/global-timeline",Generate_body({'type':'GetNote'}))

func R_Reply(msg_txt,reply_id):
	apiRequest("/notes/create",Generate_body({'type':'CreateReplyNote','text':msg_txt,'id':reply_id}))

func Result():
	if API_DATA["httpActivity"] == 1:
		return 1
	if API_DATA["httpActivity"] == 2:
		API_DATA["httpActivity"]=3
		return API_DATA["Result"]
	if API_DATA["httpActivity"] == 3:
		return 3
	
	if API_DATA["httpActivity"] != 1 and API_DATA["httpActivity"] != 3:
		return API_DATA["Result"]
	return null

func _on_HTTPRequest_request_completed(result, response_code, headers, body):
	var jsdt = JSON.parse(body.get_string_from_utf8())
	API_DATA["Result"]=jsdt.result
	API_DATA["httpActivity"]=2
	
