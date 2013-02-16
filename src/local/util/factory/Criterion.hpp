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

#ifndef    page_local_util_factory_Criterion_hpp
#   define page_local_util_factory_Criterion_hpp

	// C++
#	include <unordered_set>

	// local
#	include "../algorithm/contains.hpp"

namespace page
{
	namespace util
	{
		/**
		 * The base class for a criterion.
		 */
		template <typename Derived, typename Criteria>
			class Criterion
		{
			public:
			bool operator ()(const Criteria &criteria) const
			{
				return static_cast<const Derived &>(*this)(criteria);
			}
		};

		/**
		 * A criterion which always evaluates to @c true.
		 */
		template <typename Criteria>
			class TrueCriterion :
				public Criterion<TrueCriterion<Criteria>, Criteria>
		{
			public:
			bool operator ()(const Criteria &criteria) const
			{
				return true;
			}
		};

		/**
		 * A criterion which evaluates to @c true if the specified criteria
		 * member is equal to the specified value.
		 */
		template <
			typename T,
			typename Criteria,
			T Criteria::*member>
				class MemberEqualsCriterion final :
					public Criterion<MemberEqualsCriterion<T, Criteria, member>, Criteria>
		{
			public:
			explicit MemberEqualsCriterion(const T &value) :
				value(value) {}

			public:
			bool operator ()(const Criteria &criteria) const
			{
				return criteria.*member == value;
			}

			private:
			T value;
		};

		/**
		 * A criterion which evaluates to @c true if the specified criteria
		 * member contains the specified value.
		 */
		template <
			typename T,
			typename Criteria,
			T Criteria::*member>
				class MemberContainsCriterion final :
					public Criterion<MemberContainsCriterion<T, Criteria, member>, Criteria>
		{
			public:
			explicit MemberContainsCriterion(const typename T::value_type &value) :
				value(value) {}

			public:
			bool operator ()(const Criteria &criteria) const
			{
				return contains(criteria.*member, value);
			}

			private:
			typename T::value_type value;
		};

		/**
		 * A criterion which evaluates to @c true if both of the members also
		 * evaluate to @c true.
		 */
		template <typename T, typename U, typename Criteria>
			class AndCriterion :
				public Criterion<AndCriterion<T, U, Criteria>, Criteria>
		{
			public:
			AndCriterion(
				const Criterion<T, Criteria> &a,
				const Criterion<U, Criteria> &b) :
					a(a), b(b) {}

			AndCriterion(
				Criterion<T, Criteria> &&a,
				Criterion<U, Criteria> &&b) :
					a(std::move(a)),
					b(std::move(b)) {}

			public:
			bool operator ()(const Criteria &criteria) const
			{
				return a(criteria) && b(criteria);
			}

			private:
			Criterion<T, Criteria> a;
			Criterion<U, Criteria> b;
		};

		/**
		 * A criterion which evaluates to @c true if one of the members also
		 * evaluate to @c true.
		 */
		template <typename T, typename U, typename Criteria>
			class OrCriterion :
				public Criterion<OrCriterion<T, U, Criteria>, Criteria>
		{
			public:
			OrCriterion(
				const Criterion<T, Criteria> &a,
				const Criterion<U, Criteria> &b) :
					a(a), b(b) {}

			OrCriterion(
				Criterion<T, Criteria> &&a,
				Criterion<U, Criteria> &&b) :
					a(std::move(a)),
					b(std::move(b)) {}

			public:
			bool operator ()(const Criteria &criteria) const
			{
				return a(criteria) || b(criteria);
			}

			private:
			Criterion<T, Criteria> a;
			Criterion<U, Criteria> b;
		};

		/**
		 * A short form of @c AndCriterion.
		 */
		template <typename T, typename U, typename Criteria>
			AndCriterion<T, U, Criteria> operator &&(
				Criterion<T, Criteria> &&a,
				Criterion<U, Criteria> &&b)
		{
			return AndCriterion<T, U, Criteria>(
				std::forward<Criterion<T, Criteria>>(a),
				std::forward<Criterion<U, Criteria>>(b));
		}

		/**
		 * A short form of @c OrCriterion.
		 */
		template <typename T, typename U, typename Criteria>
			OrCriterion<T, U, Criteria> operator ||(
				Criterion<T, Criteria> &&a,
				Criterion<U, Criteria> &&b)
		{
			return OrCriterion<T, U, Criteria>(
				std::forward<Criterion<T, Criteria>>(a),
				std::forward<Criterion<U, Criteria>>(b));
		}
	}
}

#endif
