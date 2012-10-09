#!/bin/bash

fill() # string, length
{
	result="$1"
	while test ${#result} -lt "$2"; do
		result=$result.
	done
	echo $result
}

echo "checking header guards..."
for file in `find ../../src -iname *.hpp`; do
	nice_path=${file##../../}
	echo -n "  `fill \"$nice_path\" 60`"
	inc_macro="${nice_path##src/}"
	inc_macro="${inc_macro%%.hpp}"
	inc_macro=`echo $inc_macro | tr / _`
	grep "#ifndef    page_${inc_macro}_inc" "$file" >/dev/null && echo "ok" || echo "FAILED"
done
