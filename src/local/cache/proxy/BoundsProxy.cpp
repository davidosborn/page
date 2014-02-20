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

#include <boost/range/algorithm/find.hpp>
#include <boost/range/algorithm/sort.hpp>
#include <boost/range/algorithm/unique.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

#include "../../phys/Bounds.hpp"
#include "../../res/type/Model.hpp"
#include "../../util/iterator/indirect_iterator.hpp"
#include "BoundsProxy.hpp"

namespace page { namespace cache
{
	/*-------------+
	| constructors |
	+-------------*/

	BoundsProxy::BoundsProxy(const res::Model &model, bool pose) :
		BoundsProxy(
			GetGeometricallyDistinctMeshes(model),
			pose ? model.skeleton : nullptr) {}

	void BoundsProxy::Init()
	{
		// ensure dependencies are valid
		assert(boost::find(meshes, nullptr) == meshes.end());

		// uniquely sort meshes by signature
		boost::sort(meshes);
		boost::erase(meshes, boost::unique<boost::return_found_end>(meshes));
	}

	/*--------------------------+
	| BasicProxy implementation |
	+--------------------------*/

	auto BoundsProxy::DoLock() const -> pointer
	{
		return std::make_shared<phys::Bounds>(
			util::make_indirect_range(meshes), *skeleton);
	}
}}
