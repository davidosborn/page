#ifndef    page_local_phys_node_Form_hpp
#   define page_local_phys_node_Form_hpp

#	include "../../cache/proxy/Proxy.hpp"
#	include "../../res/type/Model.hpp" // Model::Part
#	include "../../res/type/Scene.hpp" // Scene::Form
#	include "../../util/Identifiable.hpp"
#	include "../attrib/Deformation.hpp"
#	include "../attrib/Material.hpp"
#	include "../attrib/Mesh.hpp"
#	include "../attrib/Opacity.hpp"
#	include "../attrib/Pose.hpp"
#	include "../attrib/PositionOrientationScale.hpp"
#	include "../mixin/Controllable.hpp"
#	include "Node.hpp"

namespace page { namespace phys
{
	/**
	 * A geometry object.
	 */
	class Form :
		public Node,
		public Controllable,
		public attrib::Opacity,
		public attrib::Pose
	{
		IMPLEMENT_CLONEABLE(Form, Node)

		public:
		class Part :
			public attrib::Deformation,
			public attrib::Material,
			public attrib::Mesh,
			public attrib::PositionOrientationScale,
			public util::Identifiable
		{
			public:
			// construct
			Part(Form &, const res::Model::Part &);

			// owner access
			Form &GetForm();
			const Form &GetForm() const;

			// ancestor access
			const res::Model::Part &GetModelPart() const;

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

		/*--------------------+
		| frame serialization |
		+--------------------*/

		protected:
		Frame GetFrame() const override;
		void SetFrame(const Frame &) override;

		private:
		// initialization
		void InitParts();

		Parts parts;
		cache::Proxy<res::Model> model;
	};
}}

#endif
