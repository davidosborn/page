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

#include "../../phys/attrib/Pose.hpp" // Pose->util::Identifiable
#include "../../phys/Skin.hpp"
#include "../../util/lexical_cast.hpp"
#include "Skin.hpp"

namespace page
{
	namespace cache
	{
		// construct
		Skin::Skin(const Proxy<res::Mesh> &mesh, const phys::attrib::Pose &pose) :
			mesh(mesh.Copy()), poseId(pose.GetId()) {}

		// clone
		Skin *Skin::Clone() const
		{
			return new Skin(*this);
		}

		// attributes
		std::string Skin::GetType() const
		{
			return "skin";
		}
		std::string Skin::GetSource() const
		{
			return mesh->GetSource() + ':' +
				util::lexical_cast<std::string>(poseId);
		}

		// dependency satisfaction
		Skin::operator bool() const
		{
			return *mesh && util::Identifiable::FromId(poseId);
		}

		// instantiation
		Skin::Instance Skin::Make() const
		{
			return Instance(new phys::Skin(**mesh,
				util::ReferenceFromId<phys::attrib::Pose>(poseId)));
		}
	}
}
