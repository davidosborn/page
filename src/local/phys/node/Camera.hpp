/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

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
