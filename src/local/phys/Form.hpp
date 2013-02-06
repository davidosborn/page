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

// geometry object

#ifndef    page_local_phys_Form_hpp
#   define page_local_phys_Form_hpp

#	include "../cache/fwd.hpp" // Proxy
#	include "../res/type/Model.hpp" // Model::Part
#	include "../res/type/Scene.hpp" // Scene::Form
#	include "../util/copy_ptr.hpp"
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
			util::copy_ptr<cache::Proxy<res::Model>> model;
		};
	}
}

#endif
