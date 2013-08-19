#!/bin/bash

. "$0/../config.sh"

for file in \
	$( find "$top_dir/src" -type f \( \
		-iname \*.c   -o \
		-iname \*.cpp -o \
		-iname \*.h   -o \
		-iname \*.hpp -o \
		-iname \*.tpp \) ); do
	sed -e "s/[[:space:]]\{1,\}$//" -i "$file"
done
