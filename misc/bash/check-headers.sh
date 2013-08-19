#!/bin/bash

. "$0/../config.sh"

fill() # string, length
{
	result="$1"
	while test ${#result} -lt "$2"; do
		result=$result.
	done
	echo $result
}

echo "checking headers..."
for file in $( find "$top_dir/src/local" -iname \*.hpp ); do
	nice_path=${file##$top_dir/}
	echo -n "  `fill \"$nice_path\" 70`"
	include_guard="${nice_path##src/}"
	include_guard="${include_guard%%.hpp}"
	include_guard=`echo $include_guard | tr / _`
	grep "#ifndef    page_${include_guard}_hpp" "$file" >/dev/null && grep "#   define page_${include_guard}_hpp" "$file" >/dev/null && echo "ok" || echo "FAILED"
done
