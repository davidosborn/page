#include "Ambient.hpp"

namespace page { namespace phys { namespace attrib
{
	/*-------------+
	| constructors |
	+-------------*/

	Ambient::Ambient(const math::RgbColor<> &value) :
		value(value) {}

	/*----------+
	| accessors |
	+----------*/

	const math::RgbColor<> &Ambient::GetAmbient() const
	{
		return value;
	}

	void Ambient::SetAmbient(const math::RgbColor<> &value)
	{
		this->value = value;
	}

	/*--------------------+
	| frame serialization |
	+--------------------*/

	Frame Ambient::GetFrame() const
	{
		Frame frame;
		frame.ambient = value;
		return frame;
	}

	void Ambient::SetFrame(const Frame &frame)
	{
		if (frame.ambient)
			value = *frame.ambient;
	}
}}}
