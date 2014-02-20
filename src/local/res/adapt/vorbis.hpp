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

#ifndef    page_local_res_adapt_vorbis_hpp
#   define page_local_res_adapt_vorbis_hpp

#	include <functional> // function
#	include <memory> // unique_ptr

#	include <vorbis/vorbisfile.h> // OggVorbis_File

namespace page
{
	namespace res
	{
		class Pipe;

		namespace vorbis
		{
			/*---------------+
			| error handling |
			+---------------*/

			/**
			 * @throw err::Exception<err::VorbisPlatformTag>::Permutation If the
			 *        error code represents an error.
			 */
			void CheckError(int);

			/*----------+
			| streaming |
			+----------*/

			/**
			 * @return @c true if the pipe refers to Vorbis data.
			 */
			bool Check(const Pipe &);

			/**
			 * An instance of @c OggVorbis_File wrapped in a smart pointer with
			 * a custom deleter.
			 */
			typedef std::unique_ptr<OggVorbis_File, std::function<void (const OggVorbis_File *)>> File;

			/**
			 * @return An instance of @c OggVorbis_File providing access to the
			 *         Vorbis data referred to by the pipe.
			 */
			File Open(const Pipe &);
		}
	}
}

#endif
