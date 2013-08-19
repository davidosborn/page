namespace page { namespace math
{
	bool IsPow2(unsigned n)
	{
		return !(n & n - 1);
	}
	unsigned Pow2Floor(unsigned n)
	{
		unsigned r = ~0 ^ ~0 >> 1;
		while (r > n) r >>= 1;
		return r;
	}
	unsigned Pow2Ceil(unsigned n)
	{
		unsigned r = 1;
		while (r < n) r <<= 1;
		return r;
	}
}}
