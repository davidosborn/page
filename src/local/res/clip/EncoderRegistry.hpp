/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
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

#ifndef    page_local_res_clip_EncoderRegistry_hpp
#   define page_local_res_clip_EncoderRegistry_hpp

#	include <forward_list>
#	include <functional> // function
#	include <memory> // unique_ptr
#	include <string>
#	include <unordered_map>
#	include <unordered_set>

#	include "../../math/fwd.hpp"
#	include "../../util/class/Monostate.hpp"
#	include "../../util/gcc/init_priority.hpp" // REG_INIT_PRIORITY

namespace page { namespace res { namespace clip
{
	class Encoder;

	/**
	 *
	 */
	typedef std::function<void (const void *, unsigned)> EncoderCallback;

	/**
	 * A pointer to a factory function that, when called, will return a new
	 * @c Encoder object, constructed with the given arguments.
	 */
	using EncoderFactoryFunction = std::function<
		std::unique_ptr<Encoder> (
			EncoderCallback const& callback,
			math::Vec2u     const& size,
			float                  frameRate,
			float                  quality)>;

////////// EncoderRegistryRecord ///////////////////////////////////////////////

	/**
	 * A data structure containing information about an encoder.
	 */
	struct EncoderRegistryRecord
	{
		/**
		 * Constructor.
		 *
		 * @note The first extension in @c extensions will become the value of
		 *       @c defaultExtension.
		 */
		EncoderRegistryRecord(
			std::string              const& name,
			EncoderFactoryFunction   const& factoryFunction,
			std::vector<std::string> const& formats          = {},
			std::vector<std::string> const& extensions       = {},
			bool                            binary           = true,
			int                             priority         = 0);

		/**
		 * The name of the encoder.
		 */
		std::string name;

		/**
		 * @copydoc EncoderFactoryFunction
		 */
		EncoderFactoryFunction factoryFunction;

		/**
		 * The formats that this encoder recognizes.
		 */
		std::unordered_set<std::string> formats;

		/**
		 * The extensions that this encoder recognizes.
		 */
		std::unordered_set<std::string> extensions;

		/**
		 * The default extension to append to a path when it doesn't already
		 * have a matching extension.
		 */
		std::string defaultExtension;

		/**
		 * @c true if the file should be opened in binary mode, as opposed to
		 * text mode.
		 */
		bool binary;

		/**
		 * A higher priority means that this encoder will be selected before
		 * other encoders of the same level with a lower priority.
		 */
		int priority;
	};

////////// EncoderRegistry /////////////////////////////////////////////////////

	/**
	 * A place for registering encoders.
	 */
	class EncoderRegistry : public util::Monostate<EncoderRegistry>
	{
		/**
		 * Short-form identifier in context.
		 */
		typedef EncoderRegistryRecord Record;

		public:
		/**
		 * Registers a encoder.
		 */
		template <typename... RecordArgs>
			void Register(RecordArgs &&...);

		private:
		/**
		 * @copydoc Register
		 */
		void Register(const Record &);

		public:
		/**
		 * Returns the encoder that best matches the given arguments.
		 *
		 * If @c extension or @c format is empty, it will not be considered as
		 * criteria during the search.
		 *
		 * @note If @c bestMatch is @c true, the @em closest-matching encoder
		 *       will be returned, even if it doesn't exactly match any of the
		 *       given arguments.  Otherwise, only those encoders which exactly
		 *       match the given arguments will be considered.
		 */
		const Record &GetEncoder(
			std::string    const& format    = "",
			std::string    const& extension = "",
			bool                  bestMatch = true) const;

		private:
		std::forward_list<Record> records;
		std::unordered_map<std::string, std::forward_list<typename decltype(records)::const_iterator>> formats;
		std::unordered_map<std::string, std::forward_list<typename decltype(records)::const_iterator>> extensions;
	};
}}

////////////////////////////////////////////////////////////////////////////////

	/**
	 * Registers a encoder with @c EncoderRegistry.
	 */
#	define REGISTER_ENCODER(T, ...) \
		namespace \
		{ \
			struct Initializer() \
			{ \
				Initializer() \
				{ \
					GLOBAL(::page::res::EncoderRegistry).Register<T>(__VA_ARGS__); \
				} \
			} initializer __attribute__((init_priority(REG_INIT_PRIORITY))); \
		}

#	include "EncoderRegistry.tpp"
#endif
