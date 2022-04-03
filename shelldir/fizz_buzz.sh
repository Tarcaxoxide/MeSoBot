

for I in `seq 0 100`
do
    echo -n "[$I:"
    if [ "$(calc "$I%3")" == "	0" ]
    then
        echo -n "fizz"
    fi
    if [ "$(calc "$I%5")" == "	0" ]
    then
        echo -n "buzz"
    fi
    echo -n "]"
done
echo -ne "\n"


# /3 == fizz
# /5 == buzz
# (I%3) == 0 (fizz)
# (I%5) == 0 (buzz)
