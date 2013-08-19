#include "Aspect.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// construct
			Aspect::Aspect(float aspect) : aspect(aspect) {}

			// access
			float Aspect::GetAspect() const
			{
				return aspect;
			}
			void Aspect::SetAspect(float aspect)
			{
				this->aspect = aspect;
			}

			// frame serialization
			Frame Aspect::GetFrame() const
			{
				Frame frame;
				frame.aspect = aspect;
				return frame;
			}
			void Aspect::Update(const Frame &frame)
			{
				if (frame.aspect) aspect = *frame.aspect;
			}
		}
	}
}
