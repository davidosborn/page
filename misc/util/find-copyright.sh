#!/bin/bash

. "$0/../include/config.sh"

grep -I -R --exclude-dir={.svn,"$top_dir/build","$top_dir/depends","$top_dir/misc/dump","$top_dir/misc/website/demo","$top_dir/misc/util/find-copyright.sh","$top_dir/res"} "Copyright .* 2006-.* David Osborn" "$top_dir" | sed 's/:.*//'
