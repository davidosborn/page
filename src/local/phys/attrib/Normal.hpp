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

#ifndef    page_local_phys_attrib_Normal_hpp
#   define page_local_phys_attrib_Normal_hpp

#	include "../../math/Matrix.hpp"
#	include "../../math/Quat.hpp"
#	include "../../math/Vector.hpp"
#	include "../Frame.hpp"
#	include "../mixin/Transformable.hpp"

namespace page { namespace phys { namespace attrib
{
	class Normal :
		public virtual Transformable
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit Normal(const math::Vec3 & = math::NormVector<3>());
		explicit Normal(const math::Quat<> &);
		explicit Normal(const math::Mat3 &);

		/*----------+
		| accessors |
		+----------*/

		const math::Vec3 &GetNormal() const;
		void SetNormal(const math::Vec3 &);

		// orientation view
		math::Quat<> GetOrientation() const;
		void SetOrientation(const math::Quat<> &);

		// matrix view
		math::Mat3 GetMatrix() const;
		math::Mat3 GetInvMatrix() const;
		void SetMatrix(const math::Mat3 &);

		// transformation observers
		const math::Vec3 &GetLastNormal() const;
		math::Quat<> GetLastOrientation() const;
		const math::Quat<> &GetTorque() const;
		const math::Quat<> &GetSpin() const;

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
		math::Quat<> torque, spin;
	};
}}}

#endif
