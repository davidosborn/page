#!/bin/bash

# This script will build wget with GNU TLS (instead of OpenSSL).  If you don't
# already have a working build of wget, you will have to download the packages
# yourself.

################################################################################
# shell setup

set -e

################################################################################
# package definitions

GNU_MIRROR=ftp://gnu.mirror.iweb.com/gnu

GMP_VERSION=5.0.4
GNUTLS_VERSION=3.0.17
NETTLE_VERSION=2.4
WGET_VERSION=1.13.4

################################################################################
# flags

MAKE_FLAGS=#"-j -l 1.5"

################################################################################
# system checks

USING_MINGW=`hash msysinfo && echo yes || echo no`

################################################################################
# Package:        gmp
# Dependencies:   
# Depended on by: nettle

wget $GNU_MIRROR/gmp/gmp-$GMP_VERSION.tar.xz -O- | tar -Jx
cd gmp-$GMP_VERSION
./configure --disable-static --enable-shared
make $MAKE_FLAGS install
#make $MAKE_FLAGS check
cd ..

################################################################################
# Package:        nettle
# Dependencies:   
# Depended on by: gnutls

wget $GNU_MIRROR/nettle/nettle-$NETTLE_VERSION.tar.gz -O- | tar -zx
cd nettle-$NETTLE_VERSION
./configure --enable-shared
make $MAKE_FLAGS install
cd ..

################################################################################
# Package:        gnutls
# Dependencies:   nettle
# Depended on by: wget

wget $GNU_MIRROR/gnutls/gnutls-$GNUTLS_VERSION.tar.xz -O- | tar -Jx
cd gnutls-$GNUTLS_VERSION
./configure --disable-static --enable-shared
make $MAKE_FLAGS install
cd ..

################################################################################
# Package:        wget
# Dependencies:   gnutls
# Depended on by: 

wget $GNU_MIRROR/wget/wget-$WGET_VERSION.tar.xz -O- | tar -Jx
cd wget-$WGET_VERSION
./configure
make $MAKE_FLAGS install
cd ..
