#!/bin/bash

# NOTE: .reg files are excluded because they are Windows files

. "$0/../include/config.sh"

IFS=
files=($(find "$top_dir" \
	-type d -wholename "$top_dir/.git"      -prune -o \
	-type d -wholename "$top_dir/build"     -prune -o \
	-type d -wholename "$top_dir/depends/*" -prune -o \
	-type d -wholename "$top_dir/misc/dump" -prune -o \
	-type f -wholename "*.3ds"              -prune -o \
	-type f -wholename "*.anim"             -prune -o \
	-type f -wholename "*.b3d"              -prune -o \
	-type f -wholename "*.beam"             -prune -o \
	-type f -wholename "*.blend"            -prune -o \
	-type f -wholename "*.bz2"              -prune -o \
	-type f -wholename "*.cam"              -prune -o \
	-type f -wholename "*.cff"              -prune -o \
	-type f -wholename "*.dbo"              -prune -o \
	-type f -wholename "*.dts"              -prune -o \
	-type f -wholename "*.exe"              -prune -o \
	-type f -wholename "*.flac"             -prune -o \
	-type f -wholename "*.gz"               -prune -o \
	-type f -wholename "*.info"             -prune -o \
	-type f -wholename "*.mat"              -prune -o \
	-type f -wholename "*.mdl"              -prune -o \
	-type f -wholename "*.mesh"             -prune -o \
	-type f -wholename "*.mid"              -prune -o \
	-type f -wholename "*.ms3d"             -prune -o \
	-type f -wholename "*.ogg"              -prune -o \
	-type f -wholename "*.otb"              -prune -o \
	-type f -wholename "*.otf"              -prune -o \
	-type f -wholename "*.pat"              -prune -o \
	-type f -wholename "*.pdb"              -prune -o \
	-type f -wholename "*.pfb"              -prune -o \
	-type f -wholename "*.pdf"              -prune -o \
	-type f -wholename "*.png"              -prune -o \
	-type f -wholename "*.ps"               -prune -o \
	-type f -wholename "*.pyc"              -prune -o \
	-type f -wholename "*.reg"              -prune -o \
	-type f -wholename "*.rg"               -prune -o \
	-type f -wholename "*.shar"             -prune -o \
	-type f -wholename "*.skel"             -prune -o \
	-type f -wholename "*.tga"              -prune -o \
	-type f -wholename "*.track"            -prune -o \
	-type f -wholename "*.ttf"              -prune -o \
	-type f -wholename "*.wav"              -prune -o \
	-type f -wholename "*.wings"            -prune -o \
	-type f -wholename "*.xcf"              -prune -o \
	-type f -wholename "*.xz"               -prune -o \
	-type f -wholename "*.Z"                -prune -o \
	-type f -wholename "*.zip"              -prune -o \
	-type f -wholename "*~"                 -prune -o \
	-type f -print0 | tr '\0' '\n'))
IFS=$'\n'

################################################################################

#for file in ${files[*]}; do
#	if [[ `grep -FIU -m1 $'\r' $file` ]]; then
#		echo "dos2unix \"$file\""
#		dos2unix "$file"
#	fi
#done

#exit

################################################################################

types=($(file -b -f- <<<"${files[*]}"))
crlfs=($(grep -cFIU -m1 $'\n' ${files[*]}))
{
	# http://stackoverflow.com/a/6145615

	while \
		read -r file <&3; do
		read -r type <&4
		read -r crlf <&5
		
		if [[ $crlf == 1 ]]; then
			echo "dos2unix \"$file\""
			#dos2unix "$file"
		fi
		
		# this test doesn't always work
		#if [[ $type == *CRLF* ]]; then
		#	echo "dos2unix \"$file\""
		#	dos2unix "$file"
		#fi
		
		if [[ $type == *data* ]]; then
			echo "binary $file"
		fi
	done
} \
	3<<<"${files[*]}" \
	4<<<"${types[*]}" \
	5<<<"${crlfs[*]}"
