#include "Diffuse.hpp"

namespace page { namespace phys { namespace attrib
{
	/*-------------+
	| constructors |
	+-------------*/

	Diffuse::Diffuse(const math::RgbColor<> &value) :
		value(value) {}

	/*----------+
	| accessors |
	+----------*/

	const math::RgbColor<> &Diffuse::GetDiffuse() const
	{
		return value;
	}

	void Diffuse::SetDiffuse(const math::RgbColor<> &value)
	{
		this->value = value;
	}

	/*--------------------+
	| frame serialization |
	+--------------------*/

	Frame Diffuse::GetFrame() const
	{
		Frame frame;
		frame.diffuse = value;
		return frame;
	}

	void Diffuse::SetFrame(const Frame &frame)
	{
		if (frame.diffuse) value = *frame.diffuse;
	}
}}}
