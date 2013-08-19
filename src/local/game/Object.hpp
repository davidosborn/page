#ifndef    page_local_game_Object_hpp
#   define page_local_game_Object_hpp

#	include "Entity.hpp"

namespace page { namespace res { class Object; }}

namespace page { namespace game
{
	struct Object : Entity
	{
		// construct
		explicit Object(const res::Object &);
	};
}}

#endif
