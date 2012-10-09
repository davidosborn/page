#!/bin/sh
for file in "$@"; do
	echo "$file"...
	zipnote "$file" | sed "/@ [^(]/s/@ \(.*\)\/\([^/]*\)$/@ \1\/\2\\n@=\2/" | zipnote -w "$file"
done
echo "done!"
