#include "Volume.hpp"

namespace page { namespace phys { namespace attrib
{
	/*-------------+
	| constructors |
	+-------------*/

	Volume::Volume(float value) :
		value(value) {}

	/*----------+
	| accessors |
	+----------*/

	float Volume::GetVolume() const
	{
		return value;
	}

	void Volume::SetVolume(float value)
	{
		this->value = value;
	}

	/*--------------------+
	| frame serialization |
	+--------------------*/

	Frame Volume::GetFrame() const
	{
		Frame frame;
		frame.volume = value;
		return frame;
	}

	void Volume::SetFrame(const Frame &frame)
	{
		if (frame.volume) value = *frame.volume;
	}
}}}
