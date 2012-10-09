#!/bin/bash

grep -I -R --exclude-dir={.svn,../../build,../../depends,../../res} "Copyright .* 2006-" ../.. | sed 's/:.*//'
