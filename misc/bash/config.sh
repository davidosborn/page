#!/bin/bash

################################################################################

set -e -o pipefail

################################################################################

# http://stackoverflow.com/a/246128

orig_dir=$( pwd )
script_dir=$( cd "$( dirname "$0" )" && pwd )
top_dir=$( echo $script_dir | sed -e 's/\/page\/.*$/\/page/' )
build_dir="$top_dir/build"

#tmp_dir="$build_dir/tmp"
#mkdir -p "$tmp_dir"

################################################################################

abs_path()
{
	echo $( cd "$( dirname "$1" )" && pwd )/$( basename "$1" )
}

native_path()
{
	echo "$( abs_path "$1" )" | sed -e 's/^\/\(.\)/\1:/' | tr '/' '\'
}

################################################################################

declare -a exit_handlers=

on_exit()
{
	for exit_handler in "${exit_handlers[@]}"; do
		eval "$exit_handler"
	done

	cd "$orig_dir"
}

push_exit()
{
	exit_handlers=("$1" "${exit_handlers[@]}")
}

pop_exit()
{
	exit_handlers=("${exit_handlers[@]:1}")
}

trap on_exit EXIT
