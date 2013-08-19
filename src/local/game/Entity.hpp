#ifndef    page_local_game_Entity_hpp
#   define page_local_game_Entity_hpp

#	include <string>

#	include "../phys/Body.hpp"

namespace page { namespace game
{
	struct Entity : phys::Body
	{
		// construct
		Entity(const std::string &name, const cache::Proxy<res::Model> &);

		// attributes
		const std::string &GetName() const;

		private:
		std::string name;
	};
}}

#endif
