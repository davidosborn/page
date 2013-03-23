"""
Copyright (c) 2006-2013 David Osborn

Permission is granted to use and redistribute this software in source and binary
form, with or without modification, subject to the following conditions:

1. Redistributions in source form must retain the above copyright notice, this
   list of conditions, and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions, and the following disclaimer in the documentation
   and/or other materials provided with the distribution, and in the same place
   and form as other copyright, license, and disclaimer information.

As a special exception, distributions of derivative works in binary form may
include an acknowledgement in place of the above copyright notice, this list of
conditions, and the following disclaimer in the documentation and/or other
materials provided with the distribution, and in the same place and form as
other acknowledgements, similar in substance to the following:

	Portions of this software are based on the work of David Osborn.

This software is provided "as is", without any express or implied warranty. In
no event will the authors be liable for any damages arising out of the use of
this software.
"""

from __future__ import absolute_import

from types import InstanceType

def CopyType(x, other):
	otherType = type(other)
	if otherType is InstanceType:
		return other.__class__(x)
	return otherType(x)

def IsArithmeticType(x):
	try: x = x + 1
	except: return False
	return True

def IsIterable(x):
	return \
		hasattr(x, '__iter__') or \
		hasattr(x, '__getitem__')

def IsIterator(x):
	return IsIterable(x) and hasattr(x, 'next')
