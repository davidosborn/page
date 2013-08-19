#include "SpecularRange.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// construct
			SpecularRange::SpecularRange(const math::RgbColor<> &val) :
				min(val), max(val) {}
			SpecularRange::SpecularRange(const math::RgbColor<> &min, const math::RgbColor<> &max) :
				min(min), max(max) {}

			// access
			const math::RgbColor<> &SpecularRange::GetMinSpecular() const
			{
				return min;
			}
			const math::RgbColor<> &SpecularRange::GetMaxSpecular() const
			{
				return max;
			}
			void SpecularRange::SetMinSpecular(const math::RgbColor<> &min)
			{
				this->min = min;
			}
			void SpecularRange::SetMaxSpecular(const math::RgbColor<> &max)
			{
				this->max = max;
			}
			void SpecularRange::SetSpecularRange(const math::RgbColor<> &min, const math::RgbColor<> &max)
			{
				this->min = min;
				this->max = max;
			}

			// frame serialization
			Frame SpecularRange::GetFrame() const
			{
				Frame frame;
				frame.specularRange = Frame::Range<math::RgbColor<>>(min, max);
				return frame;
			}
			void SpecularRange::Update(const Frame &frame)
			{
				if (frame.specularRange)
				{
					min = frame.specularRange->min;
					max = frame.specularRange->max;
				}
			}
		}
	}
}
