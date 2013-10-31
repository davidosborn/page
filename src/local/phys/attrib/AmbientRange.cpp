#include "AmbientRange.hpp"

namespace page { namespace phys { namespace attrib
{
	/*-------------+
	| constructors |
	+-------------*/

	AmbientRange::AmbientRange(const math::RgbColor<> &value) :
		min(value), max(value) {}

	AmbientRange::AmbientRange(const math::RgbColor<> &min, const math::RgbColor<> &max) :
		min(min), max(max) {}

	/*----------+
	| accessors |
	+----------*/

	const math::RgbColor<> &AmbientRange::GetMinAmbient() const
	{
		return min;
	}

	const math::RgbColor<> &AmbientRange::GetMaxAmbient() const
	{
		return max;
	}

	void AmbientRange::SetMinAmbient(const math::RgbColor<> &min)
	{
		this->min = min;
	}

	void AmbientRange::SetMaxAmbient(const math::RgbColor<> &max)
	{
		this->max = max;
	}

	void AmbientRange::SetAmbientRange(const math::RgbColor<> &value)
	{
		this->min = value;
		this->max = value;
	}

	void AmbientRange::SetAmbientRange(const math::RgbColor<> &min, const math::RgbColor<> &max)
	{
		this->min = min;
		this->max = max;
	}

	/*--------------------+
	| frame serialization |
	+--------------------*/

	Frame AmbientRange::GetFrame() const
	{
		Frame frame;
		frame.ambientRange = Frame::Range<math::RgbColor<>>(min, max);
		return frame;
	}

	void AmbientRange::SetFrame(const Frame &frame)
	{
		if (frame.ambientRange)
		{
			min = frame.ambientRange->min;
			max = frame.ambientRange->max;
		}
	}
}}}
