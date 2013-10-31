#ifndef    page_local_game_Entity_hpp
#   define page_local_game_Entity_hpp

#	include <memory> // shared_ptr
#	include <string>

#	include "../util/class/special_member_functions.hpp" // Polymorphic

namespace page { namespace phys { class Body; }}

namespace page { namespace game
{
	class Entity : public util::Polymorphic<Entity>
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		Entity(const std::string &name, const cache::Proxy<res::Model> &);
		Entity(const Entity &other);

		/*----------+
		| observers |
		+----------*/

		const std::string &GetName() const;

		phys::Body &GetBody();
		const phys::Body &GetBody() const;
		const std::shared_ptr<phys::Body> &GetBodyPtr() const;

		/*-------------+
		| data members |
		+-------------*/

		private:
		std::string name;
		std::shared_ptr<phys::Body> body;
	};
}}

#endif
