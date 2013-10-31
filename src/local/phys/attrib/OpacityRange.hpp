#ifndef    page_local_phys_attrib_OpacityRange_hpp
#   define page_local_phys_attrib_OpacityRange_hpp

#	include "../Frame.hpp"

namespace page { namespace phys { namespace attrib
{
	class OpacityRange
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit OpacityRange(float = 1);
		OpacityRange(float min, float max);

		/*----------+
		| accessors |
		+----------*/

		float GetMinOpacity() const;
		float GetMaxOpacity() const;
		void SetMinOpacity(float);
		void SetMaxOpacity(float);
		void SetOpacityRange(float);
		void SetOpacityRange(float min, float max);

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
