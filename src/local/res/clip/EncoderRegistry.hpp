#ifndef    page_local_res_clip_EncoderRegistry_hpp
#   define page_local_res_clip_EncoderRegistry_hpp

#	include <functional> // function
#	include <list>
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
	 * A function that is called by an encoder when it has encoded data that
	 * needs to be written.
	 */
	typedef std::function<void (const void *, unsigned)> EncoderCallback;

	/**
	 * A factory function that will return a new @c Encoder object, constructed
	 * with the provided arguments.
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
		 * @note The first extension in @a extensions will become the value of
		 *       @c defaultExtension.
		 */
		EncoderRegistryRecord(
			EncoderFactoryFunction   const& factoryFunction,
			std::string              const& name,
			std::vector<std::string> const& formats          = {},
			std::vector<std::string> const& extensions       = {},
			bool                            binary           = true,
			int                             priority         = 0);

		/**
		 * @copydoc EncoderFactoryFunction
		 */
		EncoderFactoryFunction factoryFunction;

		/**
		 * The name of the encoder.
		 */
		std::string name;

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
	 *
	 * @addtogroup registry
	 */
	class EncoderRegistry : public util::Monostate<EncoderRegistry>
	{
		/**
		 * Short-form identifier in context.
		 */
		typedef EncoderRegistryRecord Record;

		public:
		/**
		 * Registers an encoder.
		 */
		template <typename T, typename... RecordArgs>
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
		std::list<Record> records;
		std::unordered_map<std::string, std::list<typename decltype(records)::const_iterator>> formats;
		std::unordered_map<std::string, std::list<typename decltype(records)::const_iterator>> extensions;
	};
}}

////////////////////////////////////////////////////////////////////////////////

	/**
	 * Registers a encoder with @c EncoderRegistry.
	 */
#	define REGISTER_ENCODER(T, ...) \
		namespace \
		{ \
			struct Initializer \
			{ \
				Initializer() \
				{ \
					GLOBAL(::page::res::EncoderRegistry).Register<T>(__VA_ARGS__); \
				} \
			} initializer __attribute__((init_priority(REG_INIT_PRIORITY))); \
		}

#	include "EncoderRegistry.tpp"
#endif
