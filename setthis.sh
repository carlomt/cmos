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

alias FileConverter='$CMOSCODEDIR/FileConverter.x'
alias Riduzione='$CMOSCODEDIR/Riduzione.x'

function _mycomplete_FileConverter_()
{
    local cmd="${1##*/}"
    local word=${COMP_WORDS[COMP_CWORD]}
    local prev=${COMP_WORDS[COMP_CWORD-1]}
    local line=${COMP_LINE}
    local xpat='!*.txt'

    COMPREPLY=($(compgen -f -X "$xpat" -- "${word}"))

    # completing an option
    if [[ "$word" == -* ]]; then
	COMPREPLY=( $( compgen -W "-help -verbose -o" -- $word ) )
    fi
}

complete -d -X '.[^./]*' -F _mycomplete_FileConverter_ FileConverter

echo "created the alias 'FileConverter' with autocompletion"

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

complete -d -X '.[^./]*' -F _mycomplete_Riduzione_ Riduzione

echo "created the alias 'Riduzione' with autocompletion"
