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

#include "../err/Exception.hpp"

namespace page
{
	namespace util
	{
		// identification
		template <typename T> T *PointerFromId(Identifiable::Id id)
		{
			return dynamic_cast<T *>(Identifiable::FromId(id));
		}
		template <typename T> T &ReferenceFromId(Identifiable::Id id)
		{
			Identifiable *identifiable = Identifiable::FromId(id);
			if (!identifiable)
				THROW((err::Exception<err::UtilModuleTag, err::KeyTag>("invalid ID")))
			return dynamic_cast<T &>(*identifiable);
		}
	}
}
