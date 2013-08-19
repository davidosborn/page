#ifndef    page_local_res_adapt_flac_hpp
#   define page_local_res_adapt_flac_hpp

#	include <functional> // function
#	include <memory> // unique_ptr

#	include <FLAC/stream_decoder.h> // FLAC__{Frame,int32,StreamDecoder{,State}}

namespace page
{
	namespace res
	{
		class Pipe;

		namespace flac
		{
			// error handling
			void CheckError(FLAC__StreamDecoderState);
			void CheckError(const FLAC__StreamDecoder *);

			// streaming
			bool Check(const Pipe &);
			typedef std::function<void (const FLAC__StreamDecoder *, const FLAC__Frame *, const FLAC__int32 *const buffers[])> WriteCallback;
			typedef std::unique_ptr<FLAC__StreamDecoder> File;
			File Open(const Pipe &, const WriteCallback &);
		}

		using namespace flac;
	}
}

#endif
