#include "EmissiveRange.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// construct
			EmissiveRange::EmissiveRange(const math::RgbColor<> &val) :
				min(val), max(val) {}
			EmissiveRange::EmissiveRange(const math::RgbColor<> &min, const math::RgbColor<> &max) :
				min(min), max(max) {}

			// access
			const math::RgbColor<> &EmissiveRange::GetMinEmissive() const
			{
				return min;
			}
			const math::RgbColor<> &EmissiveRange::GetMaxEmissive() const
			{
				return max;
			}
			void EmissiveRange::SetMinEmissive(const math::RgbColor<> &min)
			{
				this->min = min;
			}
			void EmissiveRange::SetMaxEmissive(const math::RgbColor<> &max)
			{
				this->max = max;
			}
			void EmissiveRange::SetEmissiveRange(const math::RgbColor<> &min, const math::RgbColor<> &max)
			{
				this->min = min;
				this->max = max;
			}

			// frame serialization
			Frame EmissiveRange::GetFrame() const
			{
				Frame frame;
				frame.emissiveRange = Frame::Range<math::RgbColor<>>(min, max);
				return frame;
			}
			void EmissiveRange::Update(const Frame &frame)
			{
				if (frame.emissiveRange)
				{
					min = frame.emissiveRange->min;
					max = frame.emissiveRange->max;
				}
			}
		}
	}
}
