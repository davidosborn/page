#include "Depth.hpp"

namespace page { namespace phys { namespace attrib
{
	/*-------------+
	| constructors |
	+-------------*/

	Depth::Depth(float value) :
		value(value) {}

	/*----------+
	| accessors |
	+----------*/

	float Depth::GetDepth() const
	{
		return value;
	}

	void Depth::SetDepth(float value)
	{
		this->value = value;
	}

	/*--------------------+
	| frame serialization |
	+--------------------*/

	Frame Depth::GetFrame() const
	{
		Frame frame;
		frame.depth = value;
		return frame;
	}

	void Depth::SetFrame(const Frame &frame)
	{
		if (frame.depth) value = *frame.depth;
	}
}}}
