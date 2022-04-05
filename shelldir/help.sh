. shellLib/String.sh

Output="$(cat shellLib/help.txt)"

Line_Length="$(_LineLength $2)"
Post_Length="$(_PostLength $2)"



Output="$(_Trim "$Output" "$Post_Length")"

Output="$(_AddNewLines "$Output" "$Line_Length")"

echo -ne "$Output"
