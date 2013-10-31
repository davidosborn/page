#include "Aspect.hpp"

namespace page { namespace phys { namespace attrib
{
	/*-------------+
	| constructors |
	+-------------*/

	Aspect::Aspect(float value) :
		value(value) {}

	/*----------+
	| accessors |
	+----------*/

	float Aspect::GetAspect() const
	{
		return value;
	}

	void Aspect::SetAspect(float value)
	{
		this->value = value;
	}

	/*--------------------+
	| frame serialization |
	+--------------------*/

	Frame Aspect::GetFrame() const
	{
		Frame frame;
		frame.aspect = value;
		return frame;
	}

	void Aspect::SetFrame(const Frame &frame)
	{
		if (frame.aspect)
			value = *frame.aspect;
	}
}}}
