
namespace ztl
{
	template<typename Container>
	void print_contianer(Container& container)
	{
		for (auto& Iter:container)
		{
			cout << Iter << " ";
		}
	}
	template<typename Container>
	bool is_same_container(Container a, Container b)
	{
		bool chose;
		if(chose = ztl::equal(a.begin(), a.end(), b.begin(), b.end()))
		{
			cout << "true!"
		}
		else
		{
			cout << "false!";
		}
		return chose;
	}
}