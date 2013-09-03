#include <iostream> // cout

#include <boost/optional.hpp>

#include "../aud/Driver.hpp" // Driver::Play
#include "../cfg/vars.hpp"
#include "../err/report.hpp" // ReportWarning
#include "../log/Indenter.hpp"
#include "../res/type/Scene.hpp" // Scene::music
#include "Character.hpp"
#include "Object.hpp"
#include "Player.hpp"
#include "Scene.hpp"

namespace page { namespace game
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
		/*math::Euler<> sunOrientation(math::NormVector<3>(), GetSunDirection());
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
			boost::optional<log::Indenter> indenter;
			if (*CVAR(logVerbose))
			{
				std::cout << "playing music: " << scene.music.GetSource() << std::endl;
				indenter = boost::in_place();
			}
			try
			{
				music = audioDriver.Play(scene.music, true, true, 5);
			}
			catch (const std::exception &e)
			{
				err::ReportWarning(e);
				if (*CVAR(logVerbose))
					std::cout << "failed to play music" << std::endl;
			}
		}
		// create player
		player.reset(new Player(std::shared_ptr<Character>(
			new Character(*cache::ResourceProxy<res::Character>("character/male-1/male.char")))));
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
}}
