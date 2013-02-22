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

#ifndef    page_local_cache_proxy_Bounds_hpp
#   define page_local_cache_proxy_Bounds_hpp

#	include <vector>

#	include "../../util/raii/copy_ptr.hpp"
#	include "../../util/type_traits/container.hpp" // is_range
#	include "../../util/type_traits/iterator.hpp" // is_iterator
#	include "../../util/type_traits/sfinae.hpp" // ENABLE_IF
#	include "Proxy.hpp"

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
		/**
		 * A proxy representing a bounding skeleton in the cache.
		 */
		class Bounds : public Proxy<phys::Bounds>
		{
			/*------+
			| types |
			+------*/

			public:
			typedef typename Proxy<phys::Bounds>::Instance Instance;
		
			/*--------------------------+
			| constructors & destructor |
			+--------------------------*/

			public:
			/**
			 * Creates a proxy for a bounding skeleton around an optionally-
			 * posed model.
			 */
			explicit Bounds(const res::Model &, bool pose = true);

			/**
			 * Creates a proxy for a bounding skeleton around a mesh.
			 */
			explicit Bounds(const Proxy<res::Mesh> &);

			/**
			 * Creates a proxy for a bounding skeleton around a mesh posed to a
			 * skeleton.
			 */
			Bounds(
				const Proxy<res::Mesh> &,
				const Proxy<res::Skeleton> &);

			/**
			 * Creates a proxy for a bounding skeleton around a range of meshes.
			 */
			template <typename MeshInputRange>
				explicit Bounds(
					MeshInputRange,
					ENABLE_IF((util::is_range<MeshInputRange>::value)));

			/**
			 * Creates a proxy for a bounding skeleton around a range of meshes.
			 */
			template <typename MeshInputIterator>
				Bounds(
					MeshInputIterator first,
					MeshInputIterator last,
					ENABLE_IF((util::is_iterator<MeshInputIterator>::value)));

			/**
			 * Creates a proxy for a bounding skeleton around a range of meshes
			 * posed to a skeleton.
			 */
			template <typename MeshInputRange>
				Bounds(
					MeshInputRange,
					const Proxy<res::Skeleton> &,
					ENABLE_IF((util::is_range<MeshInputRange>::value)));

			/**
			 * Creates a proxy for a bounding skeleton around a range of meshes
			 * posed to a skeleton.
			 */
			template <typename MeshInputIterator>
				Bounds(
					MeshInputIterator first,
					MeshInputIterator last,
					const Proxy<res::Skeleton> &,
					ENABLE_IF((util::is_iterator<MeshInputIterator>::value)));

			/*------+
			| clone |
			+------*/

			public:
			Bounds *Clone() const override;

			/*----------+
			| observers |
			+----------*/

			public:
			std::string GetType() const override;
			std::string GetSource() const override;
			operator bool() const override;

			/*--------------+
			| instantiation |
			+--------------*/

			public:
			Instance Make() const override;

			/*---------------+
			| initialization |
			+---------------*/

			private:
			void PostInit();

			/*-------------+
			| data members |
			+-------------*/

			private:
			std::vector<util::copy_ptr<Proxy<res::Mesh>>> meshes;
			util::copy_ptr<Proxy<res::Skeleton>> skeleton;
		};
	}
}

#	include "Bounds.tpp"
#endif
