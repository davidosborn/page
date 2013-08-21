#include <cassert>

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
			model(model)
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
