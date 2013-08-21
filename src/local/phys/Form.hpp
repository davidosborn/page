// geometry object

#ifndef    page_local_phys_Form_hpp
#   define page_local_phys_Form_hpp

#	include "../cache/proxy/Proxy.hpp"
#	include "../res/type/Model.hpp" // Model::Part
#	include "../res/type/Scene.hpp" // Scene::Form
#	include "../util/Identifiable.hpp"
#	include "attrib/Deformation.hpp"
#	include "attrib/Material.hpp"
#	include "attrib/Mesh.hpp"
#	include "attrib/Opacity.hpp"
#	include "attrib/Pose.hpp"
#	include "attrib/PositionOrientationScale.hpp"
#	include "mixin/Controllable.hpp"

namespace page
{
	namespace phys
	{
		struct Form : Controllable,
			attrib::Opacity,
			attrib::Pose
		{
			struct Part : util::Identifiable,
				attrib::Deformation,
				attrib::Material,
				attrib::Mesh,
				attrib::PositionOrientationScale
			{
				// construct
				Part(Form &, const res::Model::Part &);

				// owner access
				Form &GetForm();
				const Form &GetForm() const;

				// ancestor access
				const res::Model::Part &GetModelPart() const;

				protected:
				// frame serialization
				Frame GetFrame() const;
				void Update(const Frame &);

				private:
				Form *form;
			};

			// construct
			explicit Form(const res::Scene::Form &);
			explicit Form(const cache::Proxy<res::Model> &);

			// part access
			typedef std::vector<Part> Parts;
			Parts &GetParts();
			const Parts &GetParts() const;

			// ancestor access
			const cache::Proxy<res::Model> &GetModel() const;

			protected:
			// frame serialization
			Frame GetFrame() const;
			void Update(const Frame &);

			private:
			// initialization
			void InitParts();

			Parts parts;
			cache::Proxy<res::Model> model;
		};
	}
}

#endif
