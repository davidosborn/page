#!/bin/bash

. "$0/../config.sh"

IFS=$'\n'

for file in \
	$( find "$top_dir/src" -type f \( \
		-iname \*.c   -o \
		-iname \*.cpp -o \
		-iname \*.h   -o \
		-iname \*.hpp -o \
		-iname \*.tpp \) ); do

	in_standard=yes
	in_system=yes

	local_headers=()

	for line in $( sed -e "s/^#\t*include \([<>0-9A-Za-z_\"/.]*\).*$/\1/;t;d" "$file" ); do
		if [[ ${line:0:1} = '<' ]]; then
			system=yes
		elif [[ ${line:0:1} = '"' ]]; then
			system=no
		else
			continue
		fi

		target=${line:1:$((${#line}-2))}

		[[ "$system" = yes && \
			$( expr index "$target" "/" ) -eq 0 && \
			$( expr index "$target" "." ) -eq 0 ]] && \
				standard=yes || standard=no

		if [[ "$in_system" = yes ]]; then
			if [[ "$in_standard" = yes ]]; then
				in_standard="$standard"
			else
				if [[ "$standard" = yes ]]; then
					echo "ERROR: $file"
					continue 2
				fi
			fi
			in_system="$system"
		else
			if [[ "$system" = yes ]]; then
				echo "ERROR: $file"
				continue 2
			fi
		fi

		if [[ "$system" = no && "${target:$((${#target}-4)):4}" != ".tpp" ]]; then
			local_headers=("${local_headers[@]}" "$target")
		fi
	done

	# check alphabetical ordering of local includes
	unsorted=$( echo "${local_headers[@]}" | tr '[:upper:]' '[:lower:]' )
	sorted=$( echo "$unsorted" | tr ' ' '\n' | sort | tr '\n' ' ' | sed 's/ $//' )
	if [[ "$sorted" != "$unsorted" ]]; then
		echo "UNSORTED: $file"
		echo "  unsorted: '$unsorted'"
		echo "  sorted:   '$sorted'"
		echo
	fi
done
