#include "Diffuse.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// construct
			Diffuse::Diffuse(const math::RgbColor<> &diffuse) :
				diffuse(diffuse) {}

			// access
			const math::RgbColor<> &Diffuse::GetDiffuse() const
			{
				return diffuse;
			}
			void Diffuse::SetDiffuse(const math::RgbColor<> &diffuse)
			{
				this->diffuse = diffuse;
			}

			// frame serialization
			Frame Diffuse::GetFrame() const
			{
				Frame frame;
				frame.diffuse = diffuse;
				return frame;
			}
			void Diffuse::Update(const Frame &frame)
			{
				if (frame.diffuse) diffuse = *frame.diffuse;
			}
		}
	}
}
