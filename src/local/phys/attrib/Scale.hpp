#ifndef    page_local_phys_attrib_Scale_hpp
#   define page_local_phys_attrib_Scale_hpp

#	include "../../math/Matrix.hpp"
#	include "../../math/Vector.hpp"
#	include "../Frame.hpp"
#	include "../mixin/Transformable.hpp"

namespace page { namespace phys { namespace attrib
{
	class Scale :
		public virtual Transformable
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit Scale(const math::Vec3 & = 1);
		explicit Scale(const math::Mat3 &);

		/*----------+
		| accessors |
		+----------*/

		const math::Vec3 &GetScale() const;
		void SetScale(const math::Vec3 &);

		// matrix view
		math::Mat3 GetMatrix() const;
		math::Mat3 GetInvMatrix() const;
		void SetMatrix(const math::Mat3 &);

		// transformation observers
		const math::Vec3 &GetLastScale() const;
		const math::Vec3 &GetScaleForce() const;
		const math::Vec3 &GetDeltaScale() const;

		/*--------------------+
		| frame serialization |
		+--------------------*/

		protected:
		Frame GetFrame() const;
		void SetFrame(const Frame &);

		/*-----------------------------+
		| Transformable implementation |
		+-----------------------------*/

		public:
		void BakeTransform() override;
		void UpdateForce() override;
		void UpdateDelta() override;

		/*-------------+
		| data members |
		+-------------*/

		private:
		math::Vec3 value, lastValue = value;
		math::Vec3 force = 1, delta = 1;
	};
}}}

#endif
