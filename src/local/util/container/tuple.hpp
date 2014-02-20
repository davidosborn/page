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

#ifndef    page_local_util_container_tuple_hpp
#   define page_local_util_container_tuple_hpp

#	include <functional> // function
#	include <limits> // numeric_limits
#	include <tuple>
#	include <utility> // declval

#	include "../type_traits/sfinae.hpp" // ENABLE_IF

namespace page { namespace util { namespace tuple
{
	/**
	 * @defgroup tuple
	 * @{
	 */

////////// slice ///////////////////////////////////////////////////////////////

	/**
	 * @defgroup tuple-slice Slice
	 * @{
	 */
	namespace detail
	{
		constexpr std::size_t ClampSliceIndex(int i, int size)
		{
			return i < 0 ?
				// HACK: std::{max,min} are not constexpr
				(size + i > 0 ? size + i : 0) :
				(i < size ? i : size);
		}

		template <std::size_t first, std::size_t last, typename Tuple, typename... Results>
			struct slice_impl_3
		{
			static constexpr std::size_t i = last - 1;
			typedef typename std::tuple_element<i, Tuple>::type Element;

			static auto Apply(Tuple &&t, Results &&... r) ->
				decltype(slice_impl_3<
					first,
					last - 1,
					Tuple,
					Element,
					Results...>::Apply(
						std::forward<Tuple>(t),
						static_cast<Element>(std::get<i>(std::forward<Tuple>(t))),
						std::forward<Results>(r)...))
			{
				return slice_impl_3<
					first,
					last - 1,
					Tuple,
					Element,
					Results...>::Apply(
						std::forward<Tuple>(t),
						static_cast<Element>(std::get<i>(std::forward<Tuple>(t))),
						std::forward<Results>(r)...);
			}
		};

		template <std::size_t i, typename Tuple, typename... Results>
			struct slice_impl_3<i, i, Tuple, Results...>
		{
			static auto Apply(Tuple &&t, Results... r) ->
				std::tuple<Results...>
			{
				return std::tuple<Results...>(r...);
			}
		};

		template <std::size_t first, std::size_t last, typename Tuple>
			auto slice_impl_2(Tuple &&t, ENABLE_IF(first < last)) ->
			decltype(slice_impl_3<first, last, Tuple>::Apply(std::forward<Tuple>(t)))
		{
			return slice_impl_3<first, last, Tuple>::Apply(std::forward<Tuple>(t));
		}

		template <std::size_t first, std::size_t last, typename Tuple>
			auto slice_impl_2(Tuple &&t, ENABLE_IF(first >= last)) ->
			std::tuple<>
		{
			return std::tuple<>();
		}

		template <int first, int last, typename Tuple>
			auto slice_impl(Tuple &&t) ->
			decltype(slice_impl_2<
				ClampSliceIndex(first, std::tuple_size<typename std::remove_reference<Tuple>::type>::value),
				ClampSliceIndex(last,  std::tuple_size<typename std::remove_reference<Tuple>::type>::value)>(std::forward<Tuple>(t)))
		{
			return slice_impl_2<
				ClampSliceIndex(first, std::tuple_size<typename std::remove_reference<Tuple>::type>::value),
				ClampSliceIndex(last,  std::tuple_size<typename std::remove_reference<Tuple>::type>::value)>(std::forward<Tuple>(t));
		}
	}

	/**
	 * Slices a @c std::tuple, creating a new @c std::tuple.
	 *
	 * @note This implementation has limited support for Python indexing, where
	 *       negative indices are counted back from the end.
	 */
	template <int first = 0, int last = std::numeric_limits<int>().max(), typename Tuple>
		auto slice(Tuple &&t) ->
		decltype(detail::slice_impl<first, last>(std::forward<Tuple>(t)))
	{
		return detail::slice_impl<first, last>(std::forward<Tuple>(t));
	}
	///@}

////////// back/front //////////////////////////////////////////////////////////

	/**
	 * @defgroup tuple-back-front Back & front
	 * @{
	 */
	/**
	 * Returns the last element in a @c std::tuple.
	 */
	template <typename Tuple>
		auto back(Tuple &&t) ->
		decltype(std::get<std::tuple_size<
			typename std::remove_reference<Tuple>::type
			>::value - 1>(std::forward<Tuple>(t)))
	{
		return std::get<std::tuple_size<
			typename std::remove_reference<Tuple>::type
			>::value - 1>(std::forward<Tuple>(t));
	}

	/**
	 * Returns the first element in a @c std::tuple.
	 */
	template <typename Tuple>
		auto front(Tuple &&t) ->
		decltype(std::get<0>(std::forward<Tuple>(t)))
	{
		return std::get<0>(std::forward<Tuple>(t));
	}

	/**
	 * Removes the last element from a @c std::tuple.
	 */
	template <typename Tuple>
		auto pop_back(Tuple &&t) ->
		decltype(slice<0, -1>(std::forward<Tuple>(t)))
	{
		return slice<0, -1>(std::forward<Tuple>(t));
	}

	/**
	 * Removes the first element from a @c std::tuple.
	 */
	template <typename Tuple>
		auto pop_front(Tuple &&t) ->
		decltype(slice<1>(std::forward<Tuple>(t)))
	{
		return slice<1>(std::forward<Tuple>(t));
	}
	///@}

////////// reverse /////////////////////////////////////////////////////////////

	/**
	 * @defgroup tuple-reverse Reverse
	 * @{
	 */
	namespace detail
	{
		template <std::size_t i, std::size_t n, typename Tuple, typename... Results>
			struct reverse_impl_2
		{
			typedef typename std::tuple_element<i, Tuple>::type Element;

			static auto Apply(Tuple &&t, Results &&... r) ->
				decltype(reverse_impl_2<
					i + 1,
					n,
					Tuple,
					Element,
					Results...>::Apply(
						std::forward<Tuple>(t),
						static_cast<Element>(std::get<i>(std::forward<Tuple>(t))),
						std::forward<Results>(r)...))
			{
				return reverse_impl_2<
					i + 1,
					n,
					Tuple,
					Element,
					Results...>::Apply(
						std::forward<Tuple>(t),
						static_cast<Element>(std::get<i>(std::forward<Tuple>(t))),
						std::forward<Results>(r)...);
			}
		};

		template <std::size_t n, typename Tuple, typename... Results>
			struct reverse_impl_2<n, n, Tuple, Results...>
		{
			static auto Apply(Tuple &&, Results &&... r) ->
				std::tuple<Results...>
			{
				return std::tuple<Results...>(std::forward<Results>(r)...);
			}
		};

		template <typename Tuple>
			auto reverse_impl(Tuple &&t) ->
			decltype(reverse_impl_2<0,
				std::tuple_size<typename std::remove_reference<Tuple>::type>::value,
				Tuple>::Apply(std::forward<Tuple>(t)))
		{
			return reverse_impl_2<0,
				std::tuple_size<typename std::remove_reference<Tuple>::type>::value,
				Tuple>::Apply(std::forward<Tuple>(t));
		}
	}

	/**
	 * Reverses a @c std::tuple.
	 */
	template <typename Tuple>
		auto reverse(Tuple &&t) ->
		decltype(detail::reverse_impl(std::forward<Tuple>(t)))
	{
		return detail::reverse_impl(std::forward<Tuple>(t));
	}
	///@}

////////// call ////////////////////////////////////////////////////////////////

	/**
	 * @defgroup tuple-call Call
	 * @{
	 */
	namespace detail
	{
		template <std::size_t j, typename Function, typename Tuple, typename... Args>
			struct call_impl_2
		{
			static constexpr std::size_t i = j - 1;
			typedef typename std::tuple_element<i, Tuple>::type Element;

			static auto Apply(const Function &f, Tuple &&t, Args &&... args) ->
				decltype(call_impl_2<
					j - 1,
					Function,
					Tuple,
					Element,
					Args...>::Apply(
						f,
						std::forward<Tuple>(t),
						static_cast<Element>(std::get<i>(std::forward<Tuple>(t))),
						std::forward<Args>(args)...))
			{
				return call_impl_2<
					j - 1,
					Function,
					Tuple,
					Element,
					Args...>::Apply(
						f,
						std::forward<Tuple>(t),
						static_cast<Element>(std::get<i>(std::forward<Tuple>(t))),
						std::forward<Args>(args)...);
			}
		};

		template <typename Function, typename Tuple, typename... Args>
			struct call_impl_2<0, Function, Tuple, Args...>
		{
			static auto Apply(const Function &f, Tuple &&, Args &&... args) ->
				decltype(f(std::forward<Args>(args)...))
			{
				return f(std::forward<Args>(args)...);
			}
		};

		template <typename Function, typename Tuple>
			auto call_impl(const Function &f, Tuple &&t) ->
			decltype(call_impl_2<
				std::tuple_size<typename std::remove_reference<Tuple>::type>::value,
				Function, Tuple>::Apply(f, std::forward<Tuple>(t)))
		{
			return call_impl_2<
				std::tuple_size<typename std::remove_reference<Tuple>::type>::value,
				Function, Tuple>::Apply(f, std::forward<Tuple>(t));
		}
	}

	/**
	 * Calls a function, passing the @c std::tuple values as arguments.
	 *
	 * @note When calling this function, consider using @c std::forward_as_tuple
	 *       to preserves rvalues.
	 */
	template <typename Function, typename Tuple>
		auto call(const Function &f, Tuple &&t) ->
		decltype(detail::call_impl(f, std::forward<Tuple>(t)))
	{
		return detail::call_impl(f, std::forward<Tuple>(t));
	}
	///@}

////////// transform ///////////////////////////////////////////////////////////

	/**
	 * @defgroup tuple-transform Transform
	 * @{
	 */
	namespace detail
	{
		// FIXME: implement
		/**
		 * I was going to implement this in order to implement zip_iterator, but
		 * I decided to just use boost::zip_iterator instead.  But what I was
		 * going to do was provide a transform function which transforms all the
		 * elements of a tuple by a function template, storing the results in
		 * another tuple.  Then I could write a function template adaptor which
		 * makes something like util::dereference (or any of the standard
		 * function ojbects) useable as a function for transform.  A different
		 * instantiation would be used depending on the type of the tuple
		 * element.  Something like...
		 *
		 * @code
		 * template <typename T>
		 * struct F
		 * {
		 * F() {}
		 * F(const T &f) : f(f) {}
		 * template <typename U>
		 * auto operator ()(U &&x) const
		 * {
		 * return f(x);
		 * }
		 * T f;
		 * };
		 * @endcode
		 *
		 * Then I could add other functions like tuple::all_of and tuple::any_of
		 * and basically anything in the <algorithm> library, and also support
		 * any of the function objects in <functional> via the adaptor.
		 */
	}

	/**
	 * Transforms the elements of a tuple.
	 */
	/*template <typename Function, typename Tuple>
		auto transform(Function f, Tuple &&t) ->
		decltype(detail::transform_impl(f, std::forward<Tuple>(t)))
	{
		return detail::transform_impl(f, std::forward<Tuple>(t));
	}*/

////////////////////////////////////////////////////////////////////////////////

	///@}
}}}

#endif
