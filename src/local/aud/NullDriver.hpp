#ifndef    page_local_aud_NullDriver_hpp
#   define page_local_aud_NullDriver_hpp

#	include "Driver.hpp"

namespace page { namespace aud
{
	struct NullDriver : Driver
	{
		// construct
		explicit NullDriver(wnd::Window &);

		// modifiers
		void SetVolume(float);

		private:
		// limits
		unsigned MaxAmbientChannels() const;
		unsigned MaxSpatialChannels() const;
		unsigned MaxPersistentSpatialChannels() const;

		// channel factory functions
		AmbientChannel *MakeAmbientChannel(const Sound &) const;
		SpatialChannel *MakeSpatialChannel(const phys::Sound &) const;
	};
}}

#endif
