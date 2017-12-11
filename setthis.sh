#!/bin/bash

function abspath() {
# generate absolute path from relative path
# $1     : relative filename
# return : absolute path
if [ -d "$1" ]; then
# dir
(cd "$1"; pwd)
elif [ -f "$1" ]; then
# file
if [[ $1 == */* ]]; then
echo "$(cd "${1%/*}"; pwd)/${1##*/}"
else
echo "$(pwd)/$1"
fi
fi
}

THISDIR=`pwd`
SOURCE="${BASH_SOURCE[0]}"
DIRREL="$( dirname "$SOURCE" )"
CMOSCODEDIR=$(abspath $DIRREL)




function _mycomplete_FileConverter_()
{
local cmd="${1##*/}"
local word=${COMP_WORDS[COMP_CWORD]}
local prev=${COMP_WORDS[COMP_CWORD-1]}
local line=${COMP_LINE}
#    local xpat='!*.txt'
#    COMPREPLY=($(compgen -f -X "$xpat" -- "${word}"))
COMPREPLY=($(compgen -f -X '!*.@(txt|raw)' -- "${word}"))

# completing an option
if [[ "$word" == -* ]]; then
COMPREPLY=( $( compgen -W "-help -verbose -o" -- $word ) )
fi
}

if [ -f ./FileConverter.x ]; then  
alias FileConverter='$CMOSCODEDIR/FileConverter.x'
complete -d -X '.[^./]*' -F _mycomplete_FileConverter_ FileConverter
echo "created the alias 'FileConverter' with autocompletion"
if [[ FileConverter.cpp -nt FileConverter.x ]]; then
echo "WARNING: FileConverter.cpp has been modified after FileConverter.x has been produced. Maybe you should recompile?"
fi
else
echo "WARNING: FileConverter.x does not exist, didi you compiled?"
echo "try: make FileConverter.x"
fi

function _mycomplete_Riduzione_()
{
local cmd="${1##*/}"
local word=${COMP_WORDS[COMP_CWORD]}
local prev=${COMP_WORDS[COMP_CWORD-1]}
local line=${COMP_LINE}
local xpat='!*.root'

COMPREPLY=($(compgen -f -X "$xpat" -- "${word}"))

# completing an option
if [[ "$word" == -* ]]; then
COMPREPLY=( $( compgen -W "-help -t -seedSize -checkLocalMaximumSide -edge -ped -o" -- $word ) )
fi
}

if [ -f ./Riduzione.x ]; then  
alias Riduzione='$CMOSCODEDIR/Riduzione.x'
complete -d -X '.[^./]*' -F _mycomplete_Riduzione_ Riduzione
echo "created the alias 'Riduzione' with autocompletion"
if [[ Riduzione.cpp -nt Riduzione.x ]]; then
echo "WARNING: Riduzione.cpp has been modified after Riduzione.x has been produced. Maybe you should recompile?"
fi
else
echo "WARNING: Riduzione.x does not exist, didi you compiled?"
echo "try: make Riduzione.x"
fi


function _mycomplete_esempio_()
{
local cmd="${1##*/}"
local word=${COMP_WORDS[COMP_CWORD]}
local prev=${COMP_WORDS[COMP_CWORD-1]}
local line=${COMP_LINE}
local xpat='!*.root'

COMPREPLY=($(compgen -f -X "$xpat" -- "${word}"))

# completing an option
if [[ "$word" == -* ]]; then
COMPREPLY=( $( compgen -W "-help -o" -- $word ) )
fi
}

if [ -f ./esempio.x ]; then
alias esempio='$CMOSCODEDIR/esempio.x'
complete -d -X '.[^./]*' -F _mycomplete_Riduzione_ Riduzione
echo "created the alias 'esempio' with autocompletion"
if [[ Riduzione.cpp -nt Riduzione.x ]]; then
echo "WARNING: esempio.cpp has been modified after Riduzione.x has been produced. Maybe you should recompile?"
fi
else
echo "WARNING: esempio.x does not exist, didi you compiled?"
echo "try: make esempio.x"
fi
