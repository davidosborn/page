#ifndef    page_local_phys_attrib_SizeRange_hpp
#   define page_local_phys_attrib_SizeRange_hpp

#	include "../Frame.hpp"

namespace page { namespace phys { namespace attrib
{
	class SizeRange
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit SizeRange(float = 1);
		SizeRange(float min, float max);

		/*----------+
		| accessors |
		+----------*/

		float GetMinSize() const;
		float GetMaxSize() const;
		void SetMinSize(float);
		void SetMaxSize(float);
		void SetSizeRange(float);
		void SetSizeRange(float min, float max);

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
		float min, max;
	};
}}}

#endif
