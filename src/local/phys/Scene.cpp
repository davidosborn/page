/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include <algorithm> // remove
#include <cassert>

#include <boost/iterator/indirect_iterator.hpp>
#include <boost/range/adaptor/indirected.hpp>

#include "../math/Euler.hpp"
#include "../math/float.hpp" // DegToRad
#include "../math/Plane.hpp"
#include "../math/Quat.hpp"
#include "../math/Vector.hpp"
#include "../math/ViewFrustum.hpp"
#include "../res/type/Scene.hpp"
#include "../util/functional/pointer.hpp" // address_of
#include "controller/CameraFocusController.hpp"
#include "controller/ConstrainPositionToPlaneController.hpp"
#include "controller/FollowController.hpp"
#include "controller/HeroCamController.hpp"
#include "mixin/Controllable.hpp" // UpdateControllables
#include "mixin/Trackable.hpp" // Trackable::{GetTrackFaceIndex,HasTrackFace}
#include "mixin/update/Collidable.hpp" // UpdateCollidables
#include "mixin/update/Trackable.hpp" // UpdateTrackables
#include "node/Body.hpp"
#include "node/Camera.hpp"
#include "node/Emitter.hpp"
#include "node/Light.hpp"
#include "node/Particle.hpp"
#include "node/Sound.hpp"
#include "Scene.hpp"

namespace page { namespace phys
{
	const float cameraFadeInDuration = 1;

	namespace
	{
		// initialize controllables created during/after update
		void UpdateLateControllable(Controllable &controllable)
		{
			controllable.ApplyControllers(0);
		}
	}

	/*-------------+
	| constructors |
	+-------------*/

	Scene::Scene(const res::Scene &scene) :
		cameraMode(CameraMode::scene), cameraType(CameraType::none)
	{
		Reset(scene);
	}

	/*----------+
	| modifiers |
	+----------*/

	void Scene::Insert(const std::shared_ptr<Node> &node)
	{
		assert(node);
		// FIXME: implement
		if (auto trackable = dynamic_cast<Trackable *>(node.get()))
			trackables.push_back(trackable);
		if (auto transformable = dynamic_cast<Transformable *>(node.get()))
			transformables.push_back(transformable);
	}

	void Scene::Remove(const std::shared_ptr<Node> &node)
	{
		assert(node);
		// FIXME: implement
		/*transformables.erase(
			std::remove(transformables.begin(), transformables.end(), transformable),
			transformables.end());*/
	}

	void Scene::Clear()
	{
		// attributes
		focus.target.reset();
		focus.position = 0;
		cameraSet.reset();
		track.reset();

		// objects
		transformables.clear();
		trackables.clear();
		sounds.clear();
		particles.clear();
		lights.clear();
		forms.clear();
		emitters.clear();
		controllables.clear();
		collidables.clear();
		cameras.clear();
		bodies.clear();
	}

	void Scene::Reset(const res::Scene &scene)
	{
		Clear();

		// forms
		for (const auto &form : scene.forms)
			Insert(std::shared_ptr<Form>(new Form(form)));

		// track
		if (scene.track)
			track = scene.track.lock();

		// camera set
		if (scene.cameraSet)
		{
			cameraSet = scene.cameraSet.lock();
			cameraSetCamera = cameraSet->cameras.end();
		}

		// atmospherics
		sunDirection = Norm(-math::Vec3(.5, 1, .5));
	}

	/*------+
	| views |
	+------*/

	util::reference_vector<Body> Scene::GetBodies() const
	{
		util::reference_vector<Body> view;
		view.reserve(bodies.size());
		for (auto &body : boost::adaptors::indirect(bodies))
			view.push_back(body);
		return view;
	}

	util::reference_vector<Body> Scene::GetVisibleBodies(const math::ViewFrustum<> &frustum) const
	{
		util::reference_vector<Body> view;
		view.reserve(bodies.size());
		// FIXME: should only copy visible bodies
		for (auto &body : boost::adaptors::indirect(bodies))
			view.push_back(body);
		return view;
	}

	util::reference_vector<Camera> Scene::GetCameras() const
	{
		util::reference_vector<Camera> view;
		view.reserve(cameras.size());
		for (auto &camera : boost::adaptors::indirect(cameras))
			view.push_back(camera);
		return view;
	}

	util::reference_vector<Collidable> Scene::GetCollidableNodes() const
	{
		util::reference_vector<Collidable> view;
		view.reserve(collidables.size());
		for (auto &collidable : boost::adaptors::indirect(collidables))
			view.push_back(collidable);
		return view;
	}

	util::reference_vector<Collidable> Scene::GetVisibleCollidableNodes(const math::ViewFrustum<> &frustum) const
	{
		util::reference_vector<Collidable> view;
		view.reserve(collidables.size());
		// FIXME: should only copy visible collidables
		for (auto &collidable : boost::adaptors::indirect(collidables))
			view.push_back(collidable);
		return view;
	}

	util::reference_vector<Form> Scene::GetForms() const
	{
		util::reference_vector<Form> view;
		view.reserve(forms.size());
		for (auto &form : boost::adaptors::indirect(forms))
			view.push_back(form);
		return view;
	}

	util::reference_vector<Form> Scene::GetVisibleForms(const math::ViewFrustum<> &frustum) const
	{
		util::reference_vector<Form> view;
		view.reserve(forms.size());
		// FIXME: should only copy visible forms
		for (auto &form : boost::adaptors::indirect(forms))
			view.push_back(form);
		return view;
	}

	Scene::FormsByMaterialView Scene::GetVisibleFormsByMaterial(const math::ViewFrustum<> &frustum) const
	{
		FormsByMaterialView view;
		// FIXME: implement
		return view;
	}

	util::reference_vector<Light> Scene::GetLights() const
	{
		util::reference_vector<Light> view;
		view.reserve(lights.size());
		for (auto &light : boost::adaptors::indirect(lights))
			view.push_back(light);
		return view;
	}

	util::reference_vector<Light> Scene::GetInfluentialLights(const math::ViewFrustum<> &frustum) const
	{
		util::reference_vector<Light> view;
		view.reserve(lights.size());
		// FIXME: should only copy influential lights
		for (auto &light : boost::adaptors::indirect(lights))
			view.push_back(light);
		return view;
	}

	util::reference_vector<Particle> Scene::GetParticles() const
	{
		util::reference_vector<Particle> view;
		view.reserve(particles.size());
		for (auto &particle : boost::adaptors::indirect(particles))
			view.push_back(particle);
		return view;
	}

	util::reference_vector<Particle> Scene::GetVisibleParticles(const math::ViewFrustum<> &frustum) const
	{
		util::reference_vector<Particle> view;
		view.reserve(particles.size());
		// FIXME: should only copy visible particles
		for (auto &particle : boost::adaptors::indirect(particles))
			view.push_back(particle);
		return view;
	}

	util::reference_vector<Sound> Scene::GetSounds() const
	{
		util::reference_vector<Sound> view;
		view.reserve(sounds.size());
		for (auto &sound : boost::adaptors::indirect(sounds))
			view.push_back(sound);
		return view;
	}

	util::reference_vector<Sound> Scene::GetClosestSounds(const math::Vec3 &co, unsigned n) const
	{
		util::reference_vector<Sound> view;
		view.reserve(n);
		// FIXME: find n closest sounds
		return view;
	}

	util::reference_vector<Trackable> Scene::GetTrackableNodes() const
	{
		util::reference_vector<Trackable> view;
		view.reserve(trackables.size());
		for (auto &trackable : boost::adaptors::indirect(trackables))
			view.push_back(trackable);
		return view;
	}

	/*-------------+
	| atmospherics |
	+-------------*/

	const math::Vec3 &Scene::GetSunDirection() const
	{
		return sunDirection;
	}

	void Scene::SetSunDirection(const math::Vec3 &dir)
	{
		assert(All(Near(dir, Norm(dir))));
		sunDirection = dir;
	}

	/*-------+
	| camera |
	+-------*/

	Scene::CameraMode Scene::GetCameraMode() const
	{
		return cameraMode;
	}

	void Scene::UseDetailCamera(const inp::Driver &driver)
	{
		assert(focus.target);
		std::shared_ptr<Camera> camera(new Camera);
		camera->SetOpacity(0);
		camera->AttachController(HeroCamController(driver, *focus.target));
		Insert(camera);
		cameraType = CameraType::detail;
		cameraMode = CameraMode::detail;
	}

	void Scene::UseSceneCamera()
	{
		cameraMode = sceneCameraMode;
	}

	/*------+
	| focus |
	+------*/

	const math::Vec3 &Scene::GetFocus() const
	{
		return focus.position;
	}

	void Scene::SetFocus()
	{
		focus.target.reset();
	}

	void Scene::SetFocus(const math::Vec3 &position)
	{
		focus.target.reset();
		focus.position = position;
	}

	void Scene::SetFocus(const std::shared_ptr<const Body> &target)
	{
		focus.target = target;
		focus.position = target->GetPosition();
	}

	/*------+
	| track |
	+------*/

	bool Scene::HasTrack() const
	{
		return track != nullptr;
	}
	const res::Track &Scene::GetTrack() const
	{
		assert(track);
		return *track;
	}

	/*-------+
	| update |
	+-------*/

	void Scene::Update(float deltaTime)
	{
		UpdateControllables(AnimationLayer::preCollision, deltaTime);
		UpdateForces();
		UpdateCollidables(
			boost::make_indirect_iterator(collidables.begin()),
			boost::make_indirect_iterator(collidables.end()));
		UpdateTrackables(
			boost::make_indirect_iterator(trackables.begin()),
			boost::make_indirect_iterator(trackables.end()));
		UpdateDeltas();
		UpdateControllables(AnimationLayer::postCollision, deltaTime);
		// FIXME: update constraints
		UpdateControllables(AnimationLayer::postConstraint, deltaTime);
		UpdateCameraTracking();
		UpdateObjects(deltaTime);
		// update focus
		if (focus.target) focus.position = focus.target->GetPosition();
	}

	void Scene::UpdateControllables(AnimationLayer layer, float deltaTime)
	{
		for (Controllables::iterator iter(controllables.begin()); iter != controllables.end(); ++iter)
		{
			Controllable &controllable(**iter);
			controllable.Update(layer, deltaTime);
		}
		// HACK: scene is controllable, but isn't stored in the list
		Controllable::Update(layer, deltaTime);
	}

	void Scene::UpdateForces()
	{
		for (Transformables::iterator iter(transformables.begin()); iter != transformables.end(); ++iter)
		{
			Transformable &transformable(**iter);
			transformable.UpdateForce();
		}
	}

	void Scene::UpdateDeltas()
	{
		for (Transformables::iterator iter(transformables.begin()); iter != transformables.end(); ++iter)
		{
			Transformable &transformable(**iter);
			transformable.UpdateDelta();
			transformable.BakeTransform();
		}
	}

	void Scene::UpdateCameraTracking()
	{
		// FIXME: we still need to update the cameraFollowController
		if (cameraMode != sceneCameraMode) return;
		// update camera selection
		if (focus.target)
		{
			if (focus.target->HasTrackFace() && cameraSet &&
				focus.target->GetTrackFaceIndex() < cameraSet->trackFaces.size())
			{
				const res::CameraSet::TrackFace &trackFace(
					cameraSet->trackFaces[focus.target->GetTrackFaceIndex()]);
				if (trackFace.cameras.empty()) goto FollowCamera;
				if (cameraType != CameraType::cameraSet ||
					std::find(trackFace.cameras.begin(), trackFace.cameras.end(),
						cameraSetCamera) == trackFace.cameras.end())
				{
					// switch to new camera
					cameraSetCamera = *trackFace.cameras.begin();
					std::shared_ptr<Camera> camera(new Camera(*cameraSetCamera));
					camera->SetOpacity(0);
					switch (cameraSetCamera->tracking)
					{
						case res::CameraSet::Camera::positionTracking:
						camera->AttachController(FollowController(
							*focus.target, .5,
							cameraSetCamera->orientation,
							cameraSetCamera->trackingDistance));
						if (!cameraSetCamera->trackingDistance)
							camera->AttachController(
								ConstrainPositionToPlaneController(
									math::Plane<3>(
										camera->GetNormal(),
										camera->GetPosition())));
						break;
						case res::CameraSet::Camera::orientationTracking:
						camera->AttachController(CameraFocusController(*camera, *focus.target));
						break;
					}
					UpdateLateControllable(*camera);
					Insert(camera);
					cameraType = CameraType::cameraSet;
				}
			}
			else
			{
				FollowCamera:
				// use default follow camera
				if (cameraType != CameraType::follow)
				{
					std::shared_ptr<Camera> camera(new Camera);
					camera->SetOpacity(0);
					camera->AttachController(followCameraController =
						std::shared_ptr<FollowController>(
							new FollowController(*focus.target, .5,
								math::Quat<>(math::Euler<>(
									math::DegToRad(22.5f),
									math::DegToRad(-45.f))), 8)));
					UpdateLateControllable(*camera);
					Insert(camera);
					cameraType = CameraType::follow;
				}
				else followCameraController->Follow(*focus.target);
			}
		}
		else
		{
			// use default overview camera
			if (cameraType != CameraType::overview)
			{
				// FIXME: implement
				cameraType = CameraType::overview;
			}
		}
	}

	void Scene::UpdateObjects(float deltaTime)
	{
		// update cameras
		for (Cameras::reverse_iterator iter(cameras.rbegin()); iter != cameras.rend(); ++iter)
		{
			Camera &camera(**iter);
			camera.Update(deltaTime);
			if (camera.GetOpacity() == 1)
			{
				cameras.erase(cameras.begin(), iter.base() - 1);
				break;
			}
		}
		// update sounds
		for (Sounds::iterator iter(sounds.begin()); iter != sounds.end(); ++iter)
		{
			Sound &sound(**iter);
			sound.Update(deltaTime);
		}
	}
}}
