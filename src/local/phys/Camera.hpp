/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

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
				const math::Vector<3> &position = 0,
				const math::Quat<> &orientation = math::Quat<>(),
				float depth = math::Inf());
			Camera(
				const math::Vector<3> &position,
				const math::Vector<3> &focus);
			explicit Camera(const res::CameraSet::Camera &);

			// focus
			void SetFocus(const math::Vector<3> &);

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
		math::Matrix<4> GetMatrix(const Camera &);
		math::Matrix<4> GetProjMatrix(const Camera &);
		math::Matrix<4> GetInvProjMatrix(const Camera &);
		math::Matrix<3, 4> GetViewMatrix(const Camera &);
		math::Matrix<3, 4> GetInvViewMatrix(const Camera &);
	}
}

#endif
