################################################################################
# scene

class Form:
	def __init__(self, name = '', model = None):
		self.name = name
		self.model = model
		self.position = [0, 0, 0]
		self.orientation = [0, 0, 0]
		self.scale = [1, 1, 1]
		self.ResetTransformation()

	def GetPosition(self):
		return Add(self.position, self.translation)
	def GetOrientation(self):
		return Add(self.orientation, self.rotation)
	def GetScale(self):
		return Mul(self.scale, self.rescale)

	def MultMatrix(self):
		glTranslate(*self.GetPosition())
		MultEulerMatrix(self.GetOrientation())
		glScalef(*self.GetScale())

	def ResetTransformation(self):
		self.translation = [0, 0, 0]
		self.rotation = [0, 0, 0]
		self.rescale = [1, 1, 1]
	def BakeTransformation(self):
		self.position = self.GetPosition()
		self.orientation = self.GetOrientation()
		self.scale = self.GetScale()
		self.ResetTransformation()

class Scene:
	def __init__(self):
		self.forms = []
		self.script = None
		self.track = None
		self.path = None

	def FromFile(self, path):
		self.__init__()
		ROOT_LEVEL = 0
		FORM_LEVEL = 1
		level = ROOT_LEVEL
		f = open(path)
		if f.readline().strip() != '#!PAGEscene':
			raise FormatError(path + ': invalid signature')
		lineIter = LineIterator(f)
		for line in lineIter:
			split = line.split()
			if level == ROOT_LEVEL:
				if split[0] == 'form':
					name = ''
					if len(split) >= 2:
						name = line[len(split[0]):].lstrip()
					self.forms.append(Form(name))
					if lineIter.next() != '{':
						raise FormatError(path + ': form missing opening brace')
					level = FORM_LEVEL
				elif split[0] == 'script':
					self.script = line[len(split[0]):].lstrip()
				elif split[0] == 'track':
					self.track = line[len(split[0]):].lstrip()
				elif split[0] == '{':
					raise FormatError(path + ': unexpected opening brace')
				elif split[0] == '}':
					raise FormatError(path + ': unexpected closing brace')
				else:
					raise FormatError(path + ': unrecognized token')
			elif level == FORM_LEVEL:
				if split[0] == 'model':
					self.forms[-1].model = Model().FromFile(FindResource(line[len(split[0]):].lstrip()))
				elif split[0] == 'position':
					if len(split) == 2:
						self.forms[-1].position = (float(split[1]), float(split[1]), float(split[1]))
					else:
						self.forms[-1].position = (float(split[1]), float(split[2]), float(split[3]))
				elif split[0] == 'orientation':
					split.forms[-1].orientation = (float(split[1]), float(split[2]), float(split[3]), float(split[4]))
				elif split[0] == 'scale':
					if len(split) == 2:
						self.forms[-1].scale = (float(split[1]), float(split[1]), float(split[1]))
					else:
						self.forms[-1].scale = (float(split[1]), float(split[2]), float(split[3]))
				elif split[0] == '}':
					level = ROOT_LEVEL
				elif split[0] == '{':
					raise FormatError(path + ': unexpected opening brace')
				else:
					raise FormatError(path + ': unrecognized token')
		self.path = path
		return self

	def ToFile(self, path):
		# FIXME: implement
		self.path = path

scene = Scene()

################################################################################
# model

class Part:
	def __init__(self, name = '', mesh = None, material = None):
		self.name = name
		self.mesh = mesh or Mesh()
		self.material = material or Material()
		self.position = [0, 0, 0]
		self.orientation = [0, 0, 0, 1]
		self.scale = [1, 1, 1]

	def MultMatrix(self):
		glTranslate(*self.position)
		MultQuatMatrix(self.orientation)
		glScalef(*self.scale)

class Model:
	def __init__(self):
		self.parts = []

	def FromFile(self, path):
		self.__init__()
		ROOT_LEVEL = 0
		PART_LEVEL = 1
		level = ROOT_LEVEL
		f = open(path)
		if f.readline().strip() != '#!PAGEmodel':
			raise FormatError(path + ': invalid signature')
		lineIter = LineIterator(f)
		for line in lineIter:
			split = line.split()
			if level == ROOT_LEVEL:
				if split[0] == 'part':
					name = ''
					if len(split) >= 2:
						name = line[len(split[0]):].lstrip()
					self.parts.append(Part(name))
					if lineIter.next() != '{':
						raise FormatError(path + ': part missing opening brace')
					level = PART_LEVEL
				elif split[0] == 'skeleton':
					self.skeleton = Skeleton().FromFile(FindResource(line[len(split[0]):].lstrip()))
				elif split[0] == '{':
					raise FormatError(path + ': unexpected opening brace')
				elif split[0] == '}':
					raise FormatError(path + ': unexpected closing brace')
				else:
					raise FormatError(path + ': unrecognized token')
			elif level == PART_LEVEL:
				if split[0] == 'mesh':
					self.parts[-1].mesh = Mesh().FromFile(FindResource(line[len(split[0]):].lstrip()))
				elif split[0] == 'material':
					self.parts[-1].material = Material().FromFile(FindResource(line[len(split[0]):].lstrip()))
				elif split[0] == '}':
					level = ROOT_LEVEL
				elif split[0] == '{':
					raise FormatError(path + ': unexpected opening brace')
				else:
					raise FormatError(path + ': unrecognized token')
		return self

#		self.__init__()
#		ROOT_LEVEL = 0
#		PART_LEVEL = 1
#		level = ROOT_LEVEL
#		f = open(path)
#		if f.readline().strip() != '#!PAGEmodel':
#			raise FormatError(path + ': invalid signature')
#		lineIter = CleanLines(f)
#		for line in lineIter:
#			split = line.split()
#			if level == ROOT_LEVEL:
#				if split[0] == 'part':
#					name = ''
#					if len(split) >= 2:
#						name = line[len(split[0]):].lstrip()
#					self.parts.append(Part(name))
#					if lineIter.next() != '{':
#						raise FormatError(path + ': part missing opening brace')
#					level = PART_LEVEL
#				elif split[0] == 'skeleton':
#					self.skeleton = Skeleton().FromFile(FindResource(line[len(split[0]):].lstrip()))
#				elif split[0] == '{':
#					raise FormatError(path + ': unexpected opening brace')
#				elif split[0] == '}':
#					raise FormatError(path + ': unexpected closing brace')
#				else:
#					raise FormatError(path + ': unrecognized token')
#			elif level == PART_LEVEL:
#				if split[0] == 'mesh':
#					self.parts[-1].mesh = Mesh().FromFile(FindResource(line[len(split[0]):].lstrip()))
#				elif split[0] == 'material':
#					self.parts[-1].material = Material().FromFile(FindResource(line[len(split[0]):].lstrip()))
#				elif split[0] == '}':
#					level = ROOT_LEVEL
#				elif split[0] == '{':
#					raise FormatError(path + ': unexpected opening brace')
#				else:
#					raise FormatError(path + ': unrecognized token')
#		return self

################################################################################
# material

class Pass:
	class Texture:
		def __init__(self, texture = None, shift = (0, 0), scale = (1, 1)):
			self.texture = texture
			self.shift = shift
			self.scale = scale
	class Exponent:
		def __init__(self, value = 0, texture = None):
			self.value = value
			self.texture = texture or Pass.Texture()
	class Ambient:
		def __init__(self, color = (1, 1, 1), texture = None):
			self.color = color
			self.texture = texture or Pass.Texture()
	class Blend:
		def __init__(self, value = 1, texture = None):
			self.value = value
			self.texture = texture or Pass.Texture()
	class Diffuse:
		def __init__(self, color = (1, 1, 1, 1), texture = None):
			self.color = color
			self.texture = texture or Pass.Texture()
	class Emissive:
		def __init__(self, color = (0, 0, 0), texture = None):
			self.color = color
			self.texture = texture or Pass.Texture()
	class Fresnel:
		def __init__(self, color = (0, 0, 0), texture = None, exponent = None):
			self.color = color
			self.texture = texture or Pass.Texture()
			self.exponent = exponent or Pass.Exponent()
	class Gloss:
		def __init__(self, value = 0, texture = None):
			self.value = value
			self.texture = texture or Pass.Texture()
	class Normal:
		def __init__(self, texture = None):
			self.texture = texture or Pass.Texture()
	class Specular:
		def __init__(self, color = (1, 1, 1), texture = None, exponent = None):
			self.color = color
			self.texture = texture or Pass.Texture()
			self.exponent = exponent or Pass.Exponent()

	def __init__(self):
		self.ambient = self.Ambient()
		self.blend = self.Blend()
		self.diffuse = self.Diffuse()
		self.emissive = self.Emissive()
		self.fresnel = self.Fresnel()
		self.gloss = self.Gloss()
		self.normal = self.Normal()
		self.specular = self.Specular()

class Material:
	def __init__(self):
		self.passes = []

	def FromFile(self, path):
		self.__init__()
		ROOT_LEVEL = 0
		PASS_LEVEL = 1
		TEX_LEVEL = 2
		level = ROOT_LEVEL
		checkTexLevel = False
		texLevelTex = None
		f = open(path)
		sig = f.readline().strip()
		if sig != '#!PAGEmat' and sig != '#!PAGEmaterial':
			raise FormatError(path + ': invalid signature')
		lineIter = LineIterator(f)
		for line in lineIter:
			if checkTexLevel:
				checkTexLevel = False
				if line == '{':
					level = TEX_LEVEL
					continue
			split = line.split()
			if level == ROOT_LEVEL:
				if split[0] == 'pass':
					self.passes.append(Pass())
					if lineIter.next() != '{':
						raise FormatError(path + ': pass missing opening brace')
					level = PASS_LEVEL
			elif level == PASS_LEVEL:
				if split[0] == 'ambient':
					self.passes[-1].ambient.texture.texture = Texture().FromFile(FindResource(line[len(split[0]):].lstrip()))
					texLevelTex = self.passes[-1].ambient.texture
					checkTexLevel = True
				elif split[0] == 'blend':
					self.passes[-1].blend.texture.texture = Texture().FromFile(FindResource(line[len(split[0]):].lstrip()))
					texLevelTex = self.passes[-1].blend.texture
					checkTexLevel = True
				elif split[0] == 'diffuse':
					self.passes[-1].diffuse.texture.texture = Texture().FromFile(FindResource(line[len(split[0]):].lstrip()))
					texLevelTex = self.passes[-1].diffuse.texture
					checkTexLevel = True
				elif split[0] == 'emissive':
					self.passes[-1].emissive.texture.texture = Texture().FromFile(FindResource(line[len(split[0]):].lstrip()))
					texLevelTex = self.passes[-1].emissive.texture
					checkTexLevel = True
				elif split[0] == 'fresnel':
					self.passes[-1].fresnel.texture.texture = Texture().FromFile(FindResource(line[len(split[0]):].lstrip()))
					texLevelTex = self.passes[-1].fresnel.texture
					checkTexLevel = True
				elif split[0] == 'fresnel.exponent':
					self.passes[-1].fresnel.exponent.texture.texture = Texture().FromFile(FindResource(line[len(split[0]):].lstrip()))
					texLevelTex = self.passes[-1].fresnel.exponent.texture
					checkTexLevel = True
				elif split[0] == 'gloss':
					self.passes[-1].gloss.texture.texture = Texture().FromFile(FindResource(line[len(split[0]):].lstrip()))
					texLevelTex = self.passes[-1].gloss.texture
					checkTexLevel = True
				elif split[0] == 'normal':
					self.passes[-1].normal.texture.texture = Texture().FromFile(FindResource(line[len(split[0]):].lstrip()))
					texLevelTex = self.passes[-1].normal.texture
					checkTexLevel = True
				elif split[0] == 'specular':
					self.passes[-1].specular.texture.texture = Texture().FromFile(FindResource(line[len(split[0]):].lstrip()))
					texLevelTex = self.passes[-1].specular.texture
					checkTexLevel = True
				elif split[0] == 'specular.exponent':
					self.passes[-1].specular.exponent.texture.texture = Texture().FromFile(FindResource(line[len(split[0]):].lstrip()))
					texLevelTex = self.passes[-1].specular.exponent.texture
					checkTexLevel = True
				elif split[0] == '}':
					level = ROOT_LEVEL
				elif split[0] == '{':
					raise FormatError(path + ': unexpected opening brace')
				else: raise FormatError(path + ': unrecognized token')
			elif level == TEX_LEVEL:
				if split[0] == 'shift':
					if len(split) == 2:
						texLevelTex.shift = (float(split[1]), float(split[1]))
					else:
						texLevelTex.shift = (float(split[1]), float(split[2]))
				elif split[0] == 'scale':
					if len(split) == 2:
						texLevelTex.scale = (float(split[1]), float(split[1]))
					else:
						texLevelTex.scale = (float(split[1]), float(split[2]))
				elif split[0] == '}':
					level = PASS_LEVEL
				elif split[0] == '{':
					raise FormatError(path + ': unexpected opening brace')
				else: raise FormatError(path + ': unrecognized token')
		return self

################################################################################
# camera

class Camera:
	def __init__(self, position = (0, 0, 0), orientation = (-45, 45), distance = 10):
		self.position = position
		self.orientation = orientation
		self.distance = distance

	def Translate(self, translation):
		self.position = Add(self.position, translation)
	def Rotate(self, rotation):
		self.orientation = Add(self.orientation, rotation)
	def Scale(self, scale):
		self.distance = min(max(self.distance - scale, 1), 20)

	def MultProjMatrix(self, aspect):
		gluPerspective(prefs['camera.fov'], aspect, 0.05, 500)
	def MultViewMatrix(self):
		glTranslate(0, 0, -pow(1.2, self.distance))
		MultInvEulerMatrix(self.orientation)
		glTranslate(*self.position)

camera = Camera()

################################################################################
# ALL OF THE REMAINING CODE IS DEPRECIATED!

def Add(a, b): return ApplyBinary(a, b, operator.add)
def Sub(a, b): return ApplyBinary(a, b, operator.sub)
def Mul(a, b): return ApplyBinary(a, b, operator.mul)
def Div(a, b): return ApplyBinary(a, b, operator.div)
def Mod(a, b): return ApplyBinary(a, b, operator.mod)
def Neg(a): return ApplyUnary(a, operator.neg)
def Pos(a): return ApplyUnary(a, operator.pos)
def Abs(a): return ApplyUnary(a, operator.abs)
def Inv(a): return ApplyUnary(a, operator.inv)
def Int(a): return ApplyUnary(a, lambda a: int(a))
def Float(a): return ApplyUnary(a, lambda a: float(a))

def Dot(a, b):
	return sum(map(lambda a, b: a * b, a, b))

def Cross(a, b = None):
	if len(a) == 2:
		assert b is None
		return [-b, a]
	assert len(a) == len(b) == 3
	return Sub(
		Mul((a[1], a[2], a[0]), (b[2], b[0], b[1])),
		Mul((a[2], a[0], a[1]), (b[1], b[2], b[0])))

def Len(v):
	return math.sqrt(Dot(v, v))

def Norm(v):
	return Div(v, Len(v))

def ApplyBinary(a, b, func):
	aIterable = hasattr(a, '__iter__') or hasattr(a, '__getitem__')
	bIterable = hasattr(b, '__iter__') or hasattr(b, '__getitem__')
	if not aIterable and not bIterable: return func(a, b)
	if not aIterable: a = itertools.repeat(a)
	elif not bIterable: b = itertools.repeat(b)
	else:
		def Guard(a, b):
			if a is None: return b
			if b is None: return a
			return func(a, b)
		return map(Guard, a, b)
	return [func(a, b) for a, b in zip(a, b)]

def ApplyUnary(a, func):
	if not hasattr(a, '__iter__') and not hasattr(a, '__getitem__'):
		return func(a)
	return map(func, a)

################################################################################
# euler operations

# FIXME: implement

################################################################################
# quaternion operations

def InvQuat(a):
	return (-a[0], -a[1], -a[2], a[3])

def MulQuat(a, b):
	return [
		a[3] * b[0] + a[0] * b[3] + a[1] * b[2] - a[2] * b[1],
		a[3] * b[1] + a[1] * b[3] + a[2] * b[0] - a[0] * b[2],
		a[3] * b[2] + a[2] * b[3] + a[0] * b[1] - a[1] * b[0],
		a[3] * b[3] - a[0] * b[0] - a[1] * b[1] - a[2] * b[2]]

################################################################################
# matrix operations

# FIXME: implement
