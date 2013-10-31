#ifndef    page_local_phys_mixin_Controllable_hpp
#   define page_local_phys_mixin_Controllable_hpp

#	include <memory> // {shared,weak}_ptr
#	include <vector>

#	include "../../util/class/special_member_functions.hpp" // Polymorphic
#	include "../AnimationLayer.hpp"
#	include "../Frame.hpp"

namespace page { namespace phys
{
	class Controller;

	/**
	 * A mixin which makes the derived node controllable.  The attached
	 * controllers will be able to read/write the node's attributes through
	 * @c Frame when they are executed.
	 */
	class Controllable : public util::Polymorphic<Controllable>
	{
		/*---------------------+
		| controller execution |
		+---------------------*/

		public:
		/**
		 * Updates the controllers that are attached to the specified layer, and
		 * applies them to this node.
		 */
		void ApplyControllers(AnimationLayer, float deltaTime);

		/**
		 * Updates all of the attached controllers, and applies them to this
		 * node.
		 */
		void ApplyControllers(float deltaTime);

		/*----------------------+
		| controller attachment |
		+----------------------*/

		/**
		 * Attaches a copy of the specified controller to this node.
		 */
		std::weak_ptr<Controller> AttachController(const Controller &);

		/**
		 * Attaches the specified controller to this node.
		 */
		std::weak_ptr<Controller> AttachController(const std::shared_ptr<Controller> &);

		/**
		 * Detaches the specified controller from this node.
		 */
		void DetachController(const std::weak_ptr<Controller> &);

		/**
		 * Detaches the specified controller from this node.
		 */
		void DetachController(const std::shared_ptr<Controller> &);

		/**
		 * Detaches all controllers from this node.
		 */
		void DetachAllControllers();

		/*--------------------+
		| frame serialization |
		+--------------------*/

		protected:
		virtual Frame GetFrame() const = 0;
		virtual void SetFrame(const Frame &) = 0;

		/*-------------+
		| data members |
		+-------------*/

		private:
		typedef std::vector<std::shared_ptr<Controller>> Controllers;
		std::vector<Controllers> layers;
	};
}}

#endif
