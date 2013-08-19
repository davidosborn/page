#ifndef    page_local_phys_mixin_Collidable_hpp
#   define page_local_phys_mixin_Collidable_hpp

#	include "Trackable.hpp"

namespace page
{
	namespace phys
	{
		struct Collidable : Trackable
		{
			enum Type
			{
				// NOTE: track walls count as passive colliders
				active,  // collidable against active and passive colliders
				passive, // collidable against active colliders
				inactive // not collidable
			};

			// construct
			explicit Collidable(Type = inactive, float radius = 1);

			// state
			Type GetType() const;
			float GetRadius() const;

			// modifiers
			void SetType(Type);
			void SetRadius(float);

			private:
			Type type;
			float radius;
		};
	}
}

#endif
