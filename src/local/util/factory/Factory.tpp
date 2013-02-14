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

// C++
#include <algorithm> // upper_bound
#include <functional> // greater

// local
#include "../../err/Exception.hpp"

namespace page
{
	namespace util
	{
		/*----------------+
		| global instance |
		+----------------*/

		template <typename AbstractProduct, typename ConstructorArgs, typename Criteria, typename Data>
			Factory<AbstractProduct, ConstructorArgs, Criteria, Data> &
			Factory<AbstractProduct, ConstructorArgs, Criteria, Data>::GetGlobalInstance()
		{
			static Factory instance;
			return instance;
		}

		/*-------------+
		| registration |
		+-------------*/

		template <typename AbstractProduct, typename ConstructorArgs, typename Criteria, typename Data>
			void Factory<AbstractProduct, ConstructorArgs, Criteria, Data>::Register(
				const Function &function, int priority, const Criteria &criteria, const Data &data)
		{
			Blueprint blueprint(function, priority, criteria, data);

			auto iter(
				blueprints.insert(
					std::upper_bound(
						blueprints.begin(),
						blueprints.end(),
						blueprint,
						std::greater<Blueprint>()),
					blueprint));
		}

		template <typename AbstractProduct, typename ConstructorArgs, typename Criteria, typename Data>
			template <typename ConcreteProduct>
				void Factory<AbstractProduct, ConstructorArgs, Criteria, Data>::Register(
					int priority, const Criteria &criteria, const Data &data)
		{
			Register(
				FunctionImplementation<ConcreteProduct>(),
				priority, criteria, data);
		}

		/*-----------+
		| production |
		+-----------*/

		template <typename AbstractProduct, typename ConstructorArgs, typename Criteria, typename Data>
			template <typename T>
				indirect_vector<const typename Factory<AbstractProduct, ConstructorArgs, Criteria, Data>::Blueprint>
				Factory<AbstractProduct, ConstructorArgs, Criteria, Data>::Select(
					const Criterion<T, Criteria> &criterion) const
		{
			indirect_vector<const Blueprint> r;
			for (const auto &blueprint : blueprints)
				if (criterion(blueprint.criteria))
					r.push_back(blueprint);
			return r;
		}

		template <typename AbstractProduct, typename ConstructorArgs, typename Criteria, typename Data>
			template <typename T>
				const typename Factory<AbstractProduct, ConstructorArgs, Criteria, Data>::Blueprint &
				Factory<AbstractProduct, ConstructorArgs, Criteria, Data>::SelectBest(
					const Criterion<T, Criteria> &criterion) const
		{
			for (const auto &blueprint : blueprints)
				if (criterion(blueprint.criteria))
					return blueprint;

			THROW((err::Exception<err::UtilModuleTag, err::FactoryTag>("factory failed to select product")))
		}
	}
}
