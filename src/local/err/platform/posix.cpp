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

#include <cerrno>
#include <cstring> // strerror

// local
#include "Exception.hpp"

namespace page
{
	namespace err
	{
		namespace posix
		{
			void ResetError()
			{
				errno = 0;
			}

			void CheckError()
			{
				if (!errno) return;
				int e = errno;
				ResetError();
				const char *s = strerror(e);
				switch (e)
				{
					case E2BIG:
					case EINVAL:       THROW((err::Exception<err::ErrModuleTag, err::PosixPlatformTag, err::ParameterTag>(s)))
					case EACCES:       THROW((err::Exception<err::ErrModuleTag, err::PosixPlatformTag, err::FileAccessTag>(s)))
					case EBADF:
					case EEXIST:
					case EFBIG:
					case EIO:
					case EISDIR:
					case ELOOP:
					case EMFILE:
					case EMLINK:
					case ENFILE:
					case ENOSPC:
					case ENOTEMPTY:
					case EOVERFLOW:
					case EPIPE:
					case EROFS:
					case ESPIPE:
					case ETXTBSY:
					case EXDEV:        THROW((err::Exception<err::ErrModuleTag, err::PosixPlatformTag, err::FileTag>(s)))
					case EDOM:         THROW((err::Exception<err::ErrModuleTag, err::PosixPlatformTag, err::ParameterTag>(s)))
					case ENAMETOOLONG: THROW((err::Exception<err::ErrModuleTag, err::PosixPlatformTag, err::PathTag>(s)))
					case ENODEV:
					case ENOENT:       THROW((err::Exception<err::ErrModuleTag, err::PosixPlatformTag, err::FileNotFoundTag>(s)))
					case ENOTDIR:      THROW((err::Exception<err::ErrModuleTag, err::PosixPlatformTag, err::NotDirectoryTag>(s)))
					case ERANGE:       THROW((err::Exception<err::ErrModuleTag, err::PosixPlatformTag, err::RangeTag>(s)))
					default:           THROW((err::Exception<err::ErrModuleTag, err::PosixPlatformTag>(s)))
				}
			}
		}
	}
}
