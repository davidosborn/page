/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
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

#ifndef    page_local_phys_attrib_Scale_hpp
#   define page_local_phys_attrib_Scale_hpp

#	include "../../math/Matrix.hpp"
#	include "../../math/Vector.hpp"
#	include "../Frame.hpp"
#	include "../mixin/Transformable.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			struct Scale : virtual Transformable
			{
				// construct
				explicit Scale(const math::Vector<3> & = 1);
				explicit Scale(const math::Matrix<3> &);

				// access
				const math::Vector<3> &GetScale() const;
				void SetScale(const math::Vector<3> &);

				// matrix access
				math::Matrix<3> GetMatrix() const;
				math::Matrix<3> GetInvMatrix() const;
				void SetMatrix(const math::Matrix<3> &);

				// transform state
				const math::Vector<3> &GetLastScale() const;
				const math::Vector<3> &GetScaleForce() const;
				const math::Vector<3> &GetDeltaScale() const;

				protected:
				// frame serialization
				Frame GetFrame() const;
				void Update(const Frame &);

				// transform modifiers
				void BakeTransform();
				void UpdateForce();
				void UpdateDelta();

				private:
				math::Vector<3> scale, lastScale, force, delta;
			};
		}
	}
}

#endif
