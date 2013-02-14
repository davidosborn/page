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

#	include <unordered_set>

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

		/*class AndCriterion :
			public Criterion
		{
			public:
			AndCriterion(const Criterion &first, const Criterion &second) :
				first(first), second(second) {}

			public:
			bool Apply(const Criteria &criteria) const override
			{
				return first.Apply(criteria) && second.Apply(criteria);
			}

			private:
			std::unique_ptr<Criterion> first, second;
		};

		class OrCriterion :
			public Criterion
		{
			public:
			OrCriterion(Criterion &&first, Criterion &&second) :
				first(first), second(second) {}

			public:
			bool Apply(const Criteria &criteria) const override
			{
				return first.Apply(criteria) || second.Apply(criteria);
			}

			private:
			Criterion first, second;
		};

		AndCriterion operator &&(Criterion &&a, Criterion &&b)
		{
			return AndCriterion(
				std::forward<Criterion>(a),
				std::forward<Criterion>(b));
		}

		OrCriterion operator ||(Criterion &&a, Criterion &&b)
		{
			return OrCriterion(
				std::forward<Criterion>(a),
				std::forward<Criterion>(b));
		}*/
	}
}

#endif
