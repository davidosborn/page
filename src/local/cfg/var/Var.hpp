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

#ifndef    page_local_cfg_var_Var_hpp
#   define page_local_cfg_var_Var_hpp

	// C++
#	include <functional> // function

	// local
#	include "BasicVar.hpp"

namespace page
{
	namespace cfg
	{
		/**
		 * Represents a configuration variable with a parameterized type.
		 */
		template <typename T, typename ExternT = T>
			class Var : public BasicVar
		{
			/*-------------+
			| filter types |
			+-------------*/

			public:
			typedef std::function<T       (const T       &)> LimitFilter;
			typedef std::function<T       (const ExternT &)> ConvertInFilter;
			typedef std::function<ExternT (const T       &)> ConvertOutFilter;

			/*--------------------------+
			| constructors & destructor |
			+--------------------------*/

			public:
			explicit Var(
				BasicState &,
				const std::string      &key,
				const T                &defaultValue = T(),
				const LimitFilter      &             = nullptr,
				const ConvertInFilter  &             = nullptr,
				const ConvertOutFilter &             = nullptr);

			/*----------+
			| observers |
			+----------*/

			public:
			/**
			 * @copydoc Get
			 */
			const T &operator *() const noexcept;

			/**
			 * @return A pointer to the variable's value.
			 */
			const T *operator ->() const noexcept;

			/**
			 * @return A reference to the variable's value.
			 */
			const T &Get() const noexcept;

			/*----------+
			| modifiers |
			+----------*/

			public:
			/**
			 * @copydoc Set
			 */
			Var &operator =(const T &);

			/**
			 * Set the variable's value.
			 *
			 * @note Sets the variable's "modified" state.
			 */
			void Set(const T &);

			void Unset() override;

			/*------------------+
			| (de)serialization |
			+------------------*/

			private:
			/**
			 * Serialize the variable's value to a stream.
			 */
			void Serialize(std:: ostream &) const override;
			void Serialize(std::wostream &) const override;

			/**
			 * Deserialize the variable's value from a stream.
			 */
			void Deserialize(std:: istream &) override;
			void Deserialize(std::wistream &) override;

			/*----------------+
			| default filters |
			+----------------*/

			private:
			class DefaultLimitFilter;
			class DefaultConvertInFilter;
			class DefaultConvertOutFilter;

			/*-------------+
			| data members |
			+-------------*/

			private:
			T value;
			const T defaultValue;

			LimitFilter      limitFilter;
			ConvertInFilter  convertInFilter;
			ConvertOutFilter convertOutFilter;
		};
	}
}

#endif
