#ifndef    page_local_phys_Controller_hpp
#   define page_local_phys_Controller_hpp

#	include <vector>

#	include "../util/class/Cloneable.hpp"
#	include "Frame.hpp"

namespace page
{
	namespace phys
	{
		class Controllable;

		struct Controller : virtual util::Cloneable<Controller>
		{
			enum Layer
			{
				preCollisionLayer,
				postCollisionLayer,
				constraintLayer
			};

			// construct/destroy
			explicit Controller(Layer);
			virtual ~Controller();

			// attributes
			bool IsAlive() const;
			float GetOpacity() const;
			Layer GetLayer() const;

			// update/generate frame
			void Update(float deltaTime);
			void Update(float deltaTime, float deltaUpdateTime);
			Frame GetFrame(const Frame &base, const Frame &accum) const;

			// modifiers
			void Kill();
			void SkipFade();

			// dependencies
			typedef std::vector<const Controllable *> Dependencies;
			virtual Dependencies GetDependencies() const;

			private:
			// update/generate frame
			virtual void DoUpdate(float deltaTime);
			virtual Frame DoGetFrame(const Frame &base, const Frame &accum) const = 0;

			bool alive;
			float opacity;
			Layer layer;
		};

		// layer comparison
		bool operator <(const Controller &, const Controller &);
		bool operator <(const Controller &, Controller::Layer);
	}
}

#endif
