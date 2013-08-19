#include "Depth.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// construct
			Depth::Depth(float depth) : depth(depth) {}

			// access
			float Depth::GetDepth() const
			{
				return depth;
			}
			void Depth::SetDepth(float depth)
			{
				this->depth = depth;
			}

			// frame serialization
			Frame Depth::GetFrame() const
			{
				Frame frame;
				frame.depth = depth;
				return frame;
			}
			void Depth::Update(const Frame &frame)
			{
				if (frame.depth) depth = *frame.depth;
			}
		}
	}
}
