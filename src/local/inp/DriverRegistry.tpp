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

#include <functional> // bind
#include <utility> // forward

#include "../util/functional/cast.hpp" // dynamic_cast_function
#include "../util/functional/factory.hpp" // factory_function
#include "../util/functional/pointer.hpp" // address_of

namespace page { namespace inp
{
////////// DriverRegistry /////////////////////////////////////////////////////

	template <typename T, typename Window, typename... RecordArgs>
		void DriverRegistry::Register(RecordArgs &&... recordArgs)
	{
		Register(
			Record(
				typeid(T),
				std::bind(
					util::factory_function<T, Window &>(),
					std::bind(
						util::dynamic_cast_function<Window &, wnd::Window &>(),
						std::placeholders::_1)),
				std::bind(
					util::dynamic_cast_function<Window *, wnd::Window *>(),
					std::bind(
						util::address_of<wnd::Window>(),
						std::placeholders::_1)),
				std::forward<RecordArgs>(recordArgs)...));
	}
}}
