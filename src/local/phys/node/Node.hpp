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
