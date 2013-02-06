#!/usr/bin/python

from util.math import *

e = Euler(DegToRad(90), DegToRad(-45))
v = Vector3f(.5, 2, 1)
print 'matrix:', (RotationMatrix(e) * v).tolist()
print 'quaternion:', (Quaternion(e) * v).tolist()
