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

#include <algorithm> // remove_if
#include <functional> // bind, logical_not
#include <iostream> // clog
#include <iterator> // back_inserter

#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm/sort.hpp>
#include <boost/range/algorithm/unique.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

#include "../../log/manip.hpp" // Warning
#include "../../util/functional/member.hpp" // make_member_of
#include "../../util/iterator/member_iterator.hpp"
#include "Model.hpp"
#include "Registry.hpp" // REGISTER_TYPE

namespace page { namespace res
{
	void PostLoadModel(Model &model)
	{
		Model::Parts::iterator iter(
			std::remove_if(model.parts.begin(), model.parts.end(),
				std::bind(
					std::logical_not<bool>(),
					std::bind(
						util::make_member_of(&Model::Part::mesh),
						std::placeholders::_1))));
		if (iter != model.parts.end())
		{
			model.parts.erase(iter, model.parts.end());
			std::clog << log::Warning << "contains invisible parts" << std::endl;
		}
	}

	util::reference_vector<cache::Proxy<res::Mesh>>
		GetGeometricallyDistinctMeshes(const Model &model)
	{
		// copy the mesh proxies for processing
		util::reference_vector<cache::Proxy<res::Mesh>> meshes;
		boost::copy(
			util::make_member_range(model.parts, &res::Model::Part::mesh),
			std::back_inserter(meshes));

		// uniquely sort meshes by source
		auto compareSource(cache::Proxy<res::Mesh>::CompareSource());
		boost::sort(meshes, compareSource);
		boost::erase(meshes, boost::unique<boost::return_found_end>(meshes, compareSource));
	}

	REGISTER_TYPE(Model, "model", PostLoadModel)
}}
