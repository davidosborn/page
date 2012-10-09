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

#ifndef    page_cache_proxy_Bounds_hpp
#   define page_cache_proxy_Bounds_hpp

#	include <vector>
#	include "../../util/copy_ptr.hpp"
#	include "../Proxy.hpp"

namespace page
{
	namespace phys { class Bounds; }
	namespace res
	{
		class Mesh;
		class Model;
		class Skeleton;
	}

	namespace cache
	{
		struct Bounds : Proxy<phys::Bounds>
		{
			using Proxy<phys::Bounds>::Instance;

			// construct
			explicit Bounds(const res::Model &, bool pose = true);
			explicit Bounds(const Proxy<res::Mesh> &);
			Bounds(
				const Proxy<res::Mesh> &,
				const Proxy<res::Skeleton> &);
			template <typename MeshIterator>
				Bounds(
					MeshIterator first,
					MeshIterator last);
			template <typename MeshIterator>
				Bounds(
					MeshIterator first,
					MeshIterator last,
					const Proxy<res::Skeleton> &);

			// clone
			Bounds *Clone() const;

			// attributes
			std::string GetType() const;
			std::string GetSource() const;

			// dependency satisfaction
			operator bool() const;

			private:
			// initialization
			void PostInit();

			// instantiation
			Instance Make() const;

			typedef std::vector<util::copy_ptr<Proxy<res::Mesh>>> Meshes;
			Meshes meshes;
			util::copy_ptr<Proxy<res::Skeleton>> skeleton;
		};
	}
}

#	include "Bounds.tpp"
#endif
