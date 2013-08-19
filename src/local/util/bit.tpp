namespace page
{
	namespace util
	{
		template <typename T> unsigned Bits(T n)
		{
			unsigned r = 0;
			while (n &= n - 1) r++;
			return r;
		}
		template <typename T> unsigned LowestBit(T n)
		{
			unsigned r = 0;
			while (!(n & 1 << r)) if (++r == sizeof n) return 0;
			return r;
		}
		template <typename T> unsigned HighestBit(T n)
		{
			unsigned r = sizeof n - 1;
			while (!(n & 1 << r)) if (!r--) return 0;
			return r;
		}
	}
}
