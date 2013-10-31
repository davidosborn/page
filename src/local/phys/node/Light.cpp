#include <algorithm> // max

#include "Light.hpp"

namespace page
{
	namespace phys
	{
		// attenuation state
		float Light::GetMinRange() const
		{
			return std::max(GetRange() - GetFalloff() / 2, 0.f);
		}
		float Light::GetMaxRange() const
		{
			return GetRange() + GetFalloff() / 2;
		}

		// frame serialization
		Frame Light::GetFrame() const
		{
			return
				Ambient::GetFrame() +
				Attenuation::GetFrame() +
				Cutoff::GetFrame() +
				Diffuse::GetFrame() +
				Falloff::GetFrame() +
				PositionOrientation::GetFrame() +
				Range::GetFrame() +
				Specular::GetFrame();
		}
		void Light::Update(const Frame &frame)
		{
			Ambient::Update(frame);
			Attenuation::Update(frame);
			Cutoff::Update(frame);
			Diffuse::Update(frame);
			Falloff::Update(frame);
			PositionOrientation::Update(frame);
			Range::Update(frame);
			Specular::Update(frame);
		}
	}
}
