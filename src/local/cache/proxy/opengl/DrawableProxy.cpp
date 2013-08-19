#include <memory> // unique_ptr

#include "../../../util/string/StringBuilder.hpp"
#include "../../../vid/opengl/Drawable.hpp"
#include "../Skin.hpp"
#include "Drawable.hpp"

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
		mesh(mesh), partId(0) {}

	DrawableProxy::DrawableProxy(const phys::Form::Part &part) :
		BasicProxy<vid::opengl::Drawable>(
			std::bind(opengl::Repair,
				std::placeholders::_1,
				Skin(part.GetMesh(), part.GetForm()))),
		mesh(part.GetMesh().Copy()), partId(part.GetId()) {}

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
			boost::signals::scoped_connection con(part.GetForm().dirtyPoseSig.connect(GetInvalidateFunction()));
			return Instance(drawable.release(),
				std::bind(Delete, std::placeholders::_1, con.release()));
		}
		return Instance(vid::opengl::MakeDrawable(**mesh));
	}
}}}
