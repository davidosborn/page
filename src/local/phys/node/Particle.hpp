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

#ifndef    page_local_phys_node_Particle_hpp
#   define page_local_phys_node_Particle_hpp

#	include "../../cache/proxy/fwd.hpp" // Proxy
#	include "../attrib/Ambient.hpp"
#	include "../attrib/Diffuse.hpp"
#	include "../attrib/Emissive.hpp"
#	include "../attrib/Material.hpp"
#	include "../attrib/Opacity.hpp"
#	include "../attrib/Position.hpp"
#	include "../attrib/Size.hpp"
#	include "../attrib/Specular.hpp"
#	include "Node.hpp"

namespace page { namespace res { class Material; }}

namespace page { namespace phys
{
	class Particle :
		public Node,
		public attrib::Ambient,
		public attrib::Diffuse,
		public attrib::Emissive,
		public attrib::Material,
		public attrib::Position,
		public attrib::Opacity,
		public attrib::Size,
		public attrib::Specular
	{
		IMPLEMENT_CLONEABLE(Particle, Node)

		// construct
		explicit Particle(const cache::Proxy<res::Material> &material);

		/*--------------------+
		| frame serialization |
		+--------------------*/

		protected:
		Frame GetFrame() const override;
		void SetFrame(const Frame &) override;

		private:
		float lifetime, drag, mass;
	};
}}

#endif
