#ifndef    page_local_phys_attrib_LifetimeRange_hpp
#   define page_local_phys_attrib_LifetimeRange_hpp

#	include "../Frame.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			struct LifetimeRange
			{
				// construct
				explicit LifetimeRange(float = 0);
				LifetimeRange(float min, float max);

				// access
				float GetMinLifetime() const;
				float GetMaxLifetime() const;
				void SetMinLifetime(float);
				void SetMaxLifetime(float);
				void SetLifetimeRange(float min, float max);

				protected:
				// frame serialization
				Frame GetFrame() const;
				void Update(const Frame &);

				private:
				float min, max;
			};
		}
	}
}

#endif
