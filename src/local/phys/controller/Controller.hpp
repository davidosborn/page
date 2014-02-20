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

#ifndef    page_local_phys_controller_Controller_hpp
#   define page_local_phys_controller_Controller_hpp

#	include <vector>

#	include "../../util/class/Cloneable.hpp"
#	include "../AnimationLayer.hpp"
#	include "../Frame.hpp"

namespace page { namespace phys
{
	class Controllable;
	class Node;

	class Controller : public util::Cloneable<Controller>
	{
		public:
		typedef std::vector<const Node *> Dependencies;

		/*-------------+
		| constructors |
		+-------------*/

		explicit Controller(AnimationLayer);

		/*----------+
		| observers |
		+----------*/

		/**
		 * Returns @c true if the controller has not been killed.
		 */
		bool IsAlive() const;

		/**
		 * Returns the controller's opacity, which may be less than 1 when the
		 * controller is fading in/out.
		 */
		float GetOpacity() const;

		/**
		 * Returns the layer that the controller should be attached to.
		 */
		AnimationLayer GetLayer() const;

		/**
		 * Returns the nodes that this controller depends on.
		 */
		const Dependencies &GetDependencies() const;

		/*----------+
		| modifiers |
		+----------*/

		/**
		 * Kills the controller.
		 */
		void Kill();

		/**
		 * Causes the controller's opacity to reach its goal immediately.
		 */
		void SkipFade();

		protected:
		/**
		 * Allows the derived controller to specify the nodes that it depends
		 * on.
		 */
		void SetDependencies(const Dependencies &);

		/*-------+
		| update |
		+-------*/

		public:
		/**
		 * Updates the controller.
		 */
		void Update(float deltaTime);

		/**
		 * Updates the controller.
		 */
		void Update(float deltaTime, float deltaUpdateTime);

		/**
		 * Generates a frame to be applied to the controlled node.
		 */
		Frame GetFrame(const Frame &base, const Frame &accum) const;

		/*---------------+
		| implementation |
		+---------------*/

		private:
		virtual void DoUpdate(float deltaTime);
		virtual Frame DoGetFrame(const Frame &base, const Frame &accum) const = 0;

		/*-------------+
		| data members |
		+-------------*/

		private:
		bool alive;
		float opacity;
		AnimationLayer layer;

		/**
		 * The nodes that this controller depends on.
		 */
		Dependencies dependencies;
	};

	/*-----------+
	| comparison |
	+-----------*/

	bool operator <(const Controller &, const Controller &);
	bool operator <(const Controller &, AnimationLayer);
}}

#endif
