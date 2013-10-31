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
		/*-------------+
		| constructors |
		+-------------*/

		public:
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
		 * @fn
		 * Returns a list of nodes that this controller depends on.
		 */
		typedef std::vector<const Node *> Dependencies;
		Dependencies GetDependencies() const;

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

		/*-------+
		| update |
		+-------*/

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
		virtual Dependencies DoGetDependencies() const;
		virtual void DoUpdate(float deltaTime);
		virtual Frame DoGetFrame(const Frame &base, const Frame &accum) const = 0;

		/*-------------+
		| data members |
		+-------------*/

		bool alive;
		float opacity;
		AnimationLayer layer;
	};

	/*-----------+
	| comparison |
	+-----------*/

	bool operator <(const Controller &, const Controller &);
	bool operator <(const Controller &, AnimationLayer);
}}

#endif
