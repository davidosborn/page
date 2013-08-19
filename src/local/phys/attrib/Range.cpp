#include "Range.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// construct
			Range::Range(float range) : range(range) {}

			// access
			float Range::GetRange() const
			{
				return range;
			}
			void Range::SetRange(float range)
			{
				this->range = range;
			}

			// frame serialization
			Frame Range::GetFrame() const
			{
				Frame frame;
				frame.range = range;
				return frame;
			}
			void Range::Update(const Frame &frame)
			{
				if (frame.range) range = *frame.range;
			}
		}
	}
}
