extends Control

onready var bot = $ApiWrapper
onready var Settings = bot.API_DATA["Settings"]

var Responses_Path = "user://Responses.cfg"
var config = ConfigFile.new()
var load_response = config.load(Responses_Path)
var SpecialCharz=[",",".",";"]

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
		$IO/InputList.add_item(re,null,false)
	for i in range(1,999999999999999999):
		var re = config.get_value("Out",str(i))
		if re == null:
			break
		$IO/OutputList.add_item(re,null,true)

func SaveResponses():
	var InBag = Bagify($IO/InputList.items)
	var OutBag = Bagify($IO/OutputList.items)
	for item in InBag:
		config.set_value("In",str(item["Number"]),item["Element"])
	for item in OutBag:
		config.set_value("Out",str(item["Number"]),item["Element"])
	config.save(Responses_Path)

func _ready():
	$IO/OutputList.add_item("null",null,true)
	$IO/InputList.add_item("null",null,false)
	loadResponses()

func brainFunc(msg:Array) -> String:
	var BrainBag = Bagify($IO/InputList.items)
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
		$IO/InputList.add_item(msz,null,false)
		$IO/OutputList.add_item("@"+Settings["OwnerName"]+"@"+Settings["Uri"]+" respense needed!",null,true)
		Ret=$IO/OutputList.get_item_text($IO/OutputList.get_item_count()-1)
		SaveResponses()
	else:
		Ret=$IO/OutputList.get_item_text(index)
	
	for Schar in SpecialCharz:
		Ret=Ret.replace(Schar,"")
	return Ret.to_lower()


func On_Message(txt,id):
	var args = txt.split(" ",true)
	var isMentioned=false
	var pargs = []
	for arg in args:
		if(isMentioned):
			pargs.append(arg)
		if(arg == '@'+Settings["BotName"] or arg == '@'+Settings["BotName"]+'@'+Settings["Uri"]):
			isMentioned=true
	
	if(isMentioned):
		var msg=brainFunc(pargs)
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

func ReturnNewResponse(set:bool,text:String,index:int):
	if(index != 0):
		if(set):
			$IO/OutputList.set_item_text(index,text)
	$IO/OutputList.select(0)
	SaveResponses()

func _on_OutputList_item_selected(index):
	$NewResponseDialog.POP(self,index,$IO/InputList.get_item_text(index))

