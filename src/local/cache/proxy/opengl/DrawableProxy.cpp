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
			phys::Form::Part &part(util::ReferenceFromId<phys::Form::Part>(partId));
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
