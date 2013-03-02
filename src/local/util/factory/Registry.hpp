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

#ifndef    page_local_util_factory_Registry_hpp
#   define page_local_util_factory_Registry_hpp

#	include <functional> // function
#	include <list>
#	include <memory> // unique_ptr
#	include <type_traits> // is_void

#	include <boost/mpl/if.hpp>
#	include <boost/mpl/push_front.hpp>
#	include <boost/mpl/unpack.hpp>
#	include <boost/mpl/vector.hpp>

#	include "../class/Monostate.hpp"
#	include "../container/reference_vector.hpp"
#	include "../functional/factory.hpp" // factory_function
#	include "Criterion.hpp"

namespace page
{
	namespace util
	{
		/**
		 * A variadic template used to define the keys that will be used by an
		 * instantiation of @c Factory.
		 */
		template <typename Base, typename... Keys>
			class KeyedRegistry : public Base
		{
			public:
			struct Record : Base::Record
			{
			};

			public:
			template <typename... Args>
				Record &Register(const Keys &keys..., Args &&...)
			{

			}

			private:
			template <int i>
				void RegisterKey(const Keys &keys..., const Record &record)
			{
				std::get<i>(this->keys).insert(std::get<i>(keys), &record);
			}

			private:
			std::tuple<std::unordered_map<Keys, Record *>...> keys;
		};

		template <typename Base>
			class PrioritizedRegistry : public Base
		{
			public:
			struct Record : Base::Record
			{
				/**
				 * The record's priority, where a record with a higher priority
				 * is selected first.  By convention, it is in the range of
				 * [0, 100].
				 */
				int priority;
			};

			public:
			template <typename... Args>
				Record &Register(int priority, Args &&...)
			{
				auto record(Base::Register(std::forward<Args>(args)...));
				record.priority = priority;
			}

			private:
			std::unordered_map<
		};

		struct CriteriaMixin
		{
		};

		class TypeRegistry : public KeyedRegistry<PrioritizedRegistry<Registry>>
		{
		};

		/**
		 *
		 */
		template <typename Criteria, typename Data>
			struct Record
		{
			/**
			 *
			 */
			Record(int priority, const Criteria &criteria, const Data &data) :
				function(function), priority(priority), criteria(criteria), data(data) {}

			/**
			 * The product's priority, where the higher priority products
			 * are selected first.  By convention, it is in the range of
			 * [0, 100].
			 *
			 * @note When a new product is registered, it is placed ahead of
			 *       any previously registered products of the same
			 *       priority.
			 */
			int priority;

			/**
			 * The criteria associated with this product, which are
			 * considered when selecting a product during production.
			 */
			Criteria criteria;

			/**
			 *
			 */
			Data data;
		};

		/**
		 *
		 */
		template <typename... Policies>
			class Registry :
				public Monostate<Registry<Policies...>>
		{
			/*------+
			| types |
			+------*/

			public:
			/**
			 * A structure representing the criteria that will be considered
			 * when selecting a concrete product during production.
			 */
			typedef Key_ Key;

			/**
			 * A structure containing additional data, which is associated with
			 * the product at registration and returned with the product during
			 * production.
			 */
			typedef Data_ Data;

			/*-------------+
			| registration |
			+-------------*/

			public:
			/**
			 * Adds a record to the registry.
			 */
			void Register(
				const Key  & = Key(),
				const Data & = Data());

			/*-----------+
			| production |
			+-----------*/

			public:
			/**
			 * Returns an ordered sequence of records that match the specified
			 * criteria.
			 */
			template <typename T = TrueCriterion<Criteria>>
				Selection Select(const Criterion<T, Criteria> & = T()) const;

			/**
			 * Returns the record with the highest priority that matches the
			 * specified criteria.
			 */
			template <typename T = TrueCriterion<Criteria>>
				const Record &SelectBest(const Criterion<T, Criteria> & = T()) const;

			/*-------------+
			| data members |
			+-------------*/

			private:
			/**
			 * A list of records, ordered by descending priority.
			 */
			std::list<Record> records;
		};
	}
}

#	include "Factory.tpp"
#endif
