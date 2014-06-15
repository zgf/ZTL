//±‡“Î∆⁄À„∑®
namespace ztl
{
	template<size_t N>
	class Log2
	{
	public:

		static const size_t	value = Log2<N / 2>::value + 1;

	};

	template<>
	class Log2<1>
	{
	public:
		static const size_t	value = 0;
	};
}