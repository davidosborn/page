#ifndef    page_local_cache_Signature_hpp
#   define page_local_cache_Signature_hpp

#	include <iosfwd> // [io]stream
#	include <string>

namespace page { namespace cache
{
	/**
	 * A string wrapper that uniquely identifies a resource based on its type
	 * and source.
	 *
	 * The signature is formatted like a function call:
	 * <tt>type(dependency1, dependency2...)</tt>.
	 *
	 * A dependency represents a secondary resource that is needed to acquire
	 * the primary resource.  Together, the dependencies define the source of
	 * the primary resource.
	 */
	class Signature
	{
		public:
		template <typename... Args>
			explicit Signature(const std::string &type, Args &&...);

		// string access
		std::string &str();
		const std::string &str() const;

		private:
		std::string s;
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
	template <typename>
		struct hash;

	/**
	 * A specialization of hash for ::page::cache::Signature.
	 */
	template <>
		struct hash<::page::cache::Signature>
	{
		using result_type   = typename hash<string>::result_type;
		using argument_type = ::page::cache::Signature;

		result_type operator ()(const argument_type &x) const noexcept;
	};
}

#	include "Signature.tpp"
#endif
