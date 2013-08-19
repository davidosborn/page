#include <cerrno>
#include <cstring> // strerror

#include "../Exception.hpp"

namespace page { namespace err { namespace posix
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
}}}
