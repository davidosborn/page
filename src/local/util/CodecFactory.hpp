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

#ifndef    page_local_util_CodecFactory_hpp
#   define page_local_util_CodecFactory_hpp

	// C++
#	include <functional> // function
#	include <list>
#	include <unordered_map>
#	include <memory> // unique_ptr
#	include <string> // string
#	include <vector>

	// Boost
#	include <boost/filesystem/path.hpp>

namespace page
{
	namespace util
	{
		/**
		 * An factory for producing concrete instances of an abstract codec
		 * type, where the concrete type is selected from a pool of registered
		 * codecs according to a standard criteria.
		 *
		 * @note Based in part on "Conversations: Abstract Factory, Template
		 *       Style" by Jim Hyslop and Herb Sutter.
		 */
		template <typename AbstractCodec, typename... ConstructorArgs>
			class CodecFactory
		{
			/*------+
			| types |
			+------*/

			public:
			/**
			 * A factory function which produces instances of a registered
			 * codec.
			 */
			typedef std::function<std::unique_ptr<AbstractCodec> (ConstructorArgs...)> Function;

			/*----------------+
			| global instance |
			+----------------*/

			public:
			/**
			 * Provides global access to a single instance of the class.
			 */
			static CodecFactory &GetGlobalInstance();

			/*----------+
			| observers |
			+----------*/

			public:
			/**
			 * @returns A list of the registered formats, ordered by
			 * descending priority.
			 */
			std::vector<std::string> GetFormats() const;

			/**
			 * @return A list of the registered mime-types, ordered by
			 * descending priority.
			 */
			std::vector<std::string> GetMimeTypes() const;

			/**
			 * @return A list of the registered extensions, ordered by
			 * descending priority.
			 */
			std::vector<std::string> GetExtensions() const;

			/*-------------+
			| registration |
			+-------------*/

			public:
			/**
			 * Registers a concrete codec.
			 *
			 * @note Even if your codec can handle multiple formats, you should
			 *       register it separately for each format, because the first
			 *       associated extension will be applied to the path, if it
			 *       doesn't already have an extension, during production.
			 */
			void Register(
				const Function &,
				const std::string &formats,
				const std::string &mimeTypes,
				const std::string &extensions,
				int priority);

			/**
			 * Registers a concrete codec, generating a default factory function
			 * for the type.
			 *
			 * @note Even if your codec can handle multiple formats, you should
			 *       register it separately for each format, because the first
			 *       associated extension will be applied to the path, if it
			 *       doesn't already have an extension, during production.
			 */
			template <typename ConcreteCodec>
				void Register(
					const std::string &formats,
					const std::string &mimeTypes,
					const std::string &extensions,
					int priority);

			/*-----------+
			| production |
			+-----------*/

			public:
			/**
			 * A structure containing the codec produced by @c Make, as well as
			 * some additional information.
			 */
			struct Product
			{
				std::unique_ptr<AbstractCodec> codec;
				boost::filesystem::path path;
			};

			/**
			 * Returns a new codec instance.
			 *
			 * Walks through the registered codecs, in order of priority, and
			 * calling each one's factory function, and continuing until a one
			 * of them succeeds or it runs out of codecs.
			 *
			 * If none of the factory functions succeed, an exception is thrown.
			 */
			Product Make(
				const boost::filesystem::path &,
				ConstructorArgs &&...) const;

			/**
			 * Return a new codec instance, trying the factory functions of
			 * codecs that match the specified criteria.
			 */
			Product Make(
				const boost::filesystem::path &,
				const std::string &format,
				const std::string &mimeType,
				ConstructorArgs &&...) const;

			/*-------------+
			| data members |
			+-------------*/

			private:
			/**
			 * A structure containing information about a registered codec.
			 */
			struct ConcreteCodec
			{
				ConcreteCodec(const Function &, int priority);

				/**
				 * @copydoc Function
				 */
				Function function;

				/**
				 * A list of formats associated with the codec.
				 */
				std::vector<std::string> formats;

				/**
				 * A list of mime types associated with the codec.
				 */
				std::vector<std::string> mimeTypes;

				/**
				 * A list of extensions associated with the codec.
				 */
				std::vector<std::string> extensions;

				/**
				 * The codec's priority, in the range of [0, 100], where the
				 * higher priority codecs are selected first.
				 *
				 * @note When a new codec is registered, it is placed ahead of
				 *       any previously registered codecs of the same priority.
				 */
				int priority;

				/**
				 * A flag specifying whether the codec has already been tried
				 * during production.  It must be reset before @c Make returns.
				 */
				mutable bool tried = false;
			};

			/**
			 * A list of registered codecs, ordered by descending priority.
			 */
			std::list<ConcreteCodec> codecs;

			/**
			 * Resets @c ConcreteCodec::tried for all @c codecs.  Should be
			 * called just before returning from @c Make.
			 */
			void ResetTried() const;

			/**
			 * An associative array mapping all registered formats to their
			 * associated codecs.
			 */
			std::unordered_map<
				std::string,
				std::list<typename decltype(codecs)::const_iterator>>
					formatToCodec;

			/**
			 * An associative array mapping all registered mime-types to their
			 * associated codecs.
			 */
			std::unordered_map<
				std::string,
				std::list<typename decltype(codecs)::const_iterator>>
					mimeTypeToCodec;

			/**
			 * An associative array mapping all registered extensions to their
			 * associated codecs.
			 */
			std::unordered_map<
				std::string,
				std::list<typename decltype(codecs)::const_iterator>>
					extensionToCodec;
		};

		/**
		 * Supports the ordering of codecs by priority.
		 */
		template <typename T, typename... A>
			bool operator ==(
				const typename CodecFactory<T, A...>::ConcreteCodec &,
				const typename CodecFactory<T, A...>::ConcreteCodec &);

		/**
		 * Supports the ordering of codecs by priority.
		 */
		template <typename T, typename... A>
			bool operator <(
				const typename CodecFactory<T, A...>::ConcreteCodec &,
				const typename CodecFactory<T, A...>::ConcreteCodec &);
	}
}

#	include "CodecFactory.tpp"
#endif
