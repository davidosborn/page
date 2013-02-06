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

#include <cassert>
#include "../cache/Proxy.hpp"
#include "Form.hpp"

namespace page
{
	namespace phys
	{
		// form part
		// construct
		Form::Part::Part(Form &form, const res::Model::Part &part) :
			Material(part.material), Mesh(part.mesh), form(&form) {}

		// owner access
		Form &Form::Part::GetForm()
		{
			assert(form);
			return *form;
		}
		const Form &Form::Part::GetForm() const
		{
			assert(form);
			return *form;
		}

		// ancestor access
		const res::Model::Part &Form::Part::GetModelPart() const
		{
			assert(GetForm().GetModel());
			// FIXME: implement; need more information
		}

		// frame serialization
		Frame Form::Part::GetFrame() const
		{
			return
				Deformation::GetFrame() +
				PositionOrientationScale::GetFrame();
		}
		void Form::Part::Update(const Frame &frame)
		{
			Deformation::Update(frame);
			PositionOrientationScale::Update(frame);
		}

		// form
		// construct
		Form::Form(const res::Scene::Form &form) : Form(form.model)
		{
			// initialize transform
			SetPosition(form.position);
			SetOrientation(form.orientation);
			SetScale(form.scale);
		}
		Form::Form(const cache::Proxy<res::Model> &model) :
			Pose(model && model->skeleton ? Pose(*model->skeleton) : Pose()),
			model(model.Copy())
		{
			if (model)
			{
				assert(this->model && *this->model);
				const res::Model &model(**this->model);
				// copy model parts
				parts.reserve(model.parts.size());
				for (res::Model::Parts::const_iterator part(model.parts.begin()); part != model.parts.end(); ++part)
					parts.push_back(Part(*this, *part));
			}
		}

		// part access
		Form::Parts &Form::GetParts()
		{
			return parts;
		}
		const Form::Parts &Form::GetParts() const
		{
			return parts;
		}

		// ancestor access
		const cache::Proxy<res::Model> &Form::GetModel() const
		{
			assert(model);
			return *model;
		}

		// frame serialization
		Frame Form::GetFrame() const
		{
			return
				Opacity::GetFrame() +
				Pose::GetFrame();
		}
		void Form::Update(const Frame &frame)
		{
			Opacity::Update(frame);
			Pose::Update(frame);
		}
	}
}
