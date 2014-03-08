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

#include <cassert>

#include "Form.hpp"

namespace page { namespace phys
{
////////// Form::Part //////////////////////////////////////////////////////////

	/*-------------+
	| constructors |
	+-------------*/

	Form::Part::Part(Form &form, const res::Model::Part &part) :
		Material(part.material), Mesh(part.mesh), form(&form) {}

	/*----------+
	| observers |
	+----------*/

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

	const res::Model::Part &Form::Part::GetModelPart() const
	{
		assert(GetForm().GetModel());
		// FIXME: implement; need more information
	}

	/*--------------------+
	| frame serialization |
	+--------------------*/

	Frame Form::Part::GetFrame() const
	{
		return
			Deformation             ::GetFrame() +
			PositionOrientationScale::GetFrame();
	}

	void Form::Part::SetFrame(const Frame &frame)
	{
		Deformation             ::SetFrame(frame);
		PositionOrientationScale::SetFrame(frame);
	}

////////// Form ////////////////////////////////////////////////////////////////

	/*-------------+
	| constructors |
	+-------------*/

	Form::Form(const res::Scene::Form &form) : Form(form.model)
	{
		// initialize transform
		SetPosition   (form.position);
		SetOrientation(form.orientation);
		SetScale      (form.scale);
	}

	Form::Form(const cache::Proxy<res::Model> &model) :
		Pose(model && model->skeleton ? Pose(*model->skeleton) : Pose()),
		model(model)
	{
		if (model)
		{
			// copy model parts
			const auto &model(*this->model);
			parts.reserve(model.parts.size());
			for (const auto &part : model.parts)
				parts.push_back(Part(*this, part));
		}
	}

	/*----------+
	| observers |
	+----------*/

	Form::Parts &Form::GetParts()
	{
		return parts;
	}

	const Form::Parts &Form::GetParts() const
	{
		return parts;
	}

	const cache::Proxy<res::Model> &Form::GetModel() const
	{
		return model;
	}

	/*--------------------+
	| frame serialization |
	+--------------------*/

	Frame Form::GetFrame() const
	{
		return
			Opacity::GetFrame() +
			Pose   ::GetFrame();
	}

	void Form::SetFrame(const Frame &frame)
	{
		Opacity::SetFrame(frame);
		Pose   ::SetFrame(frame);
	}
}}
