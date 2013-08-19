#include "Size.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// construct
			Size::Size(float size) : size(size) {}

			// access
			float Size::GetSize() const
			{
				return size;
			}
			void Size::SetSize(float size)
			{
				this->size = size;
			}

			// frame serialization
			Frame Size::GetFrame() const
			{
				Frame frame;
				frame.size = size;
				return frame;
			}
			void Size::Update(const Frame &frame)
			{
				if (frame.size) size = *frame.size;
			}
		}
	}
}
