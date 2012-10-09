#!/bin/bash

# Building on a Windows system using MinGW/MSYS
# ---------------------------------------------
# First, you need to install MinGW/MSYS.  You will need the mingw-get utility,
# from <http://sourceforge.net/projects/mingw/files/Installer/mingw-get/>.
# Extract it to the directory that you will use for MinGW.
#
# Then, run mingw-get to install a basic MinGW system:
# 
#     mingw-get install gcc g++ msys-base msys-m4 msys-make msys-patch msys-perl
#
# You will also need wget, which is available in the msys-wget package.
# However, but you may want to use an alternative build, because the MSYS
# version is built with OpenSSL, which involves violating a license
# incompatibility.  See install-wget.sh for instructions on building wget
# yourself.
# 
# Next, you need to point the /usr/local directory at /mingw32, which you can do
# with the following command:
#
#     echo c:\devel\mingw\mingw32 /usr/local >>c:\devel\mingw\msys\1.0\etc\fstab
#
# Finally, you need to set a bunch of environment variables.  The setx command
# affects environment variables on a system-wide basis.  It is included in the
# Windows XP Service Pack 2 Support Tools, available for Windows XP Professional
# from <http://www.microsoft.com/download/en/details.aspx?id=18546>.
# Alternatively, you can set these by hand through the Control Panel.
#
#     setx PATH=C:\devel\mingw\mingw32\bin;C:\devel\mingw\bin;C:\devel\mingw\msys\1.0\bin
#     setx C_INCLUDE_PATH=C:\devel\mingw\mingw32\include;C:\devel\mingw\include;C:\devel\msys\1.0\include
#     setx CPLUS_INCLUDE_PATH=C:\devel\mingw\mingw32\include;C:\devel\mingw\include;C:\devel\msys\1.0\include
#     setx LIBRARY_PATH=C:\devel\mingw\mingw32\lib;C:\devel\mingw\lib;C:\devel\mingw\msys\1.0\lib

################################################################################
# shell setup

set -e

################################################################################
# package definitions

GNU_MIRROR=ftp://gnu.mirror.iweb.com/gnu

BZIP2_VERSION=1.0.6
CMAKE_VERSION=2.8.5
CURL_VERSION=7.24.0
EXPAT_VERSION=2.0.1
FREETYPE_VERSION=2.4.5
LIBFLAC_VERSION=1.2.1
LIBOGG_VERSION=1.3.0
LIBPNG_VERSION=1.5.4
LIBTHEORA_VERSION=1.1.1
LIBVORBIS_VERSION=1.3.2
LUA_VERSION=5.1.4
OPENAL_SOFT_VERSION=1.13
ZLIB_VERSION=1.2.5

################################################################################
# flags

MAKE_FLAGS=#"-j -l 1.5"

################################################################################
# system checks

USING_MINGW=`hash msysinfo && echo yes || echo no`

################################################################################
# Package:        bzip2
# Dependencies:   
# Depended on by: page

wget http://bzip.org/$BZIP2_VERSION/bzip2-$BZIP2_VERSION.tar.gz -O- | tar -zx
cd bzip2-$BZIP2_VERSION
if test "$USING_MINGW" == yes; then
	patch -p1 -i../bzip2-1.0.6-mingw.patch
fi
make $MAKE_FLAGS install
cd ..

################################################################################
# Package:        curl
# Dependencies:   
# Depended on by: page

wget http://curl.haxx.se/download/curl-$CURL_VERSION.tar.bz2 -O- | tar -jx
cd curl-$CURL_VERSION
./configure && make $MAKE_FLAGS install
cd ..

################################################################################
# Package:        expat
# Dependencies:   
# Depended on by: page

wget http://sourceforge.net/projects/expat/files/expat/$EXPAT_VERSION/expat-$EXPAT_VERSION.tar.gz/download -O- | tar -zx
cd expat-$EXPAT_VERSION
./configure && make $MAKE_FLAGS install
cd ..

################################################################################
# Package:        freetype
# Dependencies:   
# Depended on by: page

wget http://download.savannah.gnu.org/releases/freetype/freetype-$FREETYPE_VERSION.tar.bz2 -O- | tar -jx
cd freetype-$FREETYPE_VERSION
./configure && make $MAKE_FLAGS install
cd ..

################################################################################
# Package:        libflac
# Dependencies:   
# Depended on by: page

wget http://downloads.xiph.org/releases/flac/flac-$LIBFLAC_VERSION.tar.gz -O- | tar -zx
cd flac-$LIBFLAC_VERSION
patch -p1 -i../flac-1.2.1.patch
./configure && make $MAKE_FLAGS install
cd ..

################################################################################
# Package:        libogg
# Dependencies:   
# Depended on by: page

wget http://downloads.xiph.org/releases/ogg/libogg-$LIBOGG_VERSION.tar.gz -O- | tar -zx
cd libogg-$LIBOGG_VERSION
./configure && make $MAKE_FLAGS install
cd ..

################################################################################
# Package:        zlib
# Dependencies:   
# Depended on by: libpng, page

wget http://prdownloads.sourceforge.net/libpng/zlib-$ZLIB_VERSION.tar.bz2?download -O- | tar -jx
cd zlib-$ZLIB_VERSION
patch -p1 -i../zlib-1.2.5.patch
./configure && make $MAKE_FLAGS install
cd ..

################################################################################
# Package:        libpng
# Dependencies:   zlib
# Depended on by: page

wget http://prdownloads.sourceforge.net/libpng/libpng-$LIBPNG_VERSION.tar.gz?download -O- | tar -zx
cd libpng-$LIBPNG_VERSION
./configure && make $MAKE_FLAGS install
cd ..

################################################################################
# Package:        libvorbis
# Dependencies:   
# Depended on by: libtheora, page

wget http://downloads.xiph.org/releases/vorbis/libvorbis-$LIBVORBIS_VERSION.tar.bz2 -O- | tar -jx
cd libvorbis-$LIBVORBIS_VERSION
./configure && make $MAKE_FLAGS install
cd ..

################################################################################
# Package:        libtheora
# Dependencies:   libvorbis
# Depended on by: page

wget http://downloads.xiph.org/releases/theora/libtheora-$LIBTHEORA_VERSION.tar.bz2 -O- | tar -jx
cd libtheora-$LIBTHEORA_VERSION
./configure && make $MAKE_FLAGS install
cd ..

################################################################################
# Package:        lua
# Dependencies:   
# Depended on by: page

wget http://www.lua.org/ftp/lua-$LUA_VERSION.tar.gz -O- | tar -zx
cd lua-$LUA_VERSION
make $MAKE_FLAGS mingw install
cd ..

################################################################################
# Package:        cmake
# Dependencies:   
# Depended on by: openal-soft

wget http://www.cmake.org/files/v2.8/cmake-$CMAKE_VERSION.tar.gz -O- | tar -zx
cd cmake-$CMAKE_VERSION
CMAKE_CONFIGURE_FLAGS=`test "$USING_MINGW" == yes && echo --prefix=\`cd /usr/local && pwd -W\``
./configure $CMAKE_CONFIGURE_FLAGS && make $MAKE_FLAGS install
cd ..

################################################################################
# Package:        openal-soft
# Dependencies:   cmake
# Depended on by: page

wget http://kcat.strangesoft.net/openal-releases/openal-soft-$OPENAL_SOFT_VERSION.tar.bz2 -O- | tar -jx
cd openal-soft-$OPENAL_SOFT_VERSION/build
cmake -G "MSYS Makefiles" -DCMAKE_INSTALL_PREFIX=`cd /usr/local && pwd -W` .. && make install
cd ../..
