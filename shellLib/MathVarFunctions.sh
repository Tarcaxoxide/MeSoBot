#!/bin/bash

_Set(){
    # 1 = user
    # 2 = value
    # 3 = variable_name
    echo "$2" > "MathVar/$1.$3"
}

_Get(){
    # 1 = user
    # 2 = variable_name
    if [ "!$(ls MathVar/$1.$2)" == "!" ]
    then
        _Set "$1" "0" "$2"
    fi
    cat "MathVar/$1.$2"
}

_GetVariables(){
    #1 = user
    ret=""
    for Var in $(ls MathVar/$1.*)
    do
        name="$(echo "$Var"|cut -d'.' -f2)"
        Result="$(_Get "$1" "$name")"
        ret="$ret$name=$Result;"
    done
    echo "$ret"
}

_ListVariables(){
    #1 = user
    ret=""
    for Var in $(ls MathVar/$1.*)
    do
        if [ "!$ret" != "!" ] ;then 
            ret="$ret,"
        fi
        name="$(echo "$Var"|cut -d'.' -f2)"
        Result="$(_Get "$1" "$name")"
        ret="$ret$name"
    done
    echo "[$ret]"
}

_Delete(){
    # 1 = user
    # 2 = variable_name
    
    rm -f "MathVar/$1.$2"
}

mkdir -p MathVar

