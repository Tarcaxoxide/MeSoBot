
_Set(){
    echo "$2" > "MathVar/$1.$3"
}

_Get(){
    cat "MathVar/$1.$2"
}

mkdir -p MathVar

