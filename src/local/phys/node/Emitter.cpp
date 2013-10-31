#include "Emitter.hpp"

namespace page { namespace phys
{
	// update
	void Emitter::Update()
	{
		// FIXME: create particles
	}

	/*--------------------+
	| frame serialization |
	+--------------------*/

	Frame Emitter::GetFrame() const
	{
		return
			AmbientRange       ::GetFrame() +
			Cutoff             ::GetFrame() +
			DiffuseRange       ::GetFrame() +
			EmissiveRange      ::GetFrame() +
			LifetimeRange      ::GetFrame() +
			OpacityRange       ::GetFrame() +
			PositionOrientation::GetFrame() +
			SpecularRange      ::GetFrame() +
			SizeRange          ::GetFrame() +
			SpeedRange         ::GetFrame();
	}

	void Emitter::SetFrame(const Frame &frame)
	{
		AmbientRange       ::SetFrame(frame);
		Cutoff             ::SetFrame(frame);
		DiffuseRange       ::SetFrame(frame);
		EmissiveRange      ::SetFrame(frame);
		LifetimeRange      ::SetFrame(frame);
		OpacityRange       ::SetFrame(frame);
		PositionOrientation::SetFrame(frame);
		SpecularRange      ::SetFrame(frame);
		SizeRange          ::SetFrame(frame);
		SpeedRange         ::SetFrame(frame);
	}
}
}
