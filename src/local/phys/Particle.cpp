#include "Particle.hpp"

namespace page
{
	namespace phys
	{
		// construct
		Particle::Particle(const cache::Proxy<res::Material> &material) :
			Material(material) {}

		// frame serialization
		// FIXME: should Particle be Controllable?
		/*Frame Particle::GetFrame() const
		{
			return
				Ambient::GetFrame() +
				Diffuse::GetFrame() +
				Emissive::GetFrame() +
				Position::GetFrame() +
				Opacity::GetFrame() +
				Size::GetFrame() +
				Specular::GetFrame();
		}
		void Particle::Update(const Frame &frame)
		{
			Ambient::Update(frame);
			Diffuse::Update(frame);
			Emissive::Update(frame);
			Position::Update(frame);
			Opacity::Update(frame);
			Size::Update(frame);
			Specular::Update(frame);
		}*/
	}
}
