// scene database
// provides multiple views of contained objects

#ifndef    page_local_phys_Scene_hpp
#   define page_local_phys_Scene_hpp

#	include <functional> // unary_function
#	include <memory> // shared_ptr
#	include <unordered_map> // unordered_multimap
#	include <vector>

#	include "../cache/fwd.hpp" // Proxy
#	include "../math/fwd.hpp" // Vector, ViewFrustum
#	include "../res/type/CameraSet.hpp" // CameraSet::Cameras
#	include "../util/raii/copy_ptr.hpp"
#	include "attrib/Ambient.hpp"
#	include "Form.hpp" // Form::Part
#	include "mixin/Controllable.hpp"

namespace page
{
	namespace inp { class Driver; }
	namespace res
	{
		class Material;
		class Scene;
		class Track;
	}

	namespace phys
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

		struct Scene : Controllable,
			attrib::Ambient
		{
			// construct
			Scene();
			Scene(const res::Scene &);

			////////////////////////////////////////////////////////////////////
			// contained object views

			private:
			// hash function forward declarations for views
			class MaterialHash;

			public:
			// view types
			template <typename T> struct View
			{
				typedef std::vector<const T *> Type;
			};

			// body views
			View<Body>::Type GetBodies() const;
			View<Body>::Type GetVisibleBodies(const math::ViewFrustum<> &) const;

			// camera views
			View<Camera>::Type GetCameras() const;

			// collidable views
			View<Collidable>::Type GetCollidables() const;
			View<Collidable>::Type GetVisibleCollidables(const math::ViewFrustum<> &) const;

			// form views
			View<Form>::Type GetForms() const;
			View<Form>::Type GetVisibleForms(const math::ViewFrustum<> &) const;
			typedef std::unordered_multimap<util::copy_ptr<cache::Proxy<res::Material>>, const Form::Part *, MaterialHash> FormsByMaterialView;
			FormsByMaterialView GetVisibleFormsByMaterial(const math::ViewFrustum<> &) const;

			// light views
			View<Light>::Type GetLights() const;
			View<Light>::Type GetInfluentialLights(const math::ViewFrustum<> &) const;

			// particle views
			View<Particle>::Type GetParticles() const;
			View<Particle>::Type GetVisibleParticles(const math::ViewFrustum<> &) const;

			// sound views
			View<Sound>::Type GetSounds() const;
			View<Sound>::Type GetClosestSounds(const math::Vec3 &, unsigned n) const;

			// trackable views
			View<Trackable>::Type GetTrackables() const;

			////////////////////////////////////////////////////////////////////
			// atmospherics

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

			////////////////////////////////////////////////////////////////////

			// track access
			bool HasTrack() const;
			const res::Track &GetTrack() const;

			// update
			void Update(float deltaTime);

			// modifiers
			void Clear();
			void Reset(const res::Scene &);

			// insertion
			void Insert(const std::shared_ptr<Body> &);
			void Insert(const std::shared_ptr<Camera> &);
			void Insert(const std::shared_ptr<Collidable> &);
			void Insert(const std::shared_ptr<Controllable> &);
			void Insert(const std::shared_ptr<Emitter> &);
			void Insert(const std::shared_ptr<Form> &);
			void Insert(const std::shared_ptr<Light> &);
			void Insert(const std::shared_ptr<Particle> &);
			void Insert(const std::shared_ptr<Sound> &);
			void Insert(const std::shared_ptr<Trackable> &);
			void Insert(const std::shared_ptr<Transformable> &);

			// deletion
			void Remove(const std::shared_ptr<Body> &);
			void Remove(const std::shared_ptr<Camera> &);
			void Remove(const std::shared_ptr<Collidable> &);
			void Remove(const std::shared_ptr<Controllable> &);
			void Remove(const std::shared_ptr<Emitter> &);
			void Remove(const std::shared_ptr<Form> &);
			void Remove(const std::shared_ptr<Light> &);
			void Remove(const std::shared_ptr<Particle> &);
			void Remove(const std::shared_ptr<Sound> &);
			void Remove(const std::shared_ptr<Trackable> &);
			void Remove(const std::shared_ptr<Transformable> &);

			// focus access
			const math::Vec3 &GetFocus() const;
			void SetFocus();
			void SetFocus(const math::Vec3 &);
			void SetFocus(const std::shared_ptr<const Body> &);

			// camera mode
			enum CameraMode
			{
				detailCameraMode,
				sceneCameraMode
			};
			CameraMode GetCameraMode() const;
			void UseDetailCamera(const inp::Driver &);
			void UseSceneCamera();

			protected:
			// frame serialization
			Frame GetFrame() const;
			void Update(const Frame &);

			private:
			// initialization
			void Init(const res::Scene &);

			// update
			void UpdateControllables(Controller::Layer, float deltaTime);
			void UpdateForces();
			void UpdateDeltas();
			void UpdateCameraTracking();
			void UpdateObjects(float deltaTime);

			// hash functions
			struct MaterialHash : std::unary_function<util::copy_ptr<cache::Proxy<res::Material>>, std::size_t>
			{
				std::size_t operator ()(const util::copy_ptr<cache::Proxy<res::Material>> &) const;
			};

			// contained objects
			template <typename T> struct Set
			{
				typedef std::vector<std::shared_ptr<T>> Type;
			};
			typedef Set<Body>::Type          Bodies;         Bodies         bodies;
			typedef Set<Camera>::Type        Cameras;        Cameras        cameras;
			typedef Set<Collidable>::Type    Collidables;    Collidables    collidables;
			typedef Set<Controllable>::Type  Controllables;  Controllables  controllables;
			typedef Set<Emitter>::Type       Emitters;       Emitters       emitters;
			typedef Set<Form>::Type          Forms;          Forms          forms;
			typedef Set<Light>::Type         Lights;         Lights         lights;
			typedef Set<Particle>::Type      Particles;      Particles      particles;
			typedef Set<Sound>::Type         Sounds;         Sounds         sounds;
			typedef Set<Trackable>::Type     Trackables;     Trackables     trackables;
			typedef Set<Transformable>::Type Transformables; Transformables transformables;

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
			enum CameraType
			{
				noCameraType,
				cameraSetCameraType,
				detailCameraType,
				followCameraType,
				overviewCameraType
			} cameraType;
			res::CameraSet::Cameras::const_iterator cameraSetCamera;
			std::shared_ptr<FollowController> followCameraController;

			// atmospherics
			math::Vec3 sunDirection;
		};
	}
}

#endif
