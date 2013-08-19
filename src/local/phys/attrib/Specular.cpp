#include "Specular.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// construct
			Specular::Specular(const math::RgbColor<> &specular) :
				specular(specular) {}

			// access
			const math::RgbColor<> &Specular::GetSpecular() const
			{
				return specular;
			}
			void Specular::SetSpecular(const math::RgbColor<> &specular)
			{
				this->specular = specular;
			}

			// frame serialization
			Frame Specular::GetFrame() const
			{
				Frame frame;
				frame.specular = specular;
				return frame;
			}
			void Specular::Update(const Frame &frame)
			{
				if (frame.specular) specular = *frame.specular;
			}
		}
	}
}
