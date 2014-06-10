#pragma once
#include <crtdefs.h>
namespace ztl
{
	struct input_iterator
	{
	};
	struct output_iterator
	{
	};
	struct forward_iterator :input_iterator
	{
	};
	struct bidirectional_iterator :forward_iterator
	{
	};
	struct random_acess_iterator :bidirectional_iterator
	{
	};
	template<typename Iterator>
	class iterator_traits
	{
	public:
		typedef Iterator iterator_type;
		typedef typename iterator_type::value_type value_type;
		typedef typename iterator_type::pointer_type pointer_type;
		typedef typename iterator_type::reference_type reference_type;
		typedef typename iterator_type::different_type different_type;
		typedef typename iterator_type::iterator_category iterator_category;
	};
	template<typename Iterator>
	class iterator_traits<Iterator*>
	{
	public:
		typedef Iterator iterator_type;
		typedef Iterator value_type;
		typedef Iterator* pointer_type;
		typedef Iterator& reference_type;
		typedef ptrdiff_t different_type;
		typedef random_acess_iterator iterator_category;
	};
	template<typename Iterator>
	class iterator_traits<Iterator*const>
	{
	public:
		typedef Iterator iterator_type;
		typedef Iterator value_type;
		typedef const Iterator* pointer_type;
		typedef const Iterator& reference_type;
		typedef ptrdiff_t different_type;
		typedef random_acess_iterator iterator_category;
	};
	// return category from iterator argument
	template<class Iterator> inline
		typename iterator_traits<Iterator>::iterator_category
		get_iterator_category(const Iterator&)
	{	
			return (typename iterator_traits<Iterator>::iterator_category());
	}
}