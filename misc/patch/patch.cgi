#!/usr/bin/env python

# TODO:
# - default to the latest version if target isn't provided
# - optimization

def main(argv):
	# validate the version range
	import cgi
	form = cgi.FieldStorage()
	base = form.getfirst('base')
	targ = form.getfirst('target')
	if not base or not targ or targ < base:
		return 0
	if targ == base:
		print 'Content-type: application/octet-stream'
		print
		return 0
	# find the optimum set of progressive patches
	import glob
	patches = []
	this = base
	while this < targ:
		avail = glob.glob('../patch/%s-*.*' % this)
		if not len(avail):
			return 0
		next_patch = 0
		for patch in avail:
			if patch.split('-')[-1] == targ:
				patches.append(patch)
				this = targ
				break
			if patch > next_patch:
				next_patch = patch
		else:
			patches.append(next_patch)
			this = next_patch.split('-')[-1]
	# send the patches one after another
	print 'Content-type: application/octet-stream'
	print
	for patch in patches:
		print file(patch, 'rb').read(),

if __name__ == '__main__':
	import sys
	sys.exit(main(sys.argv) or 0)
