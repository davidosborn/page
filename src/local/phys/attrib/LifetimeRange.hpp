#ifndef    page_local_phys_attrib_LifetimeRange_hpp
#   define page_local_phys_attrib_LifetimeRange_hpp

#	include "../Frame.hpp"

namespace page { namespace phys { namespace attrib
{
	class LifetimeRange
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit LifetimeRange(float = 0);
		LifetimeRange(float min, float max);

		/*----------+
		| accessors |
		+----------*/

		float GetMinLifetime() const;
		float GetMaxLifetime() const;
		void SetMinLifetime(float);
		void SetMaxLifetime(float);
		void SetLifetimeRange(float);
		void SetLifetimeRange(float min, float max);

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
