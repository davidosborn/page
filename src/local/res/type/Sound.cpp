#include <cassert>
#include <climits> // CHAR_BIT

#include "Registry.hpp" // REGISTER_TYPE
#include "Sound.hpp"

namespace page
{
	namespace res
	{
		// attributes
		unsigned GetSize(const Sound &sound)
		{
			return (
				sound.channels *
				sound.bitDepth *
				sound.samples + CHAR_BIT - 1) / CHAR_BIT;
		}
		float GetDuration(const Sound &sound)
		{
			return static_cast<float>(sound.samples) / sound.frequency;
		}

		void PostLoadSound(Sound &sound)
		{
			// ensure sound is fully defined if there are any samples
			assert(
				sound.channels  &&
				sound.bitDepth  &&
				sound.frequency &&
				sound.samples   &&
				sound.decoder || !sound.samples);
		}

		REGISTER_TYPE(Sound, "sound", PostLoadSound)
	}
}
