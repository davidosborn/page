#ifndef    page_local_phys_attrib_Opacity_hpp
#   define page_local_phys_attrib_Opacity_hpp

#	include "../Frame.hpp"

namespace page { namespace phys { namespace attrib
{
	class Opacity
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit Opacity(float = 1);

		/*----------+
		| accessors |
		+----------*/

		float GetOpacity() const;
		void SetOpacity(float);

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
