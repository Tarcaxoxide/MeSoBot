_Trim(){
    Input="$1"
    MaxSize="$2"
    OriginalSize="$(echo "$Input"|wc --chars)"
    ToTrim=" $(calc "$OriginalSize-$MaxSize" )"
    Output=''
    if [ "$(calc "$ToTrim > 0")" == "	1" ]
    then
        ToTrim="$(calc "$ToTrim+3")"
        Output="${Input::-$ToTrim}..."
    else
        Output="$Input"
    fi
    echo "$Output"
}

_Between(){
    Input="$1"
    StartWord="$2"
    StopWord="$3"
    trig='0'
    Output=''
    for word in $Input
    do
        if [ "$word" == "$StartWord" ]
        then
            trig='1'
            continue
        fi
        if [ "$word" == "$StopWord" ]
        then
            trig='0'
        fi
        if [ "$trig" == '1' ]
        then
            Output="$Output $word"
        fi
    done
    echo "$Output"
}

_AddNewLines(){
    Input="$1"
    LineLength="$2"
    Output=''
    count='0'
    for word in $Input
    do
        count="$(calc "$count+$(echo "$word"|wc --chars)")"
        Output="$Output $word"
        if [ "$(calc "$count>$LineLength")" == "	1" ]
        then
            echo "$Output"
            Output=''
            count='0'
        fi
    done
    
}