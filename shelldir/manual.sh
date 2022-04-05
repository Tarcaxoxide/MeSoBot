. shellLib/String.sh

Output="$(man "$1")"

Line_Length="$(_LineLength $2)"
Post_Length="$(_PostLength $2)"

Output="$(_Between "$Output" "DESCRIPTION" "EXAMPLES" )"

Output="$(_Trim "$Output" "$Post_Length")"

Output="$(_AddNewLines "$Output" "$Line_Length")"

echo -ne "$Output"