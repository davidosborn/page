#ifndef    page_local_phys_attrib_Position_hpp
#   define page_local_phys_attrib_Position_hpp

#	include "../../math/Matrix.hpp"
#	include "../../math/Vector.hpp"
#	include "../Frame.hpp"
#	include "../mixin/Positionable.hpp"
#	include "../mixin/Transformable.hpp"

namespace page { namespace phys { namespace attrib
{
	class Position :
		public virtual Positionable,
		public virtual Transformable
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit Position(const math::Vec3 & = 0);
		explicit Position(const math::Mat34 &);

		/*----------+
		| accessors |
		+----------*/

		const math::Vec3 &GetPosition() const;
		void SetPosition(const math::Vec3 &);

		// matrix view
		math::Mat34 GetMatrix() const;
		math::Mat34 GetInvMatrix() const;
		void SetMatrix(const math::Mat34 &);

		// transformation observers
		const math::Vec3 &GetLastPosition() const;
		const math::Vec3 &GetForce() const;
		const math::Vec3 &GetVelocity() const;

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
		math::Vec3 force, velocity;
	};
}}}

#endif
