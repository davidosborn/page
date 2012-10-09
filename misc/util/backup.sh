#!/bin/bash

################################################################################
# parse options

usage()
{
	cat <<EOF
Usage: $0 [options]
Options:
  --full           Include everything that can't be regenerated.
  --with-sandbox   Include the sandbox branch.
EOF
}

while test $# -gt 0; do
	case "$1" in
		--help) usage; exit 0;;
		--full) full=yes;;
		--with-demo) with_demo=yes;;
		--without-demo) with_demo=no;;
		--with-depends) with_depends=yes;;
		--without-depends) with_depends=no;;
		--with-sandbox) with_sandbox=yes;;
		--without-sandbox) with_sandbox=no;;
		--with-res) with_res=yes;;
		--without-res) with_res=no;;
	esac
	shift
done

################################################################################
# build exclude list

excludes="
^page/backup
^page/branches
^page/tags
^page/trunk/build
^page/trunk/doc
^page/trunk/misc
"

includes="
page/trunk/depends/*.patch
page/trunk/depends/*.sh
page/trunk/doc/page.texi
"

if test "$full" = yes; then
.
fi

################################################################################
# build backup file

backup_file="backup/page-`date +%y.%m.%d`.tar.xz"

mkdir -p backup
echo $excludes | tar -Jcvf $backup_file --totals -X- -C.. $includes

################################################################################
# upload backup file to GMail

# FIXME: implement
