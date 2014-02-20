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

#include "class/Monostate.hpp"
#include "Identifiable.hpp"

namespace page { namespace util
{
	namespace
	{
		/**
		 * Maps IDs to @c Identifiable instances.
		 */
		class Map : public Monostate<Map>
		{
			public:
			/**
			 * Adds a new instance to the map, returning its unique ID.
			 */
			Identifiable::Id GetId(Instance &instance)
			{
				if (!availableIds.empty())
				{
					Identifiable::Id id(availableIds.back());
					availableIds.pop_back();
					assert(instances[id - 1] == nullptr);
					instances[id - 1] = &instance;
					return id;
				}
				else
				{
					instances.push_back(&instance);
					return instances.size();
				}
			}

			/**
			 * Returns the instance associated with the specified ID.
			 */
			Identifiable *GetInstance(Identifiable::Id id) const
			{
				return id > 0 && --id < instances.size() ?
					instances[id] : nullptr;
			}

			/**
			 * Releases an ID and its associated instance.  The ID will be
			 * available for reuse by new instances.
			 */
			void Release(Identifiable::Id id)
			{
				assert(instances[id - 1] != nullptr);
				instances[id - 1] = nullptr;
				availableIds.push_back(id);
			}

			/*-------------+
			| data members |
			+-------------*/

			private:
			/**
			 * An array of instances.  The ID of an instance is defined by its
			 * index in this array plus one.  Null pointers refer to available
			 * IDs, which should also be in @c availableIds.
			 */
			std::vector<Identifiable *> instances;

			/**
			 * An array of the available IDs in @c instances.  An ID becomes
			 * available when its associated instance is deleted.
			 */
			std::vector<Identifiable::Id> availableIds;
		};
	}

////////// Identifiable ////////////////////////////////////////////////////////

	/*-------------+
	| constructors |
	+-------------*/

	Identifiable::Identifiable() :
		id(GLOBAL(Map).GetId()) {}

	Identifiable::Identifiable(const Identifiable &other) :
		Identifiable() {}

	Identifiable::~Identifiable()
	{
		GLOBAL(Map).Release(id);
	}

	Identifiable &Identifiable::operator =(const Identifiable &)
	{
		return *this;
	}

	/*---------------+
	| identification |
	+---------------*/

	Identifiable::Id Identifiable::GetId() const
	{
		return id;
	}

	Identifiable *Identifiable::GetInstance(Id id)
	{
		return GLOBAL(Map).GetInstance(id);
	}
}}
