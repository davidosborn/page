#ifndef    page_local_phys_attrib_Depth_hpp
#   define page_local_phys_attrib_Depth_hpp

#	include "../../math/float.hpp" // Inf
#	include "../Frame.hpp"

namespace page { namespace phys { namespace attrib
{
	class Depth
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit Depth(float = math::Inf());

		/*----------+
		| accessors |
		+----------*/

		float GetDepth() const;
		void SetDepth(float);

		/*--------------------+
		| frame serialization |
		+--------------------*/

		protected:
		Frame GetFrame() const;
		void SetFrame(const Frame &);

		/*-------------+
		| data members |
		+-------------*/

		private:
		float value;
	};
}}}

#endif
