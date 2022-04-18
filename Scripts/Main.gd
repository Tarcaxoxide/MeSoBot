extends Control

onready var bot = $ApiWrapper
onready var Settings = bot.API_DATA["Settings"]

var Responses_Path = "user://Responses.cfg"
var Responses_config = ConfigFile.new()
var Responses_response = Responses_config.load(Responses_Path)
var SpecialCharz=[",",".",";","!","?"]

var Settings_Path = "user://Settings.cfg"
var Settings_config = ConfigFile.new()
var load_response = Settings_config.load(Settings_Path)


func loadSettings():
	var v:int = int(str(Settings_config.get_value("bars","Bias")))
	$Main/X/Bias.value=v
	v = int(str(Settings_config.get_value("bars","Sensitivity")))
	$Main/X/Sensitivity.value=v

func saveSettings():
	Settings_config.set_value("bars","Bias",$Main/X/Bias.value)
	Settings_config.set_value("bars","Sensitivity",$Main/X/Sensitivity.value)
	Settings_config.save(Settings_Path)

func Bagify(things:Array) -> Array:
	var Ret:Array
	var count=0
	for item in things:
		if typeof(item) == TYPE_STRING:
			Ret.append({"Triggered":false,"Similarity":0.00,"Number":count,"Element":item})
			count+=1
	return Ret


func loadResponses():
	for i in range(1,999999999999999999):
		var re = Responses_config.get_value("In",str(i))
		if re == null:
			break
		$Main/IO/InputList.add_item(re,null,false)
	for i in range(1,999999999999999999):
		var re = Responses_config.get_value("Out",str(i))
		if re == null:
			break
		$Main/IO/OutputList.add_item(re,null,true)
	for i in range(1,999999999999999999):
		var re = Responses_config.get_value("RequireMention",str(i))
		if re == null:
			break
		$Main/IO/RequreMentionList.add_item(re,null,true)

func SaveResponses():
	var InBag = Bagify($Main/IO/InputList.items)
	var OutBag = Bagify($Main/IO/OutputList.items)
	var RequireBag = Bagify($Main/IO/RequreMentionList.items)
	for item in InBag:
		Responses_config.set_value("In",str(item["Number"]),item["Element"])
	for item in OutBag:
		Responses_config.set_value("Out",str(item["Number"]),item["Element"])
	for item in RequireBag:
		Responses_config.set_value("RequireMention",str(item["Number"]),item["Element"])
	Responses_config.save(Responses_Path)

func _ready():
	$Main/IO/OutputList.add_item("null",null,true)
	$Main/IO/InputList.add_item("null",null,false)
	$Main/IO/RequreMentionList.add_item("null",null,false)
	loadResponses()
	loadSettings()
	$Main/PercentDisplay.text=str( float($Main/X/Sensitivity.value)/float(100000) )
	$Main/PercentDisplay.text+=":"
	$Main/PercentDisplay.text+=str( float($Main/X/Bias.value)/float(100000) )


func ProcessesBagOfWords(Bag:Array,TargetString:String) ->Dictionary:
	var Ret:Dictionary ={"Index":-1,"Similarity":0.00}
	var sCount=1
	var Count=1
	var BestMatch:Dictionary
	for Element in Bag:
		var A=Element["Element"].split(" ",true)
		var A_Bag = Bagify(A)
		var B=TargetString.split(" ",true)
		var B_Bag = Bagify(B)
		var T:Dictionary
		for B_Element in B_Bag:
			#sCount=1
			#Count=1
			for A_Element in A_Bag:
				Count+=1
				if(A_Element["Element"] == B_Element["Element"]):
					A_Element["Triggered"]=true
					A_Element["Number"]=Element["Number"]
					sCount+=1
					T=A_Element
			if T.has("Triggered") and T["Triggered"]:
				var sim:float = (float($Main/X/Bias.value)/float(100000))-(float(sCount)/float(Count))
				if !BestMatch.has('Triggered') or (BestMatch["Similarity"] < sim):
					BestMatch=T
					BestMatch["Similarity"]=sim
	if BestMatch.has("Number") and BestMatch["Similarity"] >= (float($Main/X/Sensitivity.value)/float(100000)):
		$Console.Log(str("[Y]"+str(float($Main/X/Sensitivity.value)/float(100000)),"?",BestMatch,"=",TargetString))
		if BestMatch["Element"] == "":
			$Console.Log(str("[N]"+str(float($Main/X/Sensitivity.value)/float(100000)),"?",BestMatch,"=",TargetString))
			return {"Index":-1,"Similarity":0.00}
		return {"Index":BestMatch["Number"],"Similarity":BestMatch["Similarity"]}
	$Console.Log(str("[N]"+str(float($Main/X/Sensitivity.value)/float(100000)),"?",BestMatch,"=",TargetString))
	return {"Index":-1,"Similarity":0.00}

func brainFunc(msg:Array,Mentioned:bool) -> String:
	if(msg == [msg[0],msg[0]]):
		msg=[msg[0]]
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
	#Check
	var index=-1
	var Re:Dictionary=ProcessesBagOfWords(BrainBag,msz)
	index=Re["Index"]
	for Item in BrainBag:
		#TODO:: Implement bag of words.
		if(Item["Element"] == msz):
			index=Item["Number"] #number being it's id/index
	#Respend
	if(index == -2):
		return str("null")
	if(index == -1):
		$Main/IO/InputList.add_item(msz,null,true)
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
	if Input.is_action_just_pressed("ui_accept"):
		if $Console.visible:
			$Console.hide()
		else:
			$Console.popup()
		pass
	Settings["Token"]=$Settings.Token
	Settings["Uri"]=$Settings.Uri
	Settings["BotName"]=$Settings.BotName
	Settings["OwnerName"]=$Settings.OwnerName
	Settings["localOnly"]=!$Settings.Federating
	if $Settings.Live:
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

func _on_Sensitivity_value_changed(value):
	$Main/PercentDisplay.text=str( float($Main/X/Sensitivity.value)/float(100000) )
	$Main/PercentDisplay.text+=":"
	$Main/PercentDisplay.text+=str( float($Main/X/Bias.value)/float(100000) )
	saveSettings()
