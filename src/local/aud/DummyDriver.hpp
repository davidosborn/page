#ifndef    page_local_aud_DummyDriver_hpp
#   define page_local_aud_DummyDriver_hpp

#	include "Driver.hpp"

namespace page { namespace aud
{
	/**
	 * An audio driver that doesn't do anything, which can be used as a last
	 * resort when no other drivers are available.
	 */
	class DummyDriver : public Driver
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit DummyDriver(wnd::Window &);

		/*----------------------+
		| Driver implementation |
		+----------------------*/

		void SetVolume(float) override;

		unsigned MaxAmbientChannels() const override;
		unsigned MaxSpatialChannels() const override;
		unsigned MaxPersistentSpatialChannels() const override;

		AmbientChannel *MakeAmbientChannel(const Sound &) const override;
		SpatialChannel *MakeSpatialChannel(const phys::Sound &) const override;
	};
}}

#endif
