#ifndef    page_local_phys_attrib_Aspect_hpp
#   define page_local_phys_attrib_Aspect_hpp

#	include "../Frame.hpp"

namespace page { namespace phys { namespace attrib
{
	class Aspect
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit Aspect(float = 4.f / 3);

		/*----------+
		| accessors |
		+----------*/

		float GetAspect() const;
		void SetAspect(float);

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
