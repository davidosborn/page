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
