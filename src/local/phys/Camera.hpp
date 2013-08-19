#ifndef    page_local_phys_Camera_hpp
#   define page_local_phys_Camera_hpp

#	include "../math/float.hpp" // Inf
#	include "../math/Frustum.hpp"
#	include "../math/Matrix.hpp"
#	include "../math/ViewFrustum.hpp"
#	include "../res/type/CameraSet.hpp" // CameraSet::Camera
#	include "attrib/Aspect.hpp"
#	include "attrib/Depth.hpp"
#	include "attrib/Exposure.hpp"
#	include "attrib/Fov.hpp"
#	include "attrib/Opacity.hpp"
#	include "attrib/PositionOrientation.hpp"
#	include "mixin/Controllable.hpp"

namespace page
{
	namespace phys
	{
		struct Camera : Controllable,
			attrib::Aspect,
			attrib::Depth,
			attrib::Exposure,
			attrib::Fov,
			attrib::Opacity,
			attrib::PositionOrientation
		{
			// construct
			explicit Camera(
				const math::Vec3 &position = 0,
				const math::Quat<> &orientation = math::Quat<>(),
				float depth = math::Inf());
			Camera(
				const math::Vec3 &position,
				const math::Vec3 &focus);
			explicit Camera(const res::CameraSet::Camera &);

			// focus
			void SetFocus(const math::Vec3 &);

			// opacity
			float GetOpacity() const;

			// update
			void Update(float deltaTime);

			protected:
			// frame serialization
			Frame GetFrame() const;
			void Update(const Frame &);
		};

		// frustum/matrix
		math::ViewFrustum<> GetViewFrustum(const Camera &);
		math::Frustum<> GetFrustum(const Camera &);
		math::Mat4 GetMatrix(const Camera &);
		math::Mat4 GetProjMatrix(const Camera &);
		math::Mat4 GetInvProjMatrix(const Camera &);
		math::Matrix<3, 4> GetViewMatrix(const Camera &);
		math::Matrix<3, 4> GetInvViewMatrix(const Camera &);
	}
}

#endif
