#ifndef    page_local_phys_attrib_Volume_hpp
#   define page_local_phys_attrib_Volume_hpp

#	include "../Frame.hpp"

namespace page { namespace phys { namespace attrib
{
	class Volume
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit Volume(float = 0);

		/*----------+
		| accessors |
		+----------*/

		float GetVolume() const;
		void SetVolume(float);

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
