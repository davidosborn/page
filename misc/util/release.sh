#!/bin/sh

eval `sed -n '/VER_MAJOR=/p;/VER_MINOR=/p;/VER_PATCH=/p' trunk/configure`
# FIXME: copy this rather than stating it here
VERSION="$VER_MAJOR.$VER_MINOR"
test "$VER_PATCH" -ne 0 && VERSION="$VERSION.$VER_PATCH"

version="$VERSION"
basename="page-$version"

cd trunk/misc/www/release/current
sed "/^RedirectMatch/s/\$1.*\$2/\$1$version\$2/" .htaccess >.htaccess.new
mv -f .htaccess.new .htaccess

cd ../../../../build
make dist-all
# FIXME: copy to misc/www/release/$version

ftp -v -n -i david.osborn.name <<EOF
user habitual heidiness
binary
cd public_html/page/release
mkdir $version
cd $version
mput $basename.tar.bz2 $basename.tar.gz $basename.shar $basename.tar.Z $basename.zip
cd ../current
put ../misc/www/release/current/.htaccess
EOF
