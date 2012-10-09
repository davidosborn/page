#!/bin/bash
# MSYS/MinGW

find ../.. -path ../../build -prune -o -print | while read file; do
	if test -n "`file -b \"$file\" | grep \"CRLF\"`"; then
		echo "$file"
	fi
done

#while read -r file; do
#	tr -d \\r <"$file" >"$file"
#done <<EOF
#../../doc/bugs/initializer_list.cpp
#../../doc/doxygen.cfg.in
#../../doc/misc/blender-interface
#../../doc/misc/story
#../../misc/native/config.ini
#../../misc/notepad++/perLangWordWrap.py
#../../res/bin/scene/default.scene
#../../res/bin/scene/test/selfshadow/selfshadow.scene
#../../res/bin/scene/village/village.scene
#../../res/demo/scene/village/village.scene
#../../test/err/exception/test-1-throw-and-catch-multiple-unordered-tags.cpp
#../../test/err/exception/test-1-throw-many-tags-at-once.cpp
#../../test/util/serialize/test-1.cpp
#EOF
