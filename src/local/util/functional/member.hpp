#ifndef    page_local_util_functional_member_hpp
#   define page_local_util_functional_member_hpp

namespace page
{
	namespace util
	{
		/**
		 * @defgroup functional-member
		 *
		 * Function objects for interacting with members.
		 *
		 * @{
		 */
		/**
		 * A function object for extracting a member of an aggregate.
		 *
		 * @note @c std::mem_fn is probably a better choice if you need to
		 *       access a member function rather than a data member.
		 *       @c std::mem_fn takes a pointer-to-member-function (ie: &T::F)
		 *       and returns a function object that calls the member.
		 */
		template <typename T, typename M>
			struct member_of
		{
			explicit member_of(M T::*m) : m(m) {}

			M &operator ()(T &t) const
			{
				return t.*m;
			}
			const M &operator ()(const T &t) const
			{
				return t.*m;
			}

			private:
			M T::*m;
		};

		/**
		 * A convenience function for creating an instance of @c member_of.
		 */
		template <typename T, typename M>
			member_of<T, M> make_member_of(M T::*m)
		{
			return member_of<T, M>(m);
		}
		///@}
	}
}

#endif
