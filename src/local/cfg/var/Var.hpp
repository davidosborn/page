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

#ifndef    page_local_cfg_var_Var_hpp
#   define page_local_cfg_var_Var_hpp

#	include <functional> // function

#	include "../../util/ArrowProxy.hpp"
#	include "BasicVar.hpp"

namespace page { namespace cfg
{
	/**
	 * A configuration variable with a parameterized type.
	 *
	 * This class provides the interface for user access to configuration
	 * variables.  Each instance represents a configuration variable.
	 * Dereferencing yields the variable's value, and assignment sets the value.
	 *
	 * This class can be configured on construction with a number of filters.
	 * These filters affect the way the variable's value is interpreted when
	 * it is read from, or written to, the source, or when getting or setting
	 * it.
	 *
	 * @tparam T The @em real type of the variable's value, so far as the user
	 *         of the variable is concerned.
	 *
	 * @tparam ExternT The external type of the variable's value.  This is the
	 *         variable's type when stored in a @c Source, such as a file.  For
	 *         @c enum values, it is often @c std::string.
	 */
	template <typename T, typename ExternT = T>
		class Var : public BasicVar
	{
		/*-------------+
		| filter types |
		+-------------*/

		public:
		/**
		 * A filter that is applied to the value returned from Get().
		 */
		typedef std::function<T (const T &)> GetFilter;

		/**
		 * A filter that is applied to the value passed to Set().
		 */
		typedef std::function<T (const T &)> SetFilter;

		/**
		 * A filter that is applied to the value returned from Deserialize(), to
		 * convert it from @c ExternT to @c T.
		 */
		typedef std::function<T (const ExternT &)> ConvertInFilter;

		/**
		 * A filter that is applied to the value passed to Serialize(), to
		 * convert it from @c T to @c ExternT.
		 */
		typedef std::function<ExternT (const T &)> ConvertOutFilter;

		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit Var(
			BasicState &,
			const std::string      &key,
			const T                &defaultValue = T(),
			const GetFilter        &             = nullptr,
			const SetFilter        &             = nullptr,
			const ConvertInFilter  &             = nullptr,
			const ConvertOutFilter &             = nullptr);

		/*----------+
		| observers |
		+----------*/

		public:
		/**
		 * Calls Get().
		 */
		T operator *() const;

		/**
		 * Calls Get().
		 */
		util::ArrowProxy<T> operator ->() const;

		/**
		 * @return Var::value, after applying Var::getFilter.
		 */
		T Get() const;

		/*----------+
		| modifiers |
		+----------*/

		public:
		/**
		 * Calls Set().
		 */
		Var &operator =(const T &);

		/**
		 * Sets Var::value, after applying Var::setFilter.
		 *
		 * @note Sets the variable's "modified" state.
		 */
		void Set(const T &);

		void Unset() override;

		/*--------------+
		| serialization |
		+--------------*/

		private:
		/**
		 * Serializes the variable's value to a stream, calling Get() and
		 * appyling Var::convertOutFilter.
		 */
		void Serialize(std:: ostream &) const override;
		void Serialize(std::wostream &) const override;

		/**
		 * Deserializes the variable's value from a stream, applying
		 * Var::convertInFilter and calling Set().
		 */
		void Deserialize(std:: istream &) override;
		void Deserialize(std::wistream &) override;

		/*----------------+
		| default filters |
		+----------------*/

		private:
		/**
		 * The default @c GetFilter, which leaves the value as it is.
		 */
		class DefaultGetFilter;

		/**
		 * The default @c SetFilter, which leaves the value as it is.
		 */
		class DefaultSetFilter;

		/**
		 * The default @c SetFilter, which performs an implicit conversion from
		 * @c ExternT to @c T.
		 */
		class DefaultConvertInFilter;

		/**
		 * The default @c SetFilter, which performs an implicit conversion from
		 * @c T to @c ExternT.
		 */
		class DefaultConvertOutFilter;

		/*-------------+
		| data members |
		+-------------*/

		private:
		/**
		 * The current value of the variable.
		 */
		T value;

		/**
		 * The default value of the variable, which is used when the variable
		 * is not defined in any @c Source, or after a call to Unset().
		 */
		const T defaultValue;

		GetFilter        getFilter;        /// @copydoc GetFilter
		SetFilter        setFilter;        /// @copydoc SetFilter
		ConvertInFilter  convertInFilter;  /// @copydoc ConvertInFilter
		ConvertOutFilter convertOutFilter; /// @copydoc ConvertOutFilter
	};
}}

#endif
