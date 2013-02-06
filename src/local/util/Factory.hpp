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
 
#ifndef    page_local_util_Factory_hpp
#	define page_local_util_Factory_hpp

	// C++
#	include <functional> // function
#	include <list>
#	include <unordered_map>
#	include <memory> // unique_ptr
#	include <string> // string
#	include <vector>

namespace page
{
	namespace util
	{
		/**
		 * An abstract-factory class-template.
		 *
		 * @note This factory is parameterized by the type's constructor
		 *       arguments.
		 *
		 * @note Based in part on "Conversations: Abstract Factory, Template
		 *       Style" by Jim Hyslop and Herb Sutter.
		 */
		template <typename T, typename... Args>
			class Factory
		{
			/*------+
			| types |
			+------*/
		
			public:
			typedef std::function<std::unique_ptr<T> (Args...)> Function;
			typedef std::string Key;
			typedef int Priority;

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
			 * Return a list of the available keys, ordered by descending
			 * priority.
			 */
			std::vector<Key> GetKeys() const;

			/*-------------+
			| registration |
			+-------------*/

			public:
			/**
			 * Register a factory function with an associated key and priority.
			 */
			void Register(const Function &, const Key &, Priority priority);

			/**
			 * Register a type with an associated key and priority, generating
			 * a default factory function for the type.
			 */
			template <typename U>
				void Register(const Key &, Priority priority);

			/*-----------+
			| production |
			+-----------*/

			public:
			/**
			 * Return a new instance of @c T.
			 *
			 * Walks through the registered types, starting with the highest-
			 * priority type, and calls its factory function.  It continues
			 * until a factory function succeeds, or it runs out of types.
			 *
			 * If none of the factory functions succeed, an exception is thrown.
			 */
			std::unique_ptr<T> Make(Args &&...) const;

			/**
			 * Return a new instance of @c T, using the factory function
			 * matched by the key.
			 */
			std::unique_ptr<T> Make(const Key &, Args &&...) const;

			/*-----------------+
			| member variables |
			+-----------------*/

			private:
			/**
			 * A structure containg information about a registered type,
			 * including a factory function for creating instances of the type,
			 * as well as some associated ordering information.
			 */
			struct Type
			{
				Function function;
				Key      key;
				Priority priority;
			};
			
			/**
			 * A sorted list of registered types, ordered by descending
			 * priority.
			 */
			std::list<Type> types;
			
			/**
			 * A map of each key to its associated type.
			 */
			std::unordered_map<Key, typename decltype(types)::iterator> keyMap;
		};
	}
}

#	include "Factory.tpp"
#endif
