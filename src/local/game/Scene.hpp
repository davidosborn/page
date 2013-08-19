#ifndef    page_local_game_Scene_hpp
#   define page_local_game_Scene_hpp

#	include <memory> // unique_ptr

#	include "../aud/Driver.hpp" // SoundProxy
#	include "../cache/proxy/Resource.hpp"
#	include "../phys/Scene.hpp"

namespace page
{
	namespace aud { class Driver; }
	namespace inp { class Driver; }
}

namespace page { namespace game
{
	class Character;
	class Entity;
	class Object;
	class Player;

	struct Scene : phys::Scene
	{
		// constructor/destructor
		Scene(aud::Driver &, const res::Scene & =
			*cache::Resource<res::Scene>("scene/village/village.scene"));//"scene/default.scene"));
		/**
		 * @note We need to define the destructor ourselves, even though it
		 *       doesn't do anything, to allow @c std::unique_ptr to contain
		 *       an incomplete type.  This works because the type is no
		 *       longer incomplete at the point where the destructor is
		 *       defined.
		 */
		~Scene();

		// update
		void Update(const inp::Driver &, float deltaTime);

		// modifiers
		void Clear();
		void Reset(const res::Scene &);

		// insertion
		using phys::Scene::Insert;
		void Insert(const std::shared_ptr<Character> &);
		void Insert(const std::shared_ptr<Entity> &);
		void Insert(const std::shared_ptr<Object> &);

		// deletion
		using phys::Scene::Remove;
		void Remove(const std::shared_ptr<Character> &);
		void Remove(const std::shared_ptr<Entity> &);
		void Remove(const std::shared_ptr<Object> &);

		private:
		// initialization
		void Init(const res::Scene &);

		// modifiers
		void ClearThis(); // clears this class only, not the base class

		aud::Driver &audioDriver;
		aud::SoundProxy music;
		std::unique_ptr<Player> player;
	};
}}

#endif
