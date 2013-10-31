#ifndef    page_local_phys_attrib_Exposure_hpp
#   define page_local_phys_attrib_Exposure_hpp

#	include "../Frame.hpp"

namespace page { namespace phys { namespace attrib
{
	class Exposure
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit Exposure(float = 0);

		/*----------+
		| accessors |
		+----------*/

		float GetExposure() const;
		void SetExposure(float);

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
