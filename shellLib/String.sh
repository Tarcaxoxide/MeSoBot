. shellLib/MathVarFunctions.sh
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
        if [ "$(calc "$count>$LineLength")" == "	1" ]
        then
            echo -ne "$Output\n"
            Output=''
            count='0'
            
        else
            if [ "$word" == "<^>" ]
            then
                echo -ne "$Output\n"
                Output=''
                count='0'
            fi
        fi
        if [ "$word" == "<^>" ]
        then
            continue
        fi
        
        count="$(calc "$count+$(echo "$word"|wc --chars)")"
        Output="$Output $word"
    done
    
}

_LineLength(){
    Line_Length="$(_Get $1 "line_length")"

    if [ "$Line_Length" == "0" ]
    then
        Line_Length="50"
        _Set "$1" "50" "line_length"
    fi
    echo -n "$Line_Length"
}

_PostLength(){
    Post_Length="$(_Get $1 "post_length")"

    if [ "$Post_Length" == "0" ]
    then
        Post_Length="2900"
        _Set "$1" "2900" "post_length"
    fi
    echo -n "$Post_Length"
}