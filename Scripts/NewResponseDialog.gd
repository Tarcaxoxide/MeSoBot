extends Popup

export var callbackref:Dictionary={
	"Self":null,
	"Index":-1
}

func _on_CancelButton_pressed():
	callbackref["Self"].ReturnNewResponse(false,$VBoxContainer/LineEdit.text,callbackref["Index"])
	$VBoxContainer/LineEdit.text=""
	hide()


func _on_SubmitButton_pressed():
	callbackref["Self"].ReturnNewResponse(true,$VBoxContainer/LineEdit.text,callbackref["Index"])
	$VBoxContainer/LineEdit.text=""
	hide()
