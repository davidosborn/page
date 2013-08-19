#include "Emissive.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// construct
			Emissive::Emissive(const math::RgbColor<> &emissive) :
				emissive(emissive) {}

			// access
			const math::RgbColor<> &Emissive::GetEmissive() const
			{
				return emissive;
			}
			void Emissive::SetEmissive(const math::RgbColor<> &emissive)
			{
				this->emissive = emissive;
			}

			// frame serialization
			Frame Emissive::GetFrame() const
			{
				Frame frame;
				frame.emissive = emissive;
				return frame;
			}
			void Emissive::Update(const Frame &frame)
			{
				if (frame.emissive) emissive = *frame.emissive;
			}
		}
	}
}
