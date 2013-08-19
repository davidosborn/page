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
