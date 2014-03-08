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

#ifndef    page_local_phys_node_Light_hpp
#   define page_local_phys_node_Light_hpp

#	include "../attrib/Ambient.hpp"
#	include "../attrib/Attenuation.hpp"
#	include "../attrib/Cutoff.hpp"
#	include "../attrib/Diffuse.hpp"
#	include "../attrib/Falloff.hpp"
#	include "../attrib/PositionOrientation.hpp"
#	include "../attrib/Range.hpp"
#	include "../attrib/Specular.hpp"
#	include "../mixin/Controllable.hpp"
#	include "Node.hpp"

namespace page { namespace phys
{
	class Light :
		public Node,
		public Controllable,
		public attrib::Ambient,
		public attrib::Attenuation,
		public attrib::Cutoff,
		public attrib::Diffuse,
		public attrib::Falloff,
		public attrib::PositionOrientation,
		public attrib::Range,
		public attrib::Specular
	{
		IMPLEMENT_CLONEABLE(Light, Node)

		/*----------+
		| observers |
		+----------*/

		public:
		/**
		 * Returns the minimum attenuation distance.
		 */
		float GetMinRange() const;

		/**
		 * Returns the maximum attenuation distance.
		 */
		float GetMaxRange() const;

		/*--------------------+
		| frame serialization |
		+--------------------*/

		protected:
		Frame GetFrame() const override;
		void SetFrame(const Frame &) override;
	};
}}

#endif
