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

////////// Form::Part //////////////////////////////////////////////////////////

		public:
		class Part :
			public Controllable,
			public attrib::Deformation,
			public attrib::Material,
			public attrib::Mesh,
			public attrib::PositionOrientationScale,
			public util::Identifiable
		{
			/*-------------+
			| constructors |
			+-------------*/

			public:
			Part(Form &, const res::Model::Part &);

			/*----------+
			| observers |
			+----------*/

			Form &GetForm();
			const Form &GetForm() const;
			const res::Model::Part &GetModelPart() const;

			/*--------------------+
			| frame serialization |
			+--------------------*/

			protected:
			Frame GetFrame() const override;
			void SetFrame(const Frame &) override;

			/*-------------+
			| data members |
			+-------------*/

			private:
			Form *form;
		};

////////// Form ////////////////////////////////////////////////////////////////

		/*------+
		| types |
		+------*/

		typedef std::vector<Part> Parts;

		/*-------------+
		| constructors |
		+-------------*/

		explicit Form(const res::Scene::Form &);
		explicit Form(const cache::Proxy<res::Model> &);

		private:
		void InitParts();

		/*----------+
		| observers |
		+----------*/

		public:
		Parts &GetParts();
		const Parts &GetParts() const;
		const cache::Proxy<res::Model> &GetModel() const;

		/*--------------------+
		| frame serialization |
		+--------------------*/

		protected:
		Frame GetFrame() const override;
		void SetFrame(const Frame &) override;

		/*-------------+
		| data members |
		+-------------*/

		private:
		Parts parts;
		cache::Proxy<res::Model> model;
	};
}}

#endif
