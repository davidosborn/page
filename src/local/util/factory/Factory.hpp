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

#ifndef    page_local_util_factory_Factory_hpp
#   define page_local_util_factory_Factory_hpp

	// C++
#	include <functional> // function
#	include <list>
#	include <memory> // unique_ptr
#	include <vector>

	// Boost
#	include <boost/mpl/push_front.hpp>
#	include <boost/mpl/unpack.hpp>
#	include <boost/mpl/vector.hpp>

	// local
#	include "../container/reference_vector.hpp"
#	include "../functional/factory.hpp" // factory_function
#	include "Criterion.hpp"

namespace page
{
	namespace util
	{
		/**
		 *
		 */
		template <typename... T>
			struct ConstructorArgs
		{
			typedef boost::mpl::vector<T...> type;
		};

		/**
		 * A factory for producing concrete instances of an abstract type, where
		 * the concrete type is selected from a pool of registered types
		 * according to a standard criteria.
		 *
		 * @note Based in part on "Conversations: Abstract Factory, Template
		 *       Style" by Jim Hyslop and Herb Sutter.
		 */
		template <
			typename AbstractProduct,
			typename ConstructorArgs,
			typename Criteria_,
			typename Data_ = void>
				class Factory
		{
			/*------+
			| types |
			+------*/

			private:
			/**
			 * An implementation of @c Function, parameterized by the product
			 * type (which must be derived from @c AbstractProduct).
			 */
			template <typename Product>
				using FunctionImplementation =
					typename boost::mpl::unpack<factory_function>::apply<
						typename boost::mpl::push_front<
							typename ConstructorArgs::type,
							Product
							>::type
						>::type;

			public:
			/**
			 * The type of the factory function used to create a products.
			 */
			typedef std::function<
				typename FunctionImplementation<AbstractProduct>::signature
				> Function;

			/**
			 * A structure representing the criteria that will be considered
			 * when selecting a concrete product during production.
			 */
			typedef Criteria_ Criteria;

			/**
			 * A structure containing additional data, which is associated with
			 * the product at registration and returned with the product during
			 * production.
			 */
			typedef Data_ Data;

			/*----------------+
			| global instance |
			+----------------*/

			public:
			/**
			 * Provides global access to a single instance of the class.
			 */
			static Factory &GetGlobalInstance();

			/*-------------+
			| registration |
			+-------------*/

			public:
			/**
			 * Registers a concrete product.
			 */
			void Register(
				const Function &,
				int priority,
				const Criteria & = Criteria(),
				const Data     & = Data());

			/**
			 * Registers a concrete product, generating a default factory
			 * function for the type.
			 */
			template <typename ConcreteProduct>
				void Register(
					int priority,
					const Criteria & = Criteria(),
					const Data     & = Data());

			/*----------+
			| blueprint |
			+----------*/

			public:
			/**
			 * A structure containing information about a registered product.
			 */
			struct Blueprint
			{
				/**
				 * Constructor, to safely initialize @c Blueprint.
				 */
				Blueprint(const Function &function, int priority, const Criteria &criteria, const Data &data) :
					function(function), priority(priority), criteria(criteria), data(data) {}

				/**
				 * @defgroup factory-priority
				 *
				 * Relational operators to support the ordering of factory
				 * products by priority.
				 *
				 * @{
				 */
				bool operator ==(const Blueprint &other) const { return priority == other.priority; }
				bool operator !=(const Blueprint &other) const { return priority != other.priority; }
				bool operator < (const Blueprint &other) const { return priority <  other.priority; }
				bool operator > (const Blueprint &other) const { return priority >  other.priority; }
				bool operator <=(const Blueprint &other) const { return priority <= other.priority; }
				bool operator >=(const Blueprint &other) const { return priority >= other.priority; }
				///@}

				/**
				 * A factory function which returns a new instance of the
				 * product.
				 */
				Function function;

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

			/*-----------+
			| production |
			+-----------*/

			public:
			/**
			 *
			 */
			template <typename T>
				reference_vector<const Blueprint>
					Select(const Criterion<T, Criteria> &) const;

			/**
			 *
			 */
			template <typename T>
				const Blueprint &
					SelectBest(const Criterion<T, Criteria> &) const;

			/*-------------+
			| data members |
			+-------------*/

			private:
			/**
			 * A list of registered products, ordered by descending priority.
			 */
			std::list<Blueprint> blueprints;
		};
	}
}

#	include "Factory.tpp"
#endif
