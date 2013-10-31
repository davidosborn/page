#ifndef    page_local_phys_node_Camera_hpp
#   define page_local_phys_node_Camera_hpp

#	include "../../math/float.hpp" // Inf
#	include "../../math/Frustum.hpp"
#	include "../../math/Matrix.hpp"
#	include "../../math/ViewFrustum.hpp"
#	include "../../res/type/CameraSet.hpp" // CameraSet::Camera
#	include "../attrib/Aspect.hpp"
#	include "../attrib/Depth.hpp"
#	include "../attrib/Exposure.hpp"
#	include "../attrib/Fov.hpp"
#	include "../attrib/Opacity.hpp"
#	include "../attrib/PositionOrientation.hpp"
#	include "../mixin/Controllable.hpp"
#	include "Node.hpp"

namespace page { namespace phys
{
	/**
	 * A camera node.
	 */
	class Camera :
		public Node,
		public Controllable,
		public attrib::Aspect,
		public attrib::Depth,
		public attrib::Exposure,
		public attrib::Fov,
		public attrib::Opacity,
		public attrib::PositionOrientation
	{
		IMPLEMENT_CLONEABLE(Camera, Node)

		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit Camera(
			const math::Vec3 &position = 0,
			const math::Quat<> &orientation = math::Quat<>(),
			float depth = math::Inf());

		Camera(
			const math::Vec3 &position,
			const math::Vec3 &focus);

		explicit Camera(const res::CameraSet::Camera &);

		/*----------+
		| accessors |
		+----------*/

		/**
		 * Focuses the camera on the specified position.
		 */
		void SetFocus(const math::Vec3 &);

		/**
		 * Returns the opacity multiplied by an interpolation function.
		 */
		float GetOpacity() const;

		/*-------+
		| update |
		+-------*/

		void Update(float deltaTime);

		/*--------------------+
		| Node implementation |
		+--------------------*/

		NodeType GetType() const override;

		/*--------------------+
		| frame serialization |
		+--------------------*/

		protected:
		Frame GetFrame() const override;
		void SetFrame(const Frame &) override;
	};

	/*-------------------------+
	| frustum/matrix accessors |
	+-------------------------*/

	math::ViewFrustum<> GetViewFrustum(const Camera &);
	math::Frustum<> GetFrustum(const Camera &);
	math::Mat4 GetMatrix(const Camera &);
	math::Mat4 GetProjMatrix(const Camera &);
	math::Mat4 GetInvProjMatrix(const Camera &);
	math::Mat34 GetViewMatrix(const Camera &);
	math::Mat34 GetInvViewMatrix(const Camera &);
}}

#endif
