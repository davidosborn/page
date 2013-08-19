#ifndef    page_local_phys_attrib_Volume_hpp
#   define page_local_phys_attrib_Volume_hpp

#	include "../Frame.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			struct Volume
			{
				// construct
				explicit Volume(float = 0);

				// access
				float GetVolume() const;
				void SetVolume(float);

				protected:
				// frame serialization
				Frame GetFrame() const;
				void Update(const Frame &);

				private:
				float volume;
			};
		}
	}
}

#endif
