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

#ifndef    page_local_phys_node_Emitter_hpp
#   define page_local_phys_node_Emitter_hpp

#	include <string>

#	include "../attrib/AmbientRange.hpp"
#	include "../attrib/Cutoff.hpp"
#	include "../attrib/DiffuseRange.hpp"
#	include "../attrib/EmissiveRange.hpp"
#	include "../attrib/LifetimeRange.hpp"
#	include "../attrib/Material.hpp"
#	include "../attrib/OpacityRange.hpp"
#	include "../attrib/PositionOrientation.hpp"
#	include "../attrib/SizeRange.hpp"
#	include "../attrib/SpecularRange.hpp"
#	include "../attrib/SpeedRange.hpp"
#	include "../mixin/Controllable.hpp"
#	include "Node.hpp"

namespace page { namespace phys
{
	/**
	 * A particle emitter.
	 */
	class Emitter :
		public Node,
		public Controllable,
		public attrib::AmbientRange,
		public attrib::Cutoff,
		public attrib::DiffuseRange,
		public attrib::EmissiveRange,
		public attrib::LifetimeRange,
		public attrib::Material,
		public attrib::OpacityRange,
		public attrib::PositionOrientation,
		public attrib::SpecularRange,
		public attrib::SizeRange,
		public attrib::SpeedRange
	{
		IMPLEMENT_CLONEABLE(Emitter, Node)

		public:
		// update
		void Update();

		/*--------------------+
		| frame serialization |
		+--------------------*/

		protected:
		Frame GetFrame() const override;
		void SetFrame(const Frame &) override;
	};
}}

#endif
