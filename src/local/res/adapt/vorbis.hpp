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
