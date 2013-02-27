#!/bin/bash

# @section license
#
# Copyright (c) 2006-2012 David Osborn
#
# Permission is granted to use and redistribute this software in source and
# binary form, with or without modification, subject to the following
# conditions:
#
# 1. Redistributions in source form must retain the above copyright notice, this
#    list of conditions, and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions, and the following disclaimer in the documentation
#    and/or other materials provided with the distribution, and in the same
#    place and form as other copyright, license, and disclaimer information.
#
# As a special exception, distributions of derivative works in binary form may
# include an acknowledgement in place of the above copyright notice, this list
# of conditions, and the following disclaimer in the documentation and/or other
# materials provided with the distribution, and in the same place and form as
# other acknowledgements, similar in substance to the following:
#
#    Portions of this software are based on the work of David Osborn.
#
# This software is provided "as is", without any express or implied warranty. In
# no event will the authors be liable for any damages arising out of the use of
# this software.

################################################################################

. "$0/../misc/bash/config.sh"

################################################################################
# update to Bash 4

BASH_VERSION_LATEST=4.2
BASH_VERSION_MINIMUM=4.0
BASH_VERSION_CURRENT=$( echo ${BASH_VERSION} | sed 's/[^0-9.].*//' )

if [[ ${BASH_VERSION:0:1} < 4 ]]; then
	read -p "This script requires Bash $BASH_VERSION_MINIMUM or newer.  You have Bash $BASH_VERSION_CURRENT.  Do you want me to install Bash $BASH_VERSION_LATEST for you? " -r -n1 i
	if [[ i = y || i = Y ]]; then
		mkdir -p "$top_dir/depends/build"
		push_exit 'rm -rf "$top_dir/depends/build"'
		cd "$top_dir/depends/build"

		wget http://ftp.gnu.org/gnu/bash/bash-4.2.tar.gz -O- | tar -zx
		cd bash-4.2
		./configure && make install
		cd "$orig_dir"

		pop_exit
	else
		exit
	fi
fi

################################################################################

dependencies=<<-EOF
	PACKAGE     | NAME          | USED BY      | GROUP         | ALT OF | OPTIONAL | INSTALLABLE
	------------+---------------+--------------+---------------+---------+----------+------------
	angelscript | AngelScript   | page         | script        |        | yes      | yes
	boost       | Boost         | page         |               |        | no       | yes
	bzip2       | libbzip2      | page         |               |        | yes      | yes
	carbon      | Carbon        | page         | window, input |        | yes      | no
	cmake       | CMake         | openal-soft  | window, input |        | yes      | yes
	cg          | Cg            | page         |               |        | yes      | yes
	curl        | libcurl       | page         |               |        | yes      | yes
	d3d9        | Direct3D 9    | page         | video         |        | yes      | yes
	doxygen     | Doxygen       | page-build   | doc           |        | yes      | yes
	dinput9     | DirectInput 9 | page         | input         |        | yes      | yes
	dsound9     | DirectSound 9 | page         | sound         |        | yes      | yes
	expat       | Expat         | page         |               |        | yes      | yes
	ffmpeg      | FFmpeg        | page         |               |        | yes      | yes
	flac        | libFLAC       | page         |               |        | yes      | yes
	freetype    | FreeType 2    | page         |               |        | yes      | yes
	gif         | giflib        | page         |               |        | yes      | yes
	icu         | ICU           | boost        |               |        | yes      | yes
	jpeg        | libjpeg       | page         |               |        | yes      | yes
	lua         | Lua           | page         | script        |        | yes      | yes
	minizip     | Minizip       | page         |               |        | yes      | yes
	mng         | libmng        | page         |               |        | yes      | yes
	ogg         | libogg        | page         |               |        | yes      | yes
	openal      | OpenAL        | page         | sound         |        | yes      | yes
	openal-soft | OpenAL Soft   | page         | sound         | openal | yes      | yes
	openexr     | OpenEXR       | page         |               |        | yes      | yes
	opengl      | OpenGL        | page         | video         |        | yes      | yes
	png         | libpng        | page         |               |        | yes      | yes
	posix       | POSIX         | page         |               |        | yes      | no
	python      | Python        | page         | script        |        | yes      | yes
	sphinx      | Sphinx        | page         |               |        | yes      | yes
	squirrel    | Squirrel      | page         | script        |        | yes      | yes
	theora      | libtheora     | page         |               |        | yes      | yes
	tiff        | libtiff       | page         |               |        | yes      | yes
	ungif       | ungif         | page         |               |        | yes      | yes
	vorbis      | libvorbis     | page, theora |               |        | yes      | yes
	win32       | Win32         | page         | window, input |        | yes      | no
	x           | X11           | page         | window, input |        | yes      | no
	zip         | libzip        | page         |               |        | yes      | yes
	zlib        | zlib          | page, png    |               |        | yes      | yes
	EOF

pkg_name()
{
}

pkg_current_version()
{
}

pkg_latest_version()
{
}

pkg_install()
{
}

pkg_dependencies()
{
}
