#include "Cutoff.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// construct
			Cutoff::Cutoff(float cutoff) : cutoff(cutoff) {}

			// access
			float Cutoff::GetCutoff() const
			{
				return cutoff;
			}
			void Cutoff::SetCutoff(float cutoff)
			{
				this->cutoff = cutoff;
			}

			// frame serialization
			Frame Cutoff::GetFrame() const
			{
				Frame frame;
				frame.cutoff = cutoff;
				return frame;
			}
			void Cutoff::Update(const Frame &frame)
			{
				if (frame.cutoff) cutoff = *frame.cutoff;
			}
		}
	}
}
