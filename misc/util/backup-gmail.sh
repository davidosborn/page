#!/bin/bash

# This script builds a source distribution and uploads it to GMail for backup.

################################################################################
# build source distribution

cd ../..
mkdir -p build
cd build
# FIXME: run ../configure if Makefile doesn't exist
make dist-xz
cd dist

################################################################################
# upload to GMail

# FIXME: implement
