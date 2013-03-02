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

#include <memory> // unique_ptr

#include "../../../util/string/StringBuilder.hpp"
#include "../../../vid/opengl/Drawable.hpp"
#include "../Skin.hpp"
#include "Drawable.hpp"

namespace page
{
	namespace cache
	{
		namespace opengl
		{
			/*----------+
			| callbacks |
			+----------*/

			namespace
			{
				void Delete(const vid::opengl::Drawable *drawable, boost::signals::connection &con)
				{
					con.disconnect();
					delete drawable;
				}

				void Repair(vid::opengl::Drawable &drawable, const Skin &skin)
				{
					drawable.Update(*skin);
				}
			}

			/*--------------------------+
			| constructors & destructor |
			+--------------------------*/

			Drawable::Drawable(const Proxy<res::Mesh> &mesh) :
				mesh(mesh.Copy()), partId(0) {}

			Drawable::Drawable(const phys::Form::Part &part) :
				Proxy<vid::opengl::Drawable>(
					std::bind(opengl::Repair,
						std::placeholders::_1,
						Skin(part.GetMesh(), part.GetForm()))),
				mesh(part.GetMesh().Copy()), partId(part.GetId()) {}

			/*------+
			| clone |
			+------*/

			Drawable *Drawable::Clone() const
			{
				return new Drawable(*this);
			}

			/*----------+
			| observers |
			+----------*/

			std::string Drawable::GetType() const
			{
				return "drawable";
			}

			std::string Drawable::GetSource() const
			{
				return util::StringBuilder() <<
					mesh->GetSource() << ':' << partId;
			}

			Drawable::operator bool() const
			{
				return *mesh;
			}

			/*--------------+
			| instantiation |
			+--------------*/

			Drawable::Instance Drawable::Make() const
			{
				if (partId)
				{
					// create drawable
					phys::Form::Part &part(util::ReferenceFromId<phys::Form::Part>(partId));
					bool dynamic = part.GetForm().IsPosed() || part.IsDeformed();
					std::unique_ptr<vid::opengl::Drawable> drawable(
						vid::opengl::MakeDrawable(**mesh, dynamic));
					// update to current pose
					if (dynamic)
						drawable->Update(*Skin(part.GetMesh(), part.GetForm()));
					// bind dirty signal handler
					boost::signals::scoped_connection con(part.GetForm().dirtyPoseSig.connect(GetInvalidate()));
					return Instance(drawable.release(),
						std::bind(Delete, std::placeholders::_1, con.release()));
				}
				return Instance(vid::opengl::MakeDrawable(**mesh));
			}
		}
	}
}
