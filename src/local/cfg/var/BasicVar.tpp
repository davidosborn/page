namespace page { namespace cfg
{
	/*----------------------------+
	| stream insertion/extraction |
	+----------------------------*/

	template <typename T, typename ExternT, typename Char, typename CharTraits>
		std::basic_ostream<Char, CharTraits> &operator <<(std::basic_ostream<Char, CharTraits> &os, const BasicVar &var)
	{
		return var.Serialize(os);
	}

	template <typename T, typename ExternT, typename Char, typename CharTraits>
		std::basic_istream<Char, CharTraits> &operator >>(std::basic_istream<Char, CharTraits> &is, BasicVar &var)
	{
		return var.Deserialize(is);
	}
}}
