#ifndef    page_local_phys_Sound_hpp
#   define page_local_phys_Sound_hpp

#	include "../cache/fwd.hpp" // Proxy
#	include "../util/Identifiable.hpp"
#	include "../util/raii/copy_ptr.hpp"
#	include "attrib/Position.hpp"
#	include "attrib/Volume.hpp"
#	include "mixin/Controllable.hpp"

namespace page
{
	namespace res { class Sound; }

	namespace phys
	{
		struct Sound : Controllable, util::Identifiable,
			attrib::Position,
			attrib::Volume
		{
			// construct
			explicit Sound(const cache::Proxy<res::Sound> &, bool loop = false);

			// state
			bool IsPlaying() const;
			bool IsLooping() const;
			float GetPlayPosition() const;

			// sound access
			const cache::Proxy<res::Sound> &GetSound() const;

			// update
			void Update(float deltaTime);

			protected:
			// frame serialization
			Frame GetFrame() const;
			void Update(const Frame &);

			private:
			util::copy_ptr<cache::Proxy<res::Sound>> sound;
			bool playing, loop;
			float playPosition;
		};
	}
}

#endif
