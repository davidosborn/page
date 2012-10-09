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

#include <iostream>
#include "../aud/Driver.hpp" // Driver::Play
#include "../cfg.hpp" // logVerbose
#include "../log/Indenter.hpp"
#include "../log/manip.hpp" // Warn
#include "../res/type/Scene.hpp" // Scene::music
#include "Character.hpp"
#include "Object.hpp"
#include "Player.hpp"
#include "Scene.hpp"

namespace page
{
	namespace game
	{
		// constructor/destructor
		Scene::Scene(aud::Driver &audioDriver, const res::Scene &scene) :
			phys::Scene(scene), audioDriver(audioDriver)
		{
			Init(scene);
		}
		Scene::~Scene() {}

		// update
		void Scene::Update(const inp::Driver &inputDriver, float deltaTime)
		{
			if (player) player->Update(inputDriver);
			phys::Scene::Update(deltaTime);
			// TEST: rotate sun
/*			math::Euler<> sunOrientation(math::NormVector<3>(), GetSunDirection());
			sunOrientation.yaw += deltaTime / 2;
			SetSunDirection(math::Quat<>(sunOrientation) * math::NormVector<3>());*/
		}

		// modifiers
		void Scene::Clear()
		{
			ClearThis();
			phys::Scene::Clear();
		}
		void Scene::Reset(const res::Scene &scene)
		{
			ClearThis();
			phys::Scene::Reset(scene);
			Init(scene);
		}

		// insertion
		void Scene::Insert(const std::shared_ptr<Character> &character)
		{
			// FIXME: store character in scene
			Insert(std::static_pointer_cast<Entity>(character));
		}
		void Scene::Insert(const std::shared_ptr<Entity> &entity)
		{
			// FIXME: store entity in scene
			Insert(std::static_pointer_cast<phys::Body>(entity));
		}
		void Scene::Insert(const std::shared_ptr<Object> &object)
		{
			// FIXME: store object in scene
			Insert(std::static_pointer_cast<Entity>(object));
		}

		// deletion
		void Scene::Remove(const std::shared_ptr<Character> &character)
		{
			// FIXME: remove character from scene
			Remove(std::static_pointer_cast<Entity>(character));
		}
		void Scene::Remove(const std::shared_ptr<Entity> &entity)
		{
			// FIXME: remove entity from scene
			Remove(std::static_pointer_cast<phys::Body>(entity));
		}
		void Scene::Remove(const std::shared_ptr<Object> &object)
		{
			// FIXME: remove object from scene
			Remove(std::static_pointer_cast<Entity>(object));
		}

		// initialization
		void Scene::Init(const res::Scene &scene)
		{
			if (scene.music)
			{
				log::Indenter indenter(false);
				if (*cfg::logVerbose)
				{
					std::cout << "playing music: " << scene.music.GetSource() << std::endl;
					indenter.Reset(true);
				}
				try
				{
					music = audioDriver.Play(scene.music, true, true, 5);
				}
				catch (...)
				{
					std::clog << log::Warn << "failed to play music" << std::endl;
				}
			}
			// create player
			player.reset(new Player(std::shared_ptr<Character>(
				new Character(*cache::Resource<res::Character>("character/male-1/male.char")))));
			Insert(std::static_pointer_cast<phys::Body>(player->GetCharacter()));
			player->GetCharacter()->SetTrack(GetTrack());
			SetFocus(player->GetCharacter());
		}

		// modifiers
		void Scene::ClearThis()
		{
			if (music)
			{
				music.Stop();
				music.Reset();
			}
		}
	}
}
