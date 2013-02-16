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

	// Boost
#	include <boost/mpl/push_front.hpp>
#	include <boost/mpl/unpack.hpp>
#	include <boost/mpl/vector.hpp>

	// local
#	include "../class/Monostate.hpp"
#	include "../container/reference_vector.hpp"
#	include "../functional/factory.hpp" // factory_function
#	include "Criterion.hpp"

namespace page
{
	namespace util
	{
////////// ConstructorArgs /////////////////////////////////////////////////////

		/**
		 * A variadic template used to pass constructor arguments to @c Factory.
		 */
		template <typename... T>
			struct ConstructorArgs
		{
			typedef boost::mpl::vector<T...> type;
		};

////////// detail::Blueprint ///////////////////////////////////////////////////

		namespace detail
		{
			/**
			 * A structure containing information about a registered product.
			 */
			template <typename Function, typename Criteria, typename Data>
				struct Blueprint
			{
				/**
				 * Constructor, to safely initialize @c Blueprint.
				 */
				Blueprint(const Function &function, int priority, const Criteria &criteria, const Data &data) :
					function(function), priority(priority), criteria(criteria), data(data) {}

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

////////// detail::Selection ///////////////////////////////////////////////////

			/**
			 * An ordered container of blueprint references, used to transport
			 * the results of @c Factory::Select.
			 */
			template <typename Function, typename Criteria, typename Data>
				using Selection = reference_vector<const Blueprint<Function, Criteria, Data>>;
				
			/**
			 * Appends one @c Selection to another, removing any duplicate
			 * blueprints while maintaining the order.
			 */
			template <typename Function, typename Criteria, typename Data>
				void Merge(
					Selection<Function, Criteria, Data> &,
					const Selection<Function, Criteria, Data> &);
		}

////////// Factory /////////////////////////////////////////////////////////////

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
				class Factory :
					public Monostate<Factory<AbstractProduct, ConstructorArgs, Criteria_, Data_>>
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

			/*-----------+
			| production |
			+-----------*/

			public:
			typedef detail::Blueprint<Function, Criteria, Data> Blueprint;
			typedef detail::Selection<Function, Criteria, Data> Selection;

			/**
			 * Returns an ordered sequence of blueprints that match the
			 * specified criteria.
			 */
			template <typename T>
				Selection Select(const Criterion<T, Criteria> & = TrueCriterion<Criteria>()) const;

			/**
			 * Returns the blueprint with the highest priority that matches the
			 * specified criteria.
			 */
			template <typename T>
				const Blueprint &SelectBest(const Criterion<T, Criteria> & = TrueCriterion<Criteria>()) const;

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
