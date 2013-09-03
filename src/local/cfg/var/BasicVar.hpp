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

		/*-------------+
		| constructors |
		+-------------*/

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

	/*----------------------------+
	| stream insertion/extraction |
	+----------------------------*/

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
