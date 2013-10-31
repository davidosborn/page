#ifndef    page_local_phys_attrib_Range_hpp
#   define page_local_phys_attrib_Range_hpp

#	include "../Frame.hpp"

namespace page { namespace phys { namespace attrib
{
	class Range
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit Range(float = 1);

		/*----------+
		| accessors |
		+----------*/

		float GetRange() const;
		void SetRange(float);

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
