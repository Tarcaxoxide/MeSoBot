extends Control

onready var bot = $ApiWrapper
onready var Settings = bot.API_DATA["Settings"]

var Responses_Path = "user://Responses.cfg"
var config = ConfigFile.new()
var load_response = config.load(Responses_Path)
var SpecialCharz=[",",".",";","!","?"]

func Bagify(things:Array) -> Array:
	var Ret:Array
	var count=0
	for item in things:
		if typeof(item) == TYPE_STRING:
			Ret.append({"Triggered":false,"Number":count,"Element":item})
			count+=1
	return Ret


func loadResponses():
	for i in range(1,999999999999999999):
		var re = config.get_value("In",str(i))
		if re == null:
			break
		$Main/IO/InputList.add_item(re,null,false)
	for i in range(1,999999999999999999):
		var re = config.get_value("Out",str(i))
		if re == null:
			break
		$Main/IO/OutputList.add_item(re,null,true)
	for i in range(1,999999999999999999):
		var re = config.get_value("RequireMention",str(i))
		if re == null:
			break
		$Main/IO/RequreMentionList.add_item(re,null,true)

func SaveResponses():
	var InBag = Bagify($Main/IO/InputList.items)
	var OutBag = Bagify($Main/IO/OutputList.items)
	var RequireBag = Bagify($Main/IO/RequreMentionList.items)
	for item in InBag:
		config.set_value("In",str(item["Number"]),item["Element"])
	for item in OutBag:
		config.set_value("Out",str(item["Number"]),item["Element"])
	for item in RequireBag:
		config.set_value("RequireMention",str(item["Number"]),item["Element"])
	config.save(Responses_Path)

func _ready():
	$Main/IO/OutputList.add_item("null",null,true)
	$Main/IO/InputList.add_item("null",null,false)
	$Main/IO/RequreMentionList.add_item("null",null,false)
	loadResponses()

func brainFunc(msg:Array,Mentioned:bool) -> String:
	if msg == [msg[0],msg[0]]:
		msg=[msg[0]]
	print("?",msg)
	var BrainBag = Bagify($Main/IO/InputList.items)
	var Ret=""
	var msz=""
	var last=""
	for ms in msg:
		last=ms # get the last element
	for ms in msg:
		msz+=ms
		if(ms != last):# add space if it's not the last element
			msz+=" "
	msz=msz.to_lower()
	for Schar in SpecialCharz:
		msz=msz.replace(Schar,"")
	
	var index=-1
	for Item in BrainBag:
		if(Item["Element"] == msz):
			index=Item["Number"] #number being it's id/index
	
	if(index == -1):
		print("adding item:",msz)
		$Main/IO/InputList.add_item(msz,null,false)
		$Main/IO/OutputList.add_item("null",null,true)
		$Main/IO/RequreMentionList.add_item(str(true))
		Ret=$Main/IO/OutputList.get_item_text($Main/IO/OutputList.get_item_count()-1)
		SaveResponses()
	Ret=$Main/IO/OutputList.get_item_text(index)
	
	if Ret != str("@"+Settings["OwnerName"]+"@"+Settings["Uri"]+" respense needed!"):
		for Schar in SpecialCharz:
			Ret=Ret.replace(Schar,"")
	if Mentioned or $Main/IO/RequreMentionList.get_item_text(index) == str(false):
		return Ret.to_lower()
	return str("null")


func On_Message(txt,id):
	txt=txt.replace("\n"," ")
	var args = txt.split(" ",true)
	var isMentioned=false
	var pargs = []
	for arg in args:
		if(isMentioned):
			pargs.append(arg)
		if(arg == '@'+Settings["BotName"] or arg == '@'+Settings["BotName"]+'@'+Settings["Uri"]):
			isMentioned=true
	
	for arg in args:
		var tag=false
		for ar in arg:
			if(ar == '@'):
				tag=true
		if(!tag):
			pargs.append(arg)
	
	var msg=brainFunc(pargs,isMentioned)
	if msg != "null":
		$ApiWrapper.R_Reply(msg,id)
	

var LastResult
func _process(delta):
	Settings["Token"]=$Settings.Token
	Settings["Uri"]=$Settings.Uri
	Settings["BotName"]=$Settings.BotName
	Settings["OwnerName"]=$Settings.OwnerName
	if $Settings.Live:
		Settings["localOnly"]=!$Settings.Federating
		var res = $ApiWrapper.Result()
		match res:
			1:
				pass #busy doing request
			3:
				$ApiWrapper.R_GetNote()
			_:
				if typeof(res) == TYPE_ARRAY:
					if LastResult != str(res[0]["id"]):
						On_Message(str(res[0]["text"]),str(res[0]["id"]))
						LastResult=str(res[0]["id"])

func ReturnNewResponse(set:bool,RequreMention:bool,text:String,index:int):
	if(index != 0):
		if(set):
			$Main/IO/OutputList.set_item_text(index,text)
			$Main/IO/RequreMentionList.set_item_text(index,str(RequreMention))
	$Main/IO/OutputList.select(0)
	SaveResponses()

func _on_OutputList_item_selected(index):
	$NewResponseDialog.POP(self,index,$Main/IO/InputList.get_item_text(index))

