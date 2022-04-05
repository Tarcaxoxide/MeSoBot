. shellLib/all.sh

_Set "$3" "$2" "$1"

Result="$(_Get "$3" "$1")"

echo "$1 = $Result"