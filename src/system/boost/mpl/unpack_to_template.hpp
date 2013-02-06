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

#ifndef    page_system_boost_mpl_unpack_to_template_hpp
#	define page_system_boost_mpl_unpack_to_template_hpp

#	include <boost/mpl/back.hpp>
#	include <boost/mpl/empty.hpp>
#	include <boost/mpl/pop_back.hpp>

namespace boost
{
	namespace mpl
	{
		/**
		 * Instantiates a class template, unpacking a sequence into the
		 * template arguments.
		 *
		 * @note This class has been structured like a
		 *       <em>metafunction class</em> to maximize its usability.
		 */
		template <template <typename...> class Template>
			struct unpack_to_template
		{
			template <
				typename Sequence,
				bool = empty<Sequence>::value,
				typename... Args>
					struct apply
			{
				typedef
					typename apply<
						typename pop_back<Sequence>::type,
						empty<typename pop_back<Sequence>::type>::value,
						typename back<Sequence>::type,
						Args...>::type type;
			};
			
			template <typename Sequence, typename... Args>
				struct apply<Sequence, true, Args...>
			{
				typedef Template<Args...> type;
			};
		};
	}
}

#endif
