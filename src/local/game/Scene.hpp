/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

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

	namespace game
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
	}
}

#endif
