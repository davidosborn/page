from __future__ import absolute_import

class LineIterator:
	def __init__(self, iterator, comment = '#'):
		self.iterator = iter(iterator)
		self.comment = comment
		self.indentation = 0

	def __iter__(self):
		return self

	def next(self):
		while True:
			line = self.iterator.next().partition(self.comment)[0].rstrip()
			if line: break
		for i, c in enumerate(line):
			if not c.isspace():
				self.indentation = i
				line = line[i:]
				break
		return line
