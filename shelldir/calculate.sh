#!/bin/bash

_Set(){
    echo "$2" > "MathVar/$1.result"
}

_Get(){
    cat "MathVar/$1.result"
}


Result="$(_Get "$2")"

if [ "!$Result" == "!" ]
then
    _Set "$2" "0"
fi

PreCalculate="pi=3.14159265358979323846;\
              e=2.71828182845904523536;\
              minute=60;\
              hour=minute*60;\
              day=hour*24;\
              year=day*365.25;\
              epoch=$(date +%s);\
              result=$Result"


Result="$(calc "$PreCalculate;$1")"

_Set "$2" "$Result"

echo "$Result"
