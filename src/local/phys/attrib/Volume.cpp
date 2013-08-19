#include "Volume.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// construct
			Volume::Volume(float volume) : volume(volume) {}

			// access
			float Volume::GetVolume() const
			{
				return volume;
			}
			void Volume::SetVolume(float volume)
			{
				this->volume = volume;
			}

			// frame serialization
			Frame Volume::GetFrame() const
			{
				Frame frame;
				frame.volume = volume;
				return frame;
			}
			void Volume::Update(const Frame &frame)
			{
				if (frame.volume) volume = *frame.volume;
			}
		}
	}
}
