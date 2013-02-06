from .io import LineIterator

class Preferences(dict):
	def __init__(self):
		dict.__init__(self, {
			'camera.fov':75,
			'camera.rotation.speed':2,
			'camera.scale.speed':1,
			'camera.translation.speed':15,
			'file.model.last':'',
			'file.scene.last':'',
			'render.axes':True,
			'render.wireframe':False,
			'res.root':'',
			'res.source':[],
			'tool.rotation.speed':5,
			'tool.scale.speed':10,
			'tool.translation.speed':20})
		self.Update()

	def Update(self):
		try:
			lines = LineIterator(open('config.ini'))
			for line in lines:
				key, value = [part.strip() for part in line.split('=', 1)]
				if key in self:
					try: self[key] = type(self[key])(eval(value))
					except ValueError:
						print 'warning: configuration variable type mismatch'
				else: self[key] = eval(value)
		except IOError:
			print 'warning: failed to load configuration'

	def Commit(self):
		f = open('config.ini', 'w')
		items = self.items()
		items.sort()
		for key, value in items:
			f.write('%s=%s\n' % (key, repr(value)))

prefs = Preferences()
