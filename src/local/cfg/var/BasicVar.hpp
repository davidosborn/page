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

#ifndef    page_local_cfg_var_BasicVar_hpp
#   define page_local_cfg_var_BasicVar_hpp

#	include <iosfwd> // {,w}[io]stream
#	include <istream> // basic_istream
#	include <ostream> // basic_ostream
#	include <string>

namespace page { namespace cfg
{
	class BasicState;

	/**
	 * The base class for configuration variables.
	 */
	class BasicVar
	{
		/*--------+
		| friends |
		+--------*/

		private:
		template <typename Char, typename CharTraits>
			friend std::basic_ostream<Char, CharTraits> &operator <<(std::basic_ostream<Char, CharTraits> &, const BasicVar &);

		template <typename Char, typename CharTraits>
			friend std::basic_istream<Char, CharTraits> &operator >>(std::basic_istream<Char, CharTraits> &, BasicVar &);

		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		protected:
		explicit BasicVar(BasicState &, const std::string &key);

		public:
		virtual ~BasicVar();

		/*----------+
		| observers |
		+----------*/

		public:
		/**
		 * @return @copydoc key
		 */
		const std::string &GetKey() const;

		/**
		 * @return the variable's value, serialized to a string.
		 */
		std::string GetString() const;

		/**
		 * @return @copydoc modified
		 */
		bool IsModified() const;

		/*----------+
		| modifiers |
		+----------*/

		public:
		/**
		 * Set the variable to its default value.
		 *
		 * @note Sets the variable's "modified" state.
		 */
		virtual void Unset() = 0;

		/**
		 * Clears the variable's "modified" state.
		 */
		void ClearModified();

		/*--------------+
		| serialization |
		+--------------*/

		private:
		/**
		 * Serialize the variable's value to a stream.
		 */
		virtual void Serialize(std:: ostream &) const = 0;
		virtual void Serialize(std::wostream &) const = 0;

		/**
		 * Deserialize the variable's value from a stream.
		 */
		virtual void Deserialize(std:: istream &) = 0;
		virtual void Deserialize(std::wistream &) = 0;

		/*-------------+
		| data members |
		+-------------*/

		private:
		/**
		 * The state the variable is associated with.
		 */
		BasicState &state;

		/**
		 * The variable's key.
		 */
		std::string key;

		protected:
		/**
		 * @c true if the configuration variable been modified.
		 */
		bool modified;
	};

	/*---------+
	| ordering |
	+---------*/

	/**
	 * Compare two configuration variables by key.
	 */
	bool operator <(const BasicVar &, const BasicVar &);

	/*------------------------------+
	| stream insertion & extraction |
	+------------------------------*/

	/**
	 * Serialize a variable's value to a stream.
	 */
	template <typename T, typename ExternT, typename Char, typename CharTraits>
		std::basic_ostream<Char, CharTraits> &operator <<(std::basic_ostream<Char, CharTraits> &, const BasicVar &);

	/**
	 * Deserialize a variable's value from a stream.
	 */
	template <typename T, typename ExternT, typename Char, typename CharTraits>
		std::basic_istream<Char, CharTraits> &operator >>(std::basic_istream<Char, CharTraits> &, BasicVar &);
}}

#	include "BasicVar.tpp"
#endif
