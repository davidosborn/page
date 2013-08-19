#include "Emitter.hpp"

namespace page
{
	namespace phys
	{
		// update
		void Emitter::Update()
		{
			// FIXME: create particles
		}

		// frame serialization
		Frame Emitter::GetFrame() const
		{
			return
				AmbientRange::GetFrame() +
				Cutoff::GetFrame() +
				DiffuseRange::GetFrame() +
				EmissiveRange::GetFrame() +
				LifetimeRange::GetFrame() +
				OpacityRange::GetFrame() +
				PositionOrientation::GetFrame() +
				SpecularRange::GetFrame() +
				SizeRange::GetFrame() +
				SpeedRange::GetFrame();
		}
		void Emitter::Update(const Frame &frame)
		{
			AmbientRange::Update(frame);
			Cutoff::Update(frame);
			DiffuseRange::Update(frame);
			EmissiveRange::Update(frame);
			LifetimeRange::Update(frame);
			OpacityRange::Update(frame);
			PositionOrientation::Update(frame);
			SpecularRange::Update(frame);
			SizeRange::Update(frame);
			SpeedRange::Update(frame);
		}
	}
}
