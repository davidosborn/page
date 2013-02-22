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

#include <algorithm> // remove, transform
#include <cassert>
#include <functional> // hash
#include <iterator> // back_inserter

#include "../cache/proxy/Proxy.hpp" // hash(Proxy)
#include "../math/Euler.hpp"
#include "../math/float.hpp" // DegToRad
#include "../math/Plane.hpp"
#include "../math/Quat.hpp"
#include "../math/Vector.hpp"
#include "../math/ViewFrustum.hpp"
#include "../res/type/Scene.hpp"
#include "../util/functional/pointer.hpp" // address_of
#include "../util/iterator/indirect_iterator.hpp"
#include "Body.hpp"
#include "Camera.hpp"
#include "controller/CameraFocusController.hpp"
#include "controller/ConstrainPositionToPlaneController.hpp"
#include "controller/FollowController.hpp"
#include "controller/HeroCamController.hpp"
#include "Emitter.hpp"
#include "Light.hpp"
#include "mixin/Controllable.hpp" // UpdateControllables
#include "mixin/Trackable.hpp" // Trackable::{GetTrackFaceIndex,HasTrackFace}
#include "mixin/update/Collidable.hpp" // UpdateCollidables
#include "mixin/update/Trackable.hpp" // UpdateTrackables
#include "Particle.hpp"
#include "Scene.hpp"
#include "Sound.hpp"

namespace page
{
	namespace phys
	{
		const float cameraFadeInDuration = 1;

		namespace
		{
			// initialize controllables created during/after update
			void UpdateLateControllable(Controllable &controllable)
			{
				controllable.Update(Controller::preCollisionLayer, 0);
				controllable.Update(Controller::postCollisionLayer, 0);
				controllable.Update(Controller::constraintLayer, 0);
			}
		}

		// construct
		Scene::Scene() :
			cameraMode(sceneCameraMode), cameraType(noCameraType) {}
		Scene::Scene(const res::Scene &scene) :
			cameraMode(sceneCameraMode), cameraType(noCameraType)
		{
			Init(scene);
		}

		////////////////////////////////////////////////////////////////////////
		// contained object views

		// body views
		Scene::View<Body>::Type Scene::GetBodies() const
		{
			View<Body>::Type view;
			view.reserve(bodies.size());
			std::transform(
				util::make_indirect_iterator(bodies.begin()),
				util::make_indirect_iterator(bodies.end()),
				std::back_inserter(view), util::address_of<Body>());
			return view;
		}
		Scene::View<Body>::Type Scene::GetVisibleBodies(const math::ViewFrustum<> &frustum) const
		{
			View<Body>::Type view;
			view.reserve(bodies.size());
			// FIXME: should only copy visible bodies
			std::transform(
				util::make_indirect_iterator(bodies.begin()),
				util::make_indirect_iterator(bodies.end()),
				std::back_inserter(view), util::address_of<Body>());
			return view;
		}

		// camera views
		Scene::View<Camera>::Type Scene::GetCameras() const
		{
			View<Camera>::Type view;
			view.reserve(cameras.size());
			std::transform(
				util::make_indirect_iterator(cameras.begin()),
				util::make_indirect_iterator(cameras.end()),
				std::back_inserter(view), util::address_of<Camera>());
			return view;
		}

		// collidable views
		Scene::View<Collidable>::Type Scene::GetCollidables() const
		{
			View<Collidable>::Type view;
			view.reserve(collidables.size());
			std::transform(
				util::make_indirect_iterator(collidables.begin()),
				util::make_indirect_iterator(collidables.end()),
				std::back_inserter(view), util::address_of<Collidable>());
			return view;
		}
		Scene::View<Collidable>::Type Scene::GetVisibleCollidables(const math::ViewFrustum<> &frustum) const
		{
			View<Collidable>::Type view;
			view.reserve(collidables.size());
			// FIXME: should only copy visible collidables
			std::transform(
				util::make_indirect_iterator(collidables.begin()),
				util::make_indirect_iterator(collidables.end()),
				std::back_inserter(view), util::address_of<Collidable>());
			return view;
		}

		// form views
		Scene::View<Form>::Type Scene::GetForms() const
		{
			View<Form>::Type view;
			view.reserve(forms.size());
			std::transform(
				util::make_indirect_iterator(forms.begin()),
				util::make_indirect_iterator(forms.end()),
				std::back_inserter(view), util::address_of<Form>());
			return view;
		}
		Scene::View<Form>::Type Scene::GetVisibleForms(const math::ViewFrustum<> &frustum) const
		{
			View<Form>::Type view;
			view.reserve(forms.size());
			// FIXME: should only copy visible forms
			std::transform(
				util::make_indirect_iterator(forms.begin()),
				util::make_indirect_iterator(forms.end()),
				std::back_inserter(view), util::address_of<Form>());
			return view;
		}
		Scene::FormsByMaterialView Scene::GetVisibleFormsByMaterial(const math::ViewFrustum<> &frustum) const
		{
			FormsByMaterialView view;
			// FIXME: implement
			return view;
		}

		// light views
		Scene::View<Light>::Type Scene::GetLights() const
		{
			View<Light>::Type view;
			view.reserve(lights.size());
			std::transform(
				util::make_indirect_iterator(lights.begin()),
				util::make_indirect_iterator(lights.end()),
				std::back_inserter(view), util::address_of<Light>());
			return view;
		}
		Scene::View<Light>::Type Scene::GetInfluentialLights(const math::ViewFrustum<> &frustum) const
		{
			View<Light>::Type view;
			view.reserve(lights.size());
			// FIXME: should only copy influential lights
			std::transform(
				util::make_indirect_iterator(lights.begin()),
				util::make_indirect_iterator(lights.end()),
				std::back_inserter(view), util::address_of<Light>());
			return view;
		}

		// particle views
		Scene::View<Particle>::Type Scene::GetParticles() const
		{
			View<Particle>::Type view;
			view.reserve(particles.size());
			std::transform(
				util::make_indirect_iterator(particles.begin()),
				util::make_indirect_iterator(particles.end()),
				std::back_inserter(view), util::address_of<Particle>());
			return view;
		}
		Scene::View<Particle>::Type Scene::GetVisibleParticles(const math::ViewFrustum<> &frustum) const
		{
			View<Particle>::Type view;
			view.reserve(particles.size());
			// FIXME: should only copy visible particles
			std::transform(
				util::make_indirect_iterator(particles.begin()),
				util::make_indirect_iterator(particles.end()),
				std::back_inserter(view), util::address_of<Particle>());
			return view;
		}

		// sound views
		Scene::View<Sound>::Type Scene::GetSounds() const
		{
			View<Sound>::Type view;
			view.reserve(sounds.size());
			std::transform(
				util::make_indirect_iterator(sounds.begin()),
				util::make_indirect_iterator(sounds.end()),
				std::back_inserter(view), util::address_of<Sound>());
			return view;
		}
		Scene::View<Sound>::Type Scene::GetClosestSounds(const math::Vector<3> &co, unsigned n) const
		{
			View<Sound>::Type view;
			view.reserve(n);
			// FIXME: find n closest sounds
			return view;
		}

		// trackable views
		Scene::View<Trackable>::Type Scene::GetTrackables() const
		{
			View<Trackable>::Type view;
			view.reserve(trackables.size());
			std::transform(
				util::make_indirect_iterator(trackables.begin()),
				util::make_indirect_iterator(trackables.end()),
				std::back_inserter(view), util::address_of<Trackable>());
			return view;
		}

		////////////////////////////////////////////////////////////////////////
		// atmospherics

		// state
		const math::Vector<3> &Scene::GetSunDirection() const
		{
			return sunDirection;
		}

		// modifiers
		void Scene::SetSunDirection(const math::Vector<3> &dir)
		{
			assert(All(Near(dir, Norm(dir))));
			sunDirection = dir;
		}

		////////////////////////////////////////////////////////////////////////

		// track access
		bool Scene::HasTrack() const
		{
			return track != nullptr;
		}
		const res::Track &Scene::GetTrack() const
		{
			assert(track);
			return *track;
		}

		// update
		void Scene::Update(float deltaTime)
		{
			UpdateControllables(Controller::preCollisionLayer, deltaTime);
			UpdateForces();
			UpdateCollidables(
				util::make_indirect_iterator(collidables.begin()),
				util::make_indirect_iterator(collidables.end()));
			UpdateTrackables(
				util::make_indirect_iterator(trackables.begin()),
				util::make_indirect_iterator(trackables.end()));
			UpdateDeltas();
			UpdateControllables(Controller::postCollisionLayer, deltaTime);
			// FIXME: update constraints
			UpdateControllables(Controller::constraintLayer, deltaTime);
			UpdateCameraTracking();
			UpdateObjects(deltaTime);
			// update focus
			if (focus.target) focus.position = focus.target->GetPosition();
		}

		// modifiers
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
			Init(scene);
		}

		// insertion
		void Scene::Insert(const std::shared_ptr<Body> &body)
		{
			assert(body);
			bodies.push_back(body);
			Insert(std::static_pointer_cast<Form>(body));
			Insert(std::static_pointer_cast<Collidable>(body));
		}
		void Scene::Insert(const std::shared_ptr<Camera> &camera)
		{
			assert(camera);
			cameras.push_back(camera);
			Insert(std::static_pointer_cast<Controllable>(camera));
			Insert(std::static_pointer_cast<Transformable>(camera));
		}
		void Scene::Insert(const std::shared_ptr<Collidable> &collidable)
		{
			assert(collidable);
			collidables.push_back(collidable);
			Insert(std::static_pointer_cast<Trackable>(collidable));
		}
		void Scene::Insert(const std::shared_ptr<Controllable> &controllable)
		{
			assert(controllable);
			controllables.push_back(controllable);
		}
		void Scene::Insert(const std::shared_ptr<Emitter> &emitter)
		{
			assert(emitter);
			emitters.push_back(emitter);
			Insert(std::static_pointer_cast<Controllable>(emitter));
			Insert(std::static_pointer_cast<Transformable>(emitter));
		}
		void Scene::Insert(const std::shared_ptr<Form> &form)
		{
			assert(form);
			forms.push_back(form);
			Insert(std::static_pointer_cast<Controllable>(form));
			Insert(std::static_pointer_cast<Transformable>(form));
		}
		void Scene::Insert(const std::shared_ptr<Light> &light)
		{
			assert(light);
			lights.push_back(light);
			Insert(std::static_pointer_cast<Controllable>(light));
			Insert(std::static_pointer_cast<Transformable>(light));
		}
		void Scene::Insert(const std::shared_ptr<Particle> &particle)
		{
			assert(particle);
			particles.push_back(particle);
			Insert(std::static_pointer_cast<Transformable>(particle));
		}
		void Scene::Insert(const std::shared_ptr<Sound> &sound)
		{
			assert(sound);
			sounds.push_back(sound);
			Insert(std::static_pointer_cast<Controllable>(sound));
			Insert(std::static_pointer_cast<Transformable>(sound));
		}
		void Scene::Insert(const std::shared_ptr<Trackable> &trackable)
		{
			assert(trackable);
			trackables.push_back(trackable);
			// NOTE: Transformable is inserted by the objects that implement it
		}
		void Scene::Insert(const std::shared_ptr<Transformable> &transformable)
		{
			assert(transformable);
			transformables.push_back(transformable);
		}

		// deletion
		void Scene::Remove(const std::shared_ptr<Body> &body)
		{
			assert(body);
			bodies.erase(
				std::remove(bodies.begin(), bodies.end(), body),
				bodies.end());
			Remove(std::static_pointer_cast<Form>(body));
			Remove(std::static_pointer_cast<Collidable>(body));
		}
		void Scene::Remove(const std::shared_ptr<Camera> &camera)
		{
			assert(camera);
			cameras.erase(
				std::remove(cameras.begin(), cameras.end(), camera),
				cameras.end());
			Remove(std::static_pointer_cast<Controllable>(camera));
			Remove(std::static_pointer_cast<Transformable>(camera));
		}
		void Scene::Remove(const std::shared_ptr<Collidable> &collidable)
		{
			assert(collidable);
			collidables.erase(
				std::remove(collidables.begin(), collidables.end(), collidable),
				collidables.end());
			Remove(std::static_pointer_cast<Trackable>(collidable));
		}
		void Scene::Remove(const std::shared_ptr<Controllable> &controllable)
		{
			assert(controllable);
			controllables.erase(
				std::remove(controllables.begin(), controllables.end(), controllable),
				controllables.end());
		}
		void Scene::Remove(const std::shared_ptr<Emitter> &emitter)
		{
			assert(emitter);
			emitters.erase(
				std::remove(emitters.begin(), emitters.end(), emitter),
				emitters.end());
			Remove(std::static_pointer_cast<Controllable>(emitter));
			Remove(std::static_pointer_cast<Transformable>(emitter));
		}
		void Scene::Remove(const std::shared_ptr<Form> &form)
		{
			assert(form);
			forms.erase(
				std::remove(forms.begin(), forms.end(), form),
				forms.end());
			Remove(std::static_pointer_cast<Controllable>(form));
			Remove(std::static_pointer_cast<Transformable>(form));
		}
		void Scene::Remove(const std::shared_ptr<Light> &light)
		{
			assert(light);
			lights.erase(
				std::remove(lights.begin(), lights.end(), light),
				lights.end());
			Remove(std::static_pointer_cast<Controllable>(light));
			Remove(std::static_pointer_cast<Transformable>(light));
		}
		void Scene::Remove(const std::shared_ptr<Particle> &particle)
		{
			assert(particle);
			particles.erase(
				std::remove(particles.begin(), particles.end(), particle),
				particles.end());
			Remove(std::static_pointer_cast<Transformable>(particle));
		}
		void Scene::Remove(const std::shared_ptr<Sound> &sound)
		{
			assert(sound);
			sounds.erase(
				std::remove(sounds.begin(), sounds.end(), sound),
				sounds.end());
			Remove(std::static_pointer_cast<Controllable>(sound));
			Remove(std::static_pointer_cast<Transformable>(sound));
		}
		void Scene::Remove(const std::shared_ptr<Trackable> &trackable)
		{
			assert(trackable);
			trackables.erase(
				std::remove(trackables.begin(), trackables.end(), trackable),
				trackables.end());
			// NOTE: Transformable is inserted by the objects that implement it
		}
		void Scene::Remove(const std::shared_ptr<Transformable> &transformable)
		{
			assert(transformable);
			transformables.erase(
				std::remove(transformables.begin(), transformables.end(), transformable),
				transformables.end());
		}

		// focus modifiers
		const math::Vector<3> &Scene::GetFocus() const
		{
			return focus.position;
		}
		void Scene::SetFocus()
		{
			focus.target.reset();
		}
		void Scene::SetFocus(const math::Vector<3> &position)
		{
			focus.target.reset();
			focus.position = position;
		}
		void Scene::SetFocus(const std::shared_ptr<const Body> &target)
		{
			focus.target = target;
			focus.position = target->GetPosition();
		}

		// camera mode
		Scene::CameraMode Scene::GetCameraMode() const
		{
			return cameraMode;
		}
		void Scene::UseDetailCamera(const inp::Driver &driver)
		{
			assert(focus.target);
			std::shared_ptr<Camera> camera(new Camera);
			camera->SetOpacity(0);
			camera->InsertController(HeroCamController(driver, *focus.target));
			Insert(camera);
			cameraType = detailCameraType;
			cameraMode = detailCameraMode;
		}
		void Scene::UseSceneCamera()
		{
			cameraMode = sceneCameraMode;
		}

		// frame serialization
		Frame Scene::GetFrame() const
		{
			return Ambient::GetFrame();
		}
		void Scene::Update(const Frame &frame)
		{
			Ambient::Update(frame);
		}

		// initialization
		void Scene::Init(const res::Scene &scene)
		{
			for (res::Scene::Forms::const_iterator form(scene.forms.begin()); form != scene.forms.end(); ++form)
				Insert(std::shared_ptr<Form>(new Form(*form)));
			if (scene.track) track = scene.track.lock();
			if (scene.cameraSet)
			{
				cameraSet = scene.cameraSet.lock();
				cameraSetCamera = cameraSet->cameras.end();
			}
			// initialize atmospherics
			SetAmbient(.2);
			sunDirection = Norm(-math::Vector<3>(.5, 1, .5));
		}

		// update
		void Scene::UpdateControllables(Controller::Layer layer, float deltaTime)
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
					if (cameraType != cameraSetCameraType ||
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
							camera->InsertController(FollowController(
								*focus.target, .5,
								cameraSetCamera->orientation,
								cameraSetCamera->trackingDistance));
							if (!cameraSetCamera->trackingDistance)
								camera->InsertController(
									ConstrainPositionToPlaneController(
										math::Plane<3>(
											camera->GetNormal(),
											camera->GetPosition())));
							break;
							case res::CameraSet::Camera::orientationTracking:
							camera->InsertController(CameraFocusController(*camera, *focus.target));
							break;
						}
						UpdateLateControllable(*camera);
						Insert(camera);
						cameraType = cameraSetCameraType;
					}
				}
				else
				{
					FollowCamera:
					// use default follow camera
					if (cameraType != followCameraType)
					{
						std::shared_ptr<Camera> camera(new Camera);
						camera->SetOpacity(0);
						camera->InsertController(followCameraController =
							std::shared_ptr<FollowController>(
								new FollowController(*focus.target, .5,
									math::Quat<>(math::Euler<>(
										math::DegToRad(22.5f),
										math::DegToRad(-45.f))), 8)));
						UpdateLateControllable(*camera);
						Insert(camera);
						cameraType = followCameraType;
					}
					else followCameraController->Follow(*focus.target);
				}
			}
			else
			{
				// use default overview camera
				if (cameraType != overviewCameraType)
				{
					// FIXME: implement
					cameraType = overviewCameraType;
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

		// hash functions
		std::size_t Scene::MaterialHash::operator ()(const util::copy_ptr<cache::Proxy<res::Material>> &mat) const
		{
			return std::hash<cache::Proxy<res::Material>>()(*mat);
		}
	}
}
