#ifndef    page_local_cache_proxy_openal_BufferProxy_hpp
#   define page_local_cache_proxy_openal_BufferProxy_hpp

#	include <AL/al.h> // ALuint

#	include "../BasicProxy.hpp"
#	include "../Proxy.hpp"

namespace page { namespace res { class Sound; }}

namespace page { namespace cache { namespace openal
{
	/**
	 * A proxy representing an OpenAL buffer in the cache.
	 */
	class BufferProxy :
		public BasicProxy<ALuint>,
		public virtual util::Cloneable<BufferProxy, BasicProxy<ALuint>>
	{
		/*-------+
		| traits |
		+-------*/

		public:
		using BasicProxy<ALuint>::pointer;

		/*-------------+
		| constructors |
		+-------------*/

		explicit BufferProxy(const Proxy<res::Sound> &);

		/*--------------------------+
		| BasicProxy implementation |
		+--------------------------*/

		private:
		pointer DoLock() const override;

		/*-------------+
		| data members |
		+-------------*/

		Proxy<res::Sound> sound;
	};
}}}

#endif
