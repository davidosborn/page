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

#include <memory> // unique_ptr

#include "../../../vid/opengl/Drawable.hpp"
#include "../Skin.hpp"
#include "DrawableProxy.hpp"

namespace page { namespace cache { namespace opengl
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

	/*-------------+
	| constructors |
	+-------------*/

	DrawableProxy::DrawableProxy(const Proxy<res::Mesh> &mesh) :
		DrawableProxy(mesh, 0) {}

	DrawableProxy::DrawableProxy(const phys::Form::Part &part) :
		DrawableProxy(part.GetMesh(), part.GetId()) {}

	DrawableProxy::DrawableProxy(const Proxy<res::Mesh> &mesh, util::Identifiable::Id partId) :
		BasicProxy<vid::opengl::Drawable>(Signature("OpenGL drawable", mesh, partId)),
		mesh(mesh), partId(partId) {}

	/*--------------------------+
	| BasicProxy implementation |
	+--------------------------*/

	auto DrawableProxy::DoLock() const -> pointer
	{
		if (partId)
		{
			// create drawable
			phys::Form::Part &part(util::GetReferenceById<phys::Form::Part>(partId));
			bool dynamic = part.GetForm().IsPosed() || part.IsDeformed();
			std::unique_ptr<vid::opengl::Drawable> drawable(
				vid::opengl::MakeDrawable(*mesh, dynamic));

			// update to current pose
			if (dynamic)
				drawable->Update(*Skin(part.GetMesh(), part.GetForm()));

			std::bind(opengl::Repair, std::placeholders::_1, Skin(part.GetMesh(), part.GetForm()))),
			// bind dirty signal handler
			boost::signals::scoped_connection con(part.GetForm().dirtyPoseSig.connect(GetInvalidateFunction()));
			return pointer(drawable.release(),
				std::bind(Delete, std::placeholders::_1, con.release()));
		}
		return pointer(vid::opengl::MakeDrawable(*mesh));
	}
}}}
