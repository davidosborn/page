#include "LifetimeRange.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// construct
			LifetimeRange::LifetimeRange(float val) : min(val), max(val) {}
			LifetimeRange::LifetimeRange(float min, float max) :
				min(min), max(max) {}

			// access
			float LifetimeRange::GetMinLifetime() const
			{
				return min;
			}
			float LifetimeRange::GetMaxLifetime() const
			{
				return max;
			}
			void LifetimeRange::SetMinLifetime(float min)
			{
				this->min = min;
			}
			void LifetimeRange::SetMaxLifetime(float max)
			{
				this->max = max;
			}
			void LifetimeRange::SetLifetimeRange(float min, float max)
			{
				this->min = min;
				this->max = max;
			}

			// frame serialization
			Frame LifetimeRange::GetFrame() const
			{
				Frame frame;
				frame.lifetimeRange = Frame::Range<float>(min, max);
				return frame;
			}
			void LifetimeRange::Update(const Frame &frame)
			{
				if (frame.lifetimeRange)
				{
					min = frame.lifetimeRange->min;
					max = frame.lifetimeRange->max;
				}
			}
		}
	}
}
