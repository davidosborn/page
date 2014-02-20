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

#ifndef    page_local_cache_proxy_AabbProxy_hpp
#   define page_local_cache_proxy_AabbProxy_hpp

#	include "../../math/fwd.hpp" // Aabb
#	include "../../util/Identifiable.hpp" // Identifiable::Id
#	include "BasicProxy.hpp"
#	include "Proxy.hpp"

namespace page { namespace phys
{
	class Bounds;
	class Form;
	namespace attrib { class Pose; }
}}

namespace page { namespace cache
{
	/**
	 * A proxy representing an axis-aligned bounding-box in the cache.
	 */
	class AabbProxy : public BasicProxy<math::Aabb<3>>
	{
		IMPLEMENT_CLONEABLE(AabbProxy, BasicProxy<math::Aabb<3>>)

		/*-------+
		| traits |
		+-------*/

		public:
		using typename BasicProxy<math::Aabb<3>>::pointer;

		/*-------------+
		| constructors |
		+-------------*/

		/**
		 * Creates an AABB proxy for an optionally-posed form.
		 */
		explicit AabbProxy(const phys::Form &, bool pose = true);

		/**
		 * Creates an AABB proxy for an optionally-posed bounding-skeleton.
		 */
		AabbProxy(const Proxy<phys::Bounds> &, const phys::attrib::Pose &);

		private:
		AabbProxy(const Proxy<phys::Bounds> &, util::Identifiable::Id);

		/*--------------------------+
		| BasicProxy implementation |
		+--------------------------*/

		pointer DoLock() const override;

		/*-------------+
		| data members |
		+-------------*/

		Proxy<phys::Bounds> bounds;
		util::Identifiable::Id poseId;
	};
}}

#endif
