#ifndef    page_local_phys_attrib_Size_hpp
#   define page_local_phys_attrib_Size_hpp

#	include "../Frame.hpp"

namespace page { namespace phys { namespace attrib
{
	class Size
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit Size(float = 1);

		/*----------+
		| accessors |
		+----------*/

		float GetSize() const;
		void SetSize(float);

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
