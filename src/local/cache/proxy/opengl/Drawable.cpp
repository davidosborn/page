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

#include "../../../util/copy_ptr.hpp"
#include "../../../util/lexical_cast.hpp"
#include "../../../util/scoped_ptr.hpp"
#include "../../../vid/opengl/Drawable.hpp"
#include "../Skin.hpp"
#include "Drawable.hpp"

namespace page
{
	namespace cache
	{
		namespace opengl
		{
			namespace
			{
				// callbacks
				void Delete(const vid::opengl::Drawable *drawable, util::Connection &con)
				{
					con.Disconnect();
					delete drawable;
				}
				void Repair(vid::opengl::Drawable &drawable, const Skin &skin)
				{
					drawable.Update(*skin);
				}
			}

			// construct
			Drawable::Drawable(const Proxy<res::Mesh> &mesh) :
				mesh(mesh.Copy()), partId(0) {}
			Drawable::Drawable(const phys::Form::Part &part) :
				Proxy<vid::opengl::Drawable>(
					std::bind(opengl::Repair,
						std::placeholders::_1,
						Skin(part.GetMesh(), part.GetForm()))),
				mesh(part.GetMesh().Copy()), partId(part.GetId()) {}

			// clone
			Drawable *Drawable::Clone() const
			{
				return new Drawable(*this);
			}

			// attributes
			std::string Drawable::GetType() const
			{
				return "drawable";
			}
			std::string Drawable::GetSource() const
			{
				return mesh->GetSource() + ':' +
					util::lexical_cast<std::string>(partId);
			}

			// dependency satisfaction
			Drawable::operator bool() const
			{
				return *mesh;
			}

			// instantiation
			Drawable::Instance Drawable::Make() const
			{
				if (partId)
				{
					// create drawable
					phys::Form::Part &part(util::ReferenceFromId<phys::Form::Part>(partId));
					bool dynamic = part.GetForm().IsPosed() || part.IsDeformed();
					util::scoped_ptr<vid::opengl::Drawable> drawable(
						vid::opengl::MakeDrawable(**mesh, dynamic));
					// update to current pose
					if (dynamic)
						drawable->Update(*Skin(part.GetMesh(), part.GetForm()));
					// bind dirty signal handler
					util::ScopedConnection con(part.GetForm().dirtyPoseSig.Connect(GetInvalidate()));
					return Instance(drawable.release(),
						std::bind(Delete, std::placeholders::_1, con.Release()));
				}
				return Instance(vid::opengl::MakeDrawable(**mesh));
			}
		}
	}
}
