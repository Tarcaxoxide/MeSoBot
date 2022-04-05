. shellLib/all.sh

Output="$(man "$1")"


Output="$(_Between "$Output" "DESCRIPTION" "EXAMPLES" )"

Output="$(_Trim "$Output" "2000")"

Line_Length="$(_Get $2 "line_length")"

if [ "$Line_Length" == "0" ]
then
    Line_Length="50"
    _Set "$2" "50" "line_length"
fi


Output="$(_AddNewLines "$Output" "$Line_Length")"
echo -ne "$Output"