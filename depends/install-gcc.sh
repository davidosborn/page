#!/bin/bash

################################################################################
# shell setup

set -e

################################################################################
# package definitions

GNU_MIRROR=ftp://gnu.mirror.iweb.com/gnu

GCC_VERSION=4.6.3
GDB_VERSION=7.4
GMP_VERSION=5.0.4
MPFR_VERSION=3.1.0
MPC_VERSION=0.9

################################################################################
# flags

MAKE_FLAGS= #"-j -l 1.5"

################################################################################
# system checks

USING_MINGW=`hash msysinfo && echo yes || echo no`

################################################################################
# Package:        gmp
# Dependencies:   
# Depended on by: gcc

wget $GNU_MIRROR/gmp/gmp-$GMP_VERSION.tar.xz -O- | tar -Jx
cd gmp-$GMP_VERSION
./configure --disable-static --enable-shared
make $MAKE_FLAGS install
#make $MAKE_FLAGS check
cd ..

################################################################################
# Package:        mpfr
# Dependencies:   gmp
# Depended on by: gcc

wget $GNU_MIRROR/mpfr/mpfr-$MPFR_VERSION.tar.xz -O- | tar -Jx
cd mpfr-$MPFR_VERSION
./configure --disable-static --enable-shared
make $MAKE_FLAGS install
cd ..

################################################################################
# Package:        mpc
# Dependencies:   gmp, mpfr
# Depended on by: gcc

wget http://www.multiprecision.org/mpc/download/mpc-$MPC_VERSION.tar.gz -O- | tar zx
cd mpc-$MPC_VERSION
patch -p1 -i../mpc-0.9.patch
./configure --disable-static --enable-shared
make $MAKE_FLAGS install
cd ..

################################################################################
# Package:        gcc
# Dependencies:   gmp, mpc, mpfr
# Depended on by: gdb

wget $GNU_MIRROR/gcc/gcc-$GCC_VERSION/gcc-core-$GCC_VERSION.tar.bz2 -O- | tar -jx
wget $GNU_MIRROR/gcc/gcc-$GCC_VERSION/gcc-g++-$GCC_VERSION.tar.bz2 -O- | tar -jx
cd gcc-$GCC_VERSION
cd .. && mkdir gcc-$GCC_VERSION-build && cd gcc-$GCC_VERSION-build
GCC_CONFIGURE_FLAGS="--enable-libstdcxx-debug"
test "$USING_MINGW" == yes && GCC_CONFIGURE_FLAGS="$GCC_CONFIGURE_FLAGS --build=pentium4-pc-mingw32 --disable-nls --disable-win32-registry --enable-threads"
../gcc-$GCC_VERSION/configure $GCC_CONFIGURE_FLAGS
make $MAKE_FLAGS #CFLAGS="-O -march=pentium4" LIBCFLAGS="-O2" LIBCXXFLAGS="-O2 -fno-implicit-templates" LDFLAGS="-s"
make $MAKE_FLAGS install
cd ..

################################################################################
# Package:        gdb
# Dependencies:   gcc
# Depended on by: 

wget $GNU_MIRROR/gdb/gdb-$GDB_VERSION.tar.bz2 -O- | tar -jx
cd gdb-$GDB_VERSION
./configure --with-system-gdbinit
make $MAKE_FLAGS install
cd ..
