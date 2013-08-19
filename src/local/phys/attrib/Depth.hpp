#ifndef    page_local_phys_attrib_Depth_hpp
#   define page_local_phys_attrib_Depth_hpp

#	include "../../math/float.hpp" // Inf
#	include "../Frame.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			struct Depth
			{
				// construct
				explicit Depth(float = math::Inf());

				// access
				float GetDepth() const;
				void SetDepth(float);

				protected:
				// frame serialization
				Frame GetFrame() const;
				void Update(const Frame &);

				private:
				float depth;
			};
		}
	}
}

#endif
