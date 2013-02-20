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

#include <algorithm> // upper_bound
#include <functional> // bind, greater

#include "../../err/Exception.hpp"
#include "../functional/member.hpp" // member_of
#include "../functional/pointer.hpp" // address_of

namespace page
{
	namespace util
	{
////////// detail::Selection ///////////////////////////////////////////////////

		namespace detail
		{
			template <typename Function, typename Criteria, typename Data>
				void Merge(
					Selection<Function, Criteria, Data> &a,
					const Selection<Function, Criteria, Data> &b)
			{
				a.reserve(a.size() + b.size());
				for (const auto &blueprint : b)
				{
					if (std::find_if(a.begin(), a.end(),
						std::bind(
							std::equal_to<const Blueprint<Function, Criteria, Data> *>(),
							std::bind(
								address_of<const Blueprint<Function, Criteria, Data>>(),
								std::placeholders::_1),
							&blueprint)) == a.end())
						a.push_back(blueprint);
				}
			}
		}

////////// Factory /////////////////////////////////////////////////////////////

		/*-------------+
		| registration |
		+-------------*/

		template <typename AbstractProduct, typename ConstructorArgs, typename Criteria, typename Data, typename Derived>
			void Factory<AbstractProduct, ConstructorArgs, Criteria, Data, Derived>::Register(
				const Function &function, int priority, const Criteria &criteria, const Data &data)
		{
			Blueprint blueprint(function, priority, criteria, data);

			auto iter(
				blueprints.insert(
					std::upper_bound(
						blueprints.begin(),
						blueprints.end(),
						blueprint,
						std::bind(
							std::greater<int>(),
							std::bind(
								make_member_of(&Blueprint::priority),
								std::placeholders::_1),
							std::bind(
								make_member_of(&Blueprint::priority),
								std::placeholders::_2))),
					blueprint));
		}

		template <typename AbstractProduct, typename ConstructorArgs, typename Criteria, typename Data, typename Derived>
			template <typename ConcreteProduct>
				void Factory<AbstractProduct, ConstructorArgs, Criteria, Data, Derived>::Register(
					int priority, const Criteria &criteria, const Data &data)
		{
			Register(
				FunctionImplementation<ConcreteProduct>(),
				priority, criteria, data);
		}

		/*-----------+
		| production |
		+-----------*/

		template <typename AbstractProduct, typename ConstructorArgs, typename Criteria, typename Data, typename Derived>
			template <typename T>
				typename Factory<AbstractProduct, ConstructorArgs, Criteria, Data, Derived>::Selection
				Factory<AbstractProduct, ConstructorArgs, Criteria, Data, Derived>::Select(
					const Criterion<T, Criteria> &criterion) const
		{
			Selection r;
			for (const auto &blueprint : blueprints)
				if (criterion(blueprint.criteria))
					r.push_back(blueprint);
			return r;
		}

		template <typename AbstractProduct, typename ConstructorArgs, typename Criteria, typename Data, typename Derived>
			template <typename T>
				const typename Factory<AbstractProduct, ConstructorArgs, Criteria, Data, Derived>::Blueprint &
				Factory<AbstractProduct, ConstructorArgs, Criteria, Data, Derived>::SelectBest(
					const Criterion<T, Criteria> &criterion) const
		{
			for (const auto &blueprint : blueprints)
				if (criterion(blueprint.criteria))
					return blueprint;

			THROW((err::Exception<err::UtilModuleTag, err::FactoryTag>("factory failed to select product")))
		}
	}
}
