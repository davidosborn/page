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

#ifndef    page_cfg_Var_hpp
#   define page_cfg_Var_hpp

#	include <functional> // function
#	include <iosfwd>
#	include <iterator> // bidirectional_iterator_tag, iterator
#	include <string>
#	include "../util/iterator/iterator_adapter.hpp"
#	include "../util/iterator/range.hpp"
#	include "domain.hpp"

namespace page
{
	namespace cfg
	{
		class Reader;
		class Writer;

		namespace detail
		{
			/**
			 * A bi-directional linked-list iterator for @c BasicVar.
			 *
			 * @sa BasicVar::GetGlobalRange
			 */
			template <typename T> struct BasicVarIterator :
				util::iterator_adapter<BasicVarIterator<T>, T *,
					std::bidirectional_iterator_tag, T>
			{
				template <typename> friend class BasicVarIterator;

				friend class util::iterator_facade<BasicVarIterator<T>,
					std::bidirectional_iterator_tag, T>;

				typedef util::iterator_adapter<BasicVarIterator<T>, T *,
					std::bidirectional_iterator_tag, T> Base;

				// constructors
				explicit BasicVarIterator(T * = nullptr);
				template <typename U>
					BasicVarIterator(const BasicVarIterator<U> &);

				private:
				// arithmetic
				void Increment() override;
				void Decrement() override;
			};
		}

		/**
		 * The base class for configuration variables.
		 */
		struct BasicVar
		{
			template <typename> friend class detail::BasicVarIterator;

			typedef detail::BasicVarIterator<BasicVar> Iterator;
			typedef detail::BasicVarIterator<const BasicVar> ConstIterator;

			// constructor
			explicit BasicVar(const std::string &name);

			// destructor
			virtual ~BasicVar();

			// observers
			const std::string &GetName() const;
			std::string GetRepr(Domain = autoDomain) const;
			bool WasUserModified() const;

			// modifiers
			void Unset(Domain = autoDomain);

			// revision control
			virtual bool Update(Reader &, Domain)       = 0;
			virtual void Commit(Writer &, Domain) const = 0;

			// serialization
			template <typename Char, typename CharTraits>
				std::basic_ostream<Char, CharTraits> &Serialize(
					std::basic_ostream<Char, CharTraits> &, Domain) const;
			template <typename Char, typename CharTraits>
				std::basic_istream<Char, CharTraits> &Deserialize(
					std::basic_istream<Char, CharTraits> &, Domain);

			/**
			 * A static function providing access to all existing configuration
			 * variables through @c util::range.
			 */
			static util::range<Iterator> GetGlobalRange();

			protected:
			// serialization implementation
			virtual void DoSerialize  (std:: ostream &, Domain) const = 0;
			virtual void DoSerialize  (std::wostream &, Domain) const = 0;
			virtual void DoDeserialize(std:: istream &, Domain)       = 0;
			virtual void DoDeserialize(std::wistream &, Domain)       = 0;

			bool         userSet, siteSet; // assigned?
			mutable bool userMod, siteMod; // modified?
			bool         userEverMod;      // ever modified?

			private:
			std::string name;
			       BasicVar *prev,  *next;
			static BasicVar *first, *last;
		};

		/**
		 * A class template representing a configuration variable with a
		 * parameterized type.
		 */
		template <typename T, typename ExternT = T> struct Var : BasicVar
		{
			// filters
			typedef std::function<T       (const T       &)> LimitFilter;
			typedef std::function<T       (const ExternT &)> ConvertInFilter;
			typedef std::function<ExternT (const T       &)> ConvertOutFilter;

			// constructor
			explicit Var(
				const std::string      &name,
				const T                &defaultValue = T(),
				const LimitFilter      &             = nullptr,
				const ConvertInFilter  &             = nullptr,
				const ConvertOutFilter &             = nullptr);

			// observers
			const T &operator  *() const noexcept;
			const T *operator ->() const noexcept;
			const T &Get(Domain = autoDomain) const noexcept;

			// modifiers
			Var &operator =(const T &);
			void Set(const T &, Domain = autoDomain);

			// revision control
			bool Update(Reader &, Domain = autoDomain)       override;
			void Commit(Writer &, Domain = autoDomain) const override;

			protected:
			// serialization implementation (level 1)
			void DoSerialize  (std:: ostream &, Domain) const override;
			void DoSerialize  (std::wostream &, Domain) const override;
			void DoDeserialize(std:: istream &, Domain)       override;
			void DoDeserialize(std::wistream &, Domain)       override;

			private:
			// serialization implementation (level 2)
			template <typename Char, typename CharTraits>
				void DoSerializeImpl(std::basic_ostream<Char, CharTraits> &, Domain) const;
			template <typename Char, typename CharTraits>
				void DoDeserializeImpl(std::basic_istream<Char, CharTraits> &, Domain);

			// default filters
			class DefaultLimitFilter;
			class DefaultConvertInFilter;
			class DefaultConvertOutFilter;

			const T defaultValue;
			T userValue, siteValue;
			LimitFilter      limitFilter;
			ConvertInFilter  convertInFilter;
			ConvertOutFilter convertOutFilter;
		};

		// stream insertion/extraction
		template <typename T, typename ExternT, typename Char, typename CharTraits>
			std::basic_ostream<Char, CharTraits> &operator <<(std::basic_ostream<Char, CharTraits> &, const Var<T, ExternT> &);
		template <typename T, typename ExternT, typename Char, typename CharTraits>
			std::basic_istream<Char, CharTraits> &operator >>(std::basic_istream<Char, CharTraits> &, Var<T, ExternT> &);
	}
}

#	include "Var.tpp"
#endif
