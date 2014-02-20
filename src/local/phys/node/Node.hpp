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

#ifndef    page_local_phys_node_Node_hpp
#   define page_local_phys_node_Node_hpp

#	include "../../util/class/Cloneable.hpp"

namespace page { namespace phys
{
	/**
	 * Standard node types.
	 */
	enum class NodeType
	{
		/**
		 * The type of the node is not known, or it is not a standard type.
		 */
		unknown,

		/**
		 * A body node.
		 * @sa Body
		 */
		body,

		/**
		 * A camera node.
		 * @sa Camera
		 */
		camera,

		/**
		 * An emitter node.
		 * @sa emitter
		 */
		emitter,

		/**
		 * A form node.
		 * @sa Form
		 */
		form,

		/**
		 * A light node.
		 * @sa Light
		 */
		light,

		/**
		 * A particle node.
		 * @sa Particle
		 */
		particle,

		/**
		 * A sound node.
		 * @sa Sound
		 */
		sound
	};

	/**
	 * The base class for a node.
	 */
	class Node : public virtual util::Cloneable<Node>
	{
		public:
		/**
		 * Returns the most-derived type of the node.
		 */
		virtual NodeType GetType() const;
	};
}}

#endif
