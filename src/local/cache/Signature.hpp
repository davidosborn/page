#ifndef    page_local_cache_Signature_hpp
#   define page_local_cache_Signature_hpp

#	include <iosfwd> // basic_ostream
#	include <string>
#	include <utility> // forward
#	include <vector>

#	include <boost/variant.hpp>

namespace page { namespace cache
{
////////// SigType /////////////////////////////////////////////////////////////

	/**
	 *
	 */
	struct SigType
	{
		SigType() = default;

		SigType(
			const std::string &name,
			const std::vector<SigType> &params);

		SigType(
			const char *name,
			const std::vector<SigType> &params);

		/**
		 * Parses the type portion of a signature string.
		 */
		SigType(const std::string &);

		/**
		 * Parses the type portion of a signature string.
		 *
		 * @note Required to allow passing of string literals to Signature's
		 *       constructor (ISO/IEC N3242 12.3.4).
		 */
		SigType(const char *);

		/**
		 * Generates a signature string.
		 */
		operator std::string() const;

		std::string name;
		std::vector<SigType> params;
	};

	// stream insertion/extraction
	std::ostream &operator <<(std::ostream &, const SigType &);
	std::istream &operator >>(std::istream &, SigType &);

	// comparison
	bool operator ==(const SigType &, const SigType &);
	bool operator !=(const SigType &, const SigType &);

////////// SigSource ///////////////////////////////////////////////////////////

	/**
	 *
	 */
	struct SigSource
	{
		SigSource(const std::vector<std::string> &parts = {});
		SigSource(std::initializer_list<std::string>);

		template <typename... Args>
			SigSource(Args &&... args) :
				SigSource({std::forward<Args>(args)...}) {}

		/**
		 * Parses the source portion of a signature string.
		 */
		SigSource(const std::string &);

		/**
		 * Parses the source portion of a signature string.
		 *
		 * @note Required to allow passing of string literals to Signature's
		 *       constructor (ISO/IEC N3242 12.3.4).
		 */
		SigSource(const char *);

		/**
		 * Generates the source portion of a signature string.
		 */
		operator std::string() const;

		std::vector<std::string> parts;
	};

	// stream insertion/extraction
	std::ostream &operator <<(std::ostream &, const SigSource &);
	std::istream &operator >>(std::istream &, SigSource &);

	// comparison
	bool operator ==(const SigSource &, const SigSource &);
	bool operator !=(const SigSource &, const SigSource &);

////////// Signature ///////////////////////////////////////////////////////////

	/**
	 * A string wrapper that uniquely identifies a resource based on its type
	 * and source.
	 *
	 * The signature is formatted like a constructor call, in the following
	 * manner: <tt>type(source)</tt>.
	 *
	 * If the type has been parameterized by sub-types, it is formatted like a
	 * template specialization: <tt>type<subtype-1,subtype-2,...></tt>.
	 * Otherwise, the type is formatted by itself.  Sub-types can be further
	 * specialized by sub-sub-types, in the same manner, and so on.
	 *
	 * The source is formatted as CSV (comma-separated values), where each value
	 * represents a parameter involved in acquiring the resource, which together
	 * define its source.  There should be no extraneous whitespace between
	 * values.  If a parameter is a resource, it should be represented by its
	 * signature.
	 */
	struct Signature
	{
		Signature() = default;
		Signature(const SigType &, const SigSource &);

		/**
		 * Parses a signature string.
		 */
		Signature(const std::string &);

		/**
		 * Generates a signature string, which uniquely identifies the resource.
		 */
		operator std::string() const;

		/**
		 * The type of the resource.
		 */
		SigType type;

		/**
		 * The source of the resource.
		 */
		SigSource source;
	};

	// stream insertion/extraction
	std::ostream &operator <<(std::ostream &, const Signature &);
	std::istream &operator >>(std::istream &, Signature &);

	// comparison
	bool operator ==(const Signature &, const Signature &);
	bool operator !=(const Signature &, const Signature &);
}}

////////// std::hash<Signature> ////////////////////////////////////////////////

namespace std
{
	template <typename> struct hash;

	/**
	 * A specialization of @c hash for @c ::page::cache::Signature.
	 */
	template <> struct hash<::page::cache::Signature>
	{
		using result_type   = typename hash<string>::result_type;
		using argument_type = ::page::cache::Signature;

		result_type operator ()(const argument_type &x) const noexcept
		{
			return hash<string>()(x);
		}
	};
}

#endif
