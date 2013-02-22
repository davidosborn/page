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

#include <algorithm> // copy, find, sort, transform, unique
#include <cassert>
#include <functional> // bind, mem_fun_ref
#include <iterator> // back_inserter
#include <sstream> // ostringstream

#include "../../phys/Bounds.hpp"
#include "../../res/type/Model.hpp"
#include "../../util/functional/pointer.hpp" // dereference
#include "../../util/io/separated_ostream_iterator.hpp"
#include "../../util/iterator/call_iterator.hpp"
#include "../../util/iterator/indirect_iterator.hpp"
#include "../../util/iterator/member_iterator.hpp"
#include "Bounds.hpp"

namespace page
{
	namespace cache
	{
		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		Bounds::Bounds(const res::Model &model, bool pose)
		{
			std::transform(
				util::make_member_iterator(model.parts.begin(), &res::Model::Part::mesh),
				util::make_member_iterator(model.parts.end(),   &res::Model::Part::mesh),
				std::back_inserter(meshes),
				std::mem_fun_ref(&Proxy<res::Mesh>::Copy));

			if (pose && model.skeleton)
				skeleton = model.skeleton.Copy();

			PostInit();
		}

		Bounds::Bounds(const Proxy<res::Mesh> &mesh) :
			Bounds(&mesh, &mesh + 1) {}

		Bounds::Bounds(const Proxy<res::Mesh> &mesh, const Proxy<res::Skeleton> &skeleton) :
			Bounds(&mesh, &mesh + 1, skeleton) {}

		/*------+
		| clone |
		+------*/

		Bounds *Bounds::Clone() const
		{
			return new Bounds(*this);
		}

		/*----------+
		| observers |
		+----------*/

		std::string Bounds::GetType() const
		{
			return "bounds";
		}

		std::string Bounds::GetSource() const
		{
			std::ostringstream ss;
			std::copy(
				util::make_call_iterator(util::make_indirect_iterator(meshes.begin()),
					std::mem_fun_ref(&Proxy<res::Mesh>::GetSource)),
				util::make_call_iterator(util::make_indirect_iterator(meshes.end()),
					std::mem_fun_ref(&Proxy<res::Mesh>::GetSource)),
				util::separated_ostream_iterator<std::string>(ss, ','));

			if (skeleton)
				// NOTE: using alternate separator to differentiate from mesh
				ss << '+' << skeleton->GetSource();

			return ss.str();
		}

		Bounds::operator bool() const
		{
			if (std::find(
				util::make_indirect_iterator(meshes.begin()),
				util::make_indirect_iterator(meshes.end()),
				false).base() != meshes.end())
					return false;

			if (skeleton && !*skeleton)
				return false;

			return true;
		}

		/*--------------+
		| instantiation |
		+--------------*/

		Bounds::Instance Bounds::Make() const
		{
			return Instance(skeleton ?
				new phys::Bounds(
					util::make_indirect_iterator(util::make_indirect_iterator(meshes.begin())),
					util::make_indirect_iterator(util::make_indirect_iterator(meshes.end())),
					**skeleton) :
				new phys::Bounds(
					util::make_indirect_iterator(util::make_indirect_iterator(meshes.begin())),
					util::make_indirect_iterator(util::make_indirect_iterator(meshes.end()))));
		}

		/*---------------+
		| initialization |
		+---------------*/

		void Bounds::PostInit()
		{
			// ensure pointers are valid
			assert(std::find(meshes.begin(), meshes.end(), nullptr) == meshes.end());

			// sort meshes by source
			using namespace std::placeholders;
			sort(meshes.begin(), meshes.end(),
				bind(Proxy<res::Mesh>::CompareSource(),
					bind(util::dereference<util::copy_ptr<Proxy<res::Mesh>>>(), _1),
					bind(util::dereference<util::copy_ptr<Proxy<res::Mesh>>>(), _2)));

			// remove duplicate meshes
			meshes.erase(
				unique(meshes.begin(), meshes.end(),
					bind(Proxy<res::Mesh>::CompareSource(),
						bind(util::dereference<util::copy_ptr<Proxy<res::Mesh>>>(), _1),
						bind(util::dereference<util::copy_ptr<Proxy<res::Mesh>>>(), _2))),
				meshes.end());
		}
	}
}
