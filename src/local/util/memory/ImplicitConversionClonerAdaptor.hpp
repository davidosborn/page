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

#ifndef    page_local_util_memory_ImplicitConversionClonerAdaptor_hpp
#   define page_local_util_memory_ImplicitConversionClonerAdaptor_hpp

#	include <functional> // function
#	include <memory> // unique_ptr

#	include "../algorithm/downcast.hpp"

namespace page { namespace util
{
	/**
	 * An adaptor for cloners to support implicit pointer conversion, such as
	 * derived-to-base-pointer conversion.
	 */
	template <typename T, typename U>
		struct ImplicitConversionClonerAdaptor
	{
		typedef std::unique_ptr<T> result_type;
		typedef const T &argument_type;

		private:
		typedef std::function<std::unique_ptr<U> (const U &)> cloner_type;

		public:
		explicit ImplicitConversionClonerAdaptor(const cloner_type &cloner) :
			cloner(cloner) {}

		std::unique_ptr<T> operator ()(const T &x) const
		{
			return cloner(downcast<const U &>(x));
		}

		private:
		cloner_type cloner;
	};
}}

#endif
