#ifndef    page_local_aud_openal_Channel_hpp
#   define page_local_aud_openal_Channel_hpp

#	include <memory> // shared_ptr

#	include <AL/al.h> // ALuint

#	include "../../cache/proxy/fwd.hpp" // Proxy
#	include "../Channel.hpp"

namespace page { namespace res { class Sound; }}

namespace page { namespace aud { namespace openal
{
	class Buffer;

	struct Channel : virtual aud::Channel
	{
		// construct/destroy
		Channel(const cache::Proxy<res::Sound> &, bool loop, float playPosition);
		~Channel();

		protected:
		// source access
		ALuint GetSource() const;

		private:
		// update
		void DoUpdate2();
		virtual void DoUpdate3() = 0;

		ALuint source;
		std::shared_ptr<Buffer> buffer;
	};
}}}

#endif
