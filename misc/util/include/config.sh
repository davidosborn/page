#!/bin/bash

################################################################################

set -e

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

on_exit()
{
	cd "$orig_dir"
}

trap on_exit EXIT
