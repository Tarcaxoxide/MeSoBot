extends Popup

export var callbackref:Dictionary={
	"Self":null,
	"Index":-1
}

func _on_CancelButton_pressed():
	callbackref["Self"].ReturnNewResponse(false,true,$VBoxContainer/LineEdit.text,callbackref["Index"])
	$VBoxContainer/LineEdit.text=""
	hide()


func POP(Self,Index,RespondingTo):
	callbackref["Self"]=Self
	callbackref["Index"]=Index
	$VBoxContainer/Label.text=RespondingTo
	popup()
	


func _on_SubmitButton_pressed():
	callbackref["Self"].ReturnNewResponse(true,$VBoxContainer/RequreMention.pressed,$VBoxContainer/LineEdit.text,callbackref["Index"])
	$VBoxContainer/LineEdit.text=""
	$VBoxContainer/RequreMention.pressed=true
	hide()
