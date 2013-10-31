#ifndef    page_local_phys_attrib_PositionOrientation_hpp
#   define page_local_phys_attrib_PositionOrientation_hpp

#	include "Orientation.hpp"
#	include "Position.hpp"

namespace page { namespace phys { namespace attrib
{
	class PositionOrientation :
		public Position,
		public Orientation
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit PositionOrientation(
			const math::Vec3 & = 0,
			const math::Quat<> & = math::Quat<>());
		PositionOrientation(
			const math::Vec3 &,
			const math::Vec3 &);
		explicit PositionOrientation(const math::Mat34 &);

		/*----------+
		| accessors |
		+----------*/

		math::Mat34 GetMatrix() const;
		math::Mat34 GetInvMatrix() const;
		void SetMatrix(const math::Mat34 &);

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
	};
}}}

#endif
