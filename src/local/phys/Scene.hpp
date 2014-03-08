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

#ifndef    page_local_phys_Scene_hpp
#   define page_local_phys_Scene_hpp

#	include <functional> // unary_function
#	include <memory> // shared_ptr
#	include <unordered_map> // unordered_multimap
#	include <vector>

#	include "../cache/proxy/Proxy.hpp"
#	include "../math/fwd.hpp" // Vector, ViewFrustum
#	include "../res/type/CameraSet.hpp" // CameraSet::Cameras
#	include "../util/container/reference_vector.hpp"
#	include "node/Form.hpp" // Form::Part

namespace page
{
	namespace inp { class Driver; }
	namespace res
	{
		class Material;
		class Scene;
		class Track;
	}
}

namespace page { namespace phys
{
	class Body;
	class Camera;
	class Collidable;
	class Emitter;
	class FollowController;
	class Light;
	class Particle;
	class Sound;
	class Trackable;
	class Transformable;

	/**
	 * A scene, which contains nodes.
	 */
	class Scene
	{
		/*------+
		| types |
		+------*/

		public:
		enum class CameraMode
		{
			detail,
			scene
		};

		enum class CameraType
		{
			none,
			cameraSet,
			detail,
			follow,
			overview
		};

		/*-------------+
		| constructors |
		+-------------*/

		/**
		 * Creates an empty scene.
		 */
		Scene() = default;

		/**
		 * Creates and populates a scene using the specified @c res::Scene
		 * resource.
		 */
		Scene(const res::Scene &);

		/*----------+
		| modifiers |
		+----------*/

		void Insert(const std::shared_ptr<Node> &);
		void Remove(const std::shared_ptr<Node> &);
		void Clear();
		void Reset(const res::Scene &);

		/*------+
		| views |
		+------*/

		/**
		 * Returns all of the bodies in the scene.
		 */
		util::reference_vector<Body> GetBodies() const;

		/**
		 * Returns all of the bodies that are visible within the specified view
		 * frustum.
		 */
		util::reference_vector<Body> GetVisibleBodies(const math::ViewFrustum<> &) const;

		/**
		 * Returns all of the cameras in the scene.
		 */
		util::reference_vector<Camera> GetCameras() const;

		/**
		 * Returns all of the collidable nodes in the scene.
		 */
		util::reference_vector<Collidable> GetCollidableNodes() const;

		/**
		 * Returns all of the collidable nodes that are visible within the
		 * specified view frustum.
		 */
		util::reference_vector<Collidable> GetVisibleCollidableNodes(const math::ViewFrustum<> &) const;

		/**
		 * Returns all of the forms in the scene.
		 */
		util::reference_vector<Form> GetForms() const;

		/**
		 * Returns all of the forms that are visible within the specified view
		 * frustum.
		 */
		util::reference_vector<Form> GetVisibleForms(const math::ViewFrustum<> &) const;

		/**
		 * Returns all of the forms that are visible within the specified view
		 * frustum, ordered by their material.
		 */
		typedef std::unordered_multimap<cache::Proxy<res::Material>, const Form::Part *> FormsByMaterialView;
		FormsByMaterialView GetVisibleFormsByMaterial(const math::ViewFrustum<> &) const;

		/**
		 * Returns all of the lights in the scene.
		 */
		util::reference_vector<Light> GetLights() const;

		/**
		 * Returns all of the lights that will have an effect on the specified
		 * view frustum.
		 */
		util::reference_vector<Light> GetInfluentialLights(const math::ViewFrustum<> &) const;

		/**
		 * Returns all of the particles in the scene.
		 */
		util::reference_vector<Particle> GetParticles() const;

		/**
		 * Returns all of the particles that are visible within the specified
		 * view frustum.
		 */
		util::reference_vector<Particle> GetVisibleParticles(const math::ViewFrustum<> &) const;

		/**
		 * Returns all of the sounds in the scene.
		 */
		util::reference_vector<Sound> GetSounds() const;

		/**
		 * Returns the @a n closest sounds to the specified position.
		 */
		util::reference_vector<Sound> GetClosestSounds(const math::Vec3 &, unsigned n) const;

		/**
		 * Returns all of the trackable nodes in the scene.
		 */
		util::reference_vector<Trackable> GetTrackableNodes() const;

		/*-------------+
		| atmospherics |
		+-------------*/

		// state
		const math::Vec3 &GetSunDirection() const;

		// sunlight access
		/*bool HasSunlight() const;
		const Sunlight &GetSunlight() const;*/

		// cloud access
		/*bool HasClouds() const;
		const Clouds &GetClouds() const;*/

		// precipitation access
		/*bool HasPrecipitation() const;
		const Precipitation &GetPrecipitation() const;*/

		// modifiers
		void SetSunDirection(const math::Vec3 &);

		/*-------+
		| camera |
		+-------*/

		CameraMode GetCameraMode() const;
		void UseDetailCamera(const inp::Driver &);
		void UseSceneCamera();

		/*------+
		| focus |
		+------*/

		const math::Vec3 &GetFocus() const;
		void SetFocus();
		void SetFocus(const math::Vec3 &);
		void SetFocus(const std::shared_ptr<const Body> &);

		/*------+
		| track |
		+------*/

		bool HasTrack() const;
		const res::Track &GetTrack() const;

		/*-------+
		| update |
		+-------*/

		public:
		void Update(float deltaTime);

		private:
		void UpdateControllables(AnimationLayer, float deltaTime);
		void UpdateForces();
		void UpdateDeltas();
		void UpdateCameraTracking();
		void UpdateObjects(float deltaTime);

		/*-------------+
		| data members |
		+-------------*/

		std::vector<std::shared_ptr<Node>> nodes;

		std::vector<Body *> bodies;
		std::vector<Camera *> cameras;
		std::vector<Collidable *> collidables;
		std::vector<Controllable *> controllables;
		std::vector<Emitter *> emitters;
		std::vector<Form *> forms;
		std::vector<Light *> lights;
		std::vector<Particle *> particles;
		std::vector<Sound *> sounds;
		std::vector<Trackable *> trackables;
		std::vector<Transformable *> transformables;

		// attributes
		std::shared_ptr<const res::Track> track;
		std::shared_ptr<const res::CameraSet> cameraSet;
		struct
		{
			std::shared_ptr<const Body> target;
			math::Vec3 position;
		} focus;

		// camera state
		CameraMode cameraMode;
		CameraType cameraType;
		res::CameraSet::Cameras::const_iterator cameraSetCamera;
		std::shared_ptr<FollowController> followCameraController;

		// atmospherics
		math::Vec3 sunDirection;
	};
}}

#endif
