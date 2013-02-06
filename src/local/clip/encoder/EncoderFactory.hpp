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

#ifndef    page_local_clip_encoder_EncoderFactory_hpp
#	define page_local_clip_encoder_EncoderFactory_hpp

	// C++
#	include <functional> // function
#	include <list>
#	include <unordered_map>
#	include <memory> // unique_ptr
#	include <string> // string
#	include <vector>

	// local
#	include "../../math/Vector.hpp"
#	include "Encoder.hpp" // Encoder::Callback

namespace page
{
	namespace clip
	{
		/**
		 * An factory for producing @c Encoder instances.
		 */
		class EncoderFactory
		{
			/*------+
			| types |
			+------*/

			public:
			/**
			 *
			 */
			typedef std::function<std::unique_ptr<Encoder> (
				const Encoder::Callback &callback,
				const math::Vector<2, unsigned> &size,
				float frameRate,
				float quality)> Function;

			/*----------------+
			| global instance |
			+----------------*/

			public:
			/**
			 * Provides global access to a single instance of the class.
			 */
			static Factory &GetGlobalInstance();

			/*----------+
			| observers |
			+----------*/

			public:
			/**
			 * Return a list of the available formats, ordered by descending
			 * priority.
			 */
			std::vector<std::string> GetFormats() const;

			/**
			 * Return a list of the available extensions, ordered by descending
			 * priority.
			 */
			std::vector<std::string> GetExtensions() const;

			/*-------------+
			| registration |
			+-------------*/

			public:
			/**
			 * Register an encoder, with its factory function, formats,
			 * extensions, and priority.
			 */
			void Register(
				const Function &,
				const std::string &formats,
				const std::string &extensions,
				int priority);

			/**
			 * Register a type with associated formats, extensions, and
			 * priority, generating a default factory function for the type.
			 */
			template <typename T>
				void Register(
					const std::string &formats,
					const std::string &extensions,
					int priority)
			{
				Register(
					[] { return std::unique_ptr<Encoder>(new T()); },
					formats,
					extensions,
					priority);
			}

			/*-----------+
			| production |
			+-----------*/

			public:
			/**
			 * Return a new @c Encoder instance.
			 *
			 * Walks through the registered types, starting with the highest-
			 * priority type, and calls its factory function.  It continues
			 * until a factory function succeeds, or it runs out of types.
			 *
			 * If none of the factory functions succeed, an exception is thrown.
			 */
			std::unique_ptr<Encoder> Make() const;

			/**
			 * Return a new @c Encoder instance, using the factory function
			 * matched by the extension and format.
			 */
			std::unique_ptr<Encoder> Make(
				const std::string &format,
				const std::string &extension) const;

			/*-------------+
			| data members |
			+-------------*/

			private:
			/**
			 * A structure containing information about a registered type.
			 */
			struct ConcreteType
			{
				/**
				 * A factory function that can be used to produce instances of
				 * the type.
				 */
				Function function;

				/**
				 * A list of formats associated with the type.
				 */
				std::vector<std::string> formats;

				/**
				 * A list of extensions associated with the type.
				 */
				std::vector<std::string> extensions;

				/**
				 * The type's priority, where the higher priority types are
				 * selected first, and which should be in the range of [0, 100].
				 *
				 * @note When a type is registered, it is placed ahead of any
				 *       previously registered types of the same priority.
				 */
				int priority;
			};

			/**
			 * Supports the ordering of concrete types by priority.
			 */
			static bool operator ==(const ConcreteType &, const ConcreteType &);

			/**
			 * Supports the ordering of concrete types by priority.
			 */
			static bool operator <(const ConcreteType &, const ConcreteType &);

			/**
			 * A list of registered types, ordered by descending priority.
			 */
			std::list<ConcreteType> types;

			/**
			 * An associative array mapping all registered formats to their
			 * associated concrete types.
			 */
			std::unordered_map<
				std::string,
				std::list<typename decltype(types)::const_iterator>>
					formatToEncoder;

			/**
			 * An associative array mapping all registered extensions to their
			 * associated concrete types.
			 */
			std::unordered_map<
				std::string,
				std::list<typename decltype(types)::const_iterator>>
					extensionToEncoder;
		};
	}
}

#endif
