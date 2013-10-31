#include <algorithm> // max, min
#include "Fov.hpp"

namespace page { namespace phys { namespace attrib
{
	/*-------------+
	| constructors |
	+-------------*/

	Fov::Fov(float value) :
		value(value) {}

	/*----------+
	| accessors |
	+----------*/

	float Fov::GetFov() const
	{
		return value;
	}

	void Fov::SetFov(float value)
	{
		this->value = std::min(std::max(value, 0.f), 180.f);
	}

	/*--------------------+
	| frame serialization |
	+--------------------*/

	Frame Fov::GetFrame() const
	{
		Frame frame;
		frame.fov = value;
		return frame;
	}

	void Fov::SetFrame(const Frame &frame)
	{
		if (frame.fov) value = *frame.fov;
	}
}}}
