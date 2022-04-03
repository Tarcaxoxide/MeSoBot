#!/bin/bash


. shellLib/MathVarFunctions.sh




Result="$(_Get "$2" "Math")"

if [ "!$Result" == "!" ]
then
    _Set "$2" "0" "Math"
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

_Set "$2" "$Result" "Math"

echo "$Result"
