extends Popup

export var Token:String
export var Uri:String
export var BotName:String
export var OwnerName:String
export var Live:bool
export var Federating:bool
export var Scooping:bool
var ivisible=false
var Settings_Path = "user://Settings.cfg"
var config = ConfigFile.new()
var load_response = config.load(Settings_Path)


func loadSettings():
	$List/Token/Value.text=config.get_value("Settings","Token","")
	$List/Uri/Value.text=config.get_value("Settings","Uri","")
	$List/BotName/Value.text=config.get_value("Settings","BotName","")
	$List/OwnerName/Value.text=config.get_value("Settings","OwnerName","")

func saveSettings():
	config.set_value("Settings","Token",Token)
	config.set_value("Settings","Uri",Uri)
	config.set_value("Settings","BotName",BotName)
	config.set_value("Settings","OwnerName",OwnerName)
	config.save(Settings_Path)

func _ready():
	loadSettings()

func _process(delta):
	Token = $List/Token/Value.text
	Uri = $List/Uri/Value.text
	BotName = $List/BotName/Value.text
	OwnerName = $List/OwnerName/Value.text
	Live = $List/Switches/Live.pressed
	Federating = $List/Switches/Federating.pressed
	Scooping =  $List/Switches/Scooping.pressed
	if Input.is_action_just_pressed("ui_cancel"):
		if ivisible:
			hide()
			ivisible=false
			saveSettings()
		else:
			popup()
			ivisible=true


