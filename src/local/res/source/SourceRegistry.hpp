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

#ifndef    page_local_res_source_SourceRegistry_hpp
#   define page_local_res_source_SourceRegistry_hpp

#	include <forward_list>
#	include <functional>
#	include <memory> // unique_ptr
#	include <string>

#	include "../../util/class/Monostate.hpp"
#	include "../../util/gcc/init_priority.hpp" // REG_INIT_PRIORITY

namespace page { namespace res
{
	/**
	 * A pointer to a factory function that, when called, will return a new
	 * @c Source object, constructed with the given arguments.
	 */
	using SourceFactoryFunction = std::function<std::unique_ptr<Source> (const std::string &path)>;

	/**
	 * A pointer to a function that, when called, will return @c true if the
	 * source is capable of handling the specified path.
	 */
	using SourceCompatibleFunction = std::function<bool (const std::string &path)>;

////////// SourceRegistryRecord ////////////////////////////////////////////////

	/**
	 * A data structure used by @c SourceRegistry which contains information
	 * about a source class.
	 */
	struct SourceRegistryRecord
	{
		SourceRegistryRecord(
			SourceFactoryFunction    const& factoryFunction,
			SourceCompatibleFunction const& compatibleFunction,
			int                             priority = 0);

		/**
		 * @copydoc SourceFactoryFunction
		 */
		SourceFactoryFunction factoryFunction;

		/**
		 * @copydoc SourceCompatibleFunction
		 */
		SourceCompatibleFunction compatibleFunction;

		/**
		 * A higher priority means that this source will be tried before other
		 * sources with a lower priority.
		 */
		int priority;
	};

////////// SourceRegistry //////////////////////////////////////////////////////

	/**
	 * A place for registering @c Source classes.
	 */
	class SourceRegistry : public util::Monostate<SourceRegistry>
	{
		/**
		 * Short-form identifier in context.
		 */
		typedef SourceRegistryRecord Record;

		public:
		/**
		 * Registers a source class.
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
		 * Searches for a source that is capable of handling the specified path
		 * and returns a new instance of it.
		 */
		std::unique_ptr<Source> Make(const std::string &path) const;

		private:
		std::forward_list<Record> records;
	};
}}

////////////////////////////////////////////////////////////////////////////////

	/**
	 * Registers a type with @c SourceRegistry.
	 */
#	define REGISTER_SOURCE(T, ...) \
		namespace \
		{ \
			struct Initializer() \
			{ \
				Initializer() \
				{ \
					GLOBAL(::page::res::SourceRegistry).Register<T>(__VA_ARGS__); \
				} \
			} initializer __attribute__((init_priority(REG_INIT_PRIORITY))); \
		}

#	include "SourceRegistry.tpp"
#endif
