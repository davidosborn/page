#include "Ambient.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// construct
			Ambient::Ambient(const math::RgbColor<> &ambient) :
				ambient(ambient) {}

			// access
			const math::RgbColor<> &Ambient::GetAmbient() const
			{
				return ambient;
			}
			void Ambient::SetAmbient(const math::RgbColor<> &ambient)
			{
				this->ambient = ambient;
			}

			// frame serialization
			Frame Ambient::GetFrame() const
			{
				Frame frame;
				frame.ambient = ambient;
				return frame;
			}
			void Ambient::Update(const Frame &frame)
			{
				if (frame.ambient) ambient = *frame.ambient;
			}
		}
	}
}
