// deque::back 
#include <iostream> 
#include <deque> 

int  Test_deque__back()
{
	std::cout << "期望的结果:mydeque contains: 10 9 8 7 6 5 4 3 2 1 0\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::deque< int > mydeque;

	mydeque.push_back(10);

	while(mydeque.back() != 0)
		mydeque.push_back(mydeque.back() - 1);

	std::cout << "mydeque contains:";

	for(ztl::deque< int >::iterator_type it = mydeque.begin(); it != mydeque.end(); ++it)
		std::cout << ' ' << *it;

	std::cout << '\n';

	return  0;
} // deque::assign 
#include <iostream> 
#include <deque> 

int  Test_deque__assign()
{
	std::cout << "期望的结果:Size of first: 7\nSize of second: 5\nSize of third: 3\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::deque< int > first;
	ztl::deque< int > second;
	ztl::deque< int > third;

	first.assign(7, 100);              // 7 ints with a value of 100 

	ztl::deque< int >::iterator_type it;
	it = first.begin() + 1;

	second.assign(it, first.end() - 1);  // the 5 central values of first 

	int  myints[] = {1776, 7, 4};
	third.assign(myints, myints + 3);    // assigning from array. 

	std::cout << "Size of first: " << int(first.size()) << '\n';
	std::cout << "Size of second: " << int(second.size()) << '\n';
	std::cout << "Size of third: " << int(third.size()) << '\n';
	return  0;
} // clearing deques 
#include <iostream> 
#include <deque> 

int  Test_deque__clear()
{
	std::cout << "期望的结果:mydeque contains: 100 200 300\nmydeque contains: 1101 2202\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	unsigned   int  i;
	ztl::deque< int > mydeque;
	mydeque.push_back(100);
	mydeque.push_back(200);
	mydeque.push_back(300);

	std::cout << "mydeque contains:";
	for(ztl::deque< int >::iterator_type it = mydeque.begin(); it != mydeque.end(); ++it)
		std::cout << ' ' << *it;
	std::cout << '\n';

	mydeque.clear();
	mydeque.push_back(1101);
	mydeque.push_back(2202);

	std::cout << "mydeque contains:";
	for(ztl::deque< int >::iterator_type it = mydeque.begin(); it != mydeque.end(); ++it)
		std::cout << ' ' << *it;
	std::cout << '\n';

	return  0;
} // deque::cbegin/cend 
#include <iostream> 
#include <deque> 

int  Test_deque__cend()
{
	std::cout << "期望的结果:mydeque contains: 10 20 30 40 50\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::deque< int > mydeque = {10, 20, 30, 40, 50};

	std::cout << "mydeque contains:";

	for(auto it = mydeque.cbegin(); it != mydeque.cend(); ++it)
		std::cout << ' ' << *it;

	std::cout << '\n';

	return  0;
} // inserting into a deque 
#include <iostream> 
#include <deque> 
#include <vector> 

int  Test_deque__insert()
{
	std::cout << "期望的结果:mydeque contains: 1 20 30 30 20 10 2 3 4 5\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::deque< int > mydeque;

	// set some initial values: 
	for(int i = 1; i < 6; i++) mydeque.push_back(i);  // 1 2 3 4 5 

	ztl::deque< int >::iterator_type it = mydeque.begin();
	++it;

	it = mydeque.insert(it, 10);                   // 1 10 2 3 4 5 
	// "it" now points to the newly inserted 10 

	mydeque.insert(it, 2, 20);                      // 1 20 20 10 2 3 4 5 
	// "it" no longer valid! 

	it = mydeque.begin() + 2;

	ztl::vector< int > myvector(2, 30);
	mydeque.insert(it, myvector.begin(), myvector.end());
	// 1 20 30 30 20 10 2 3 4 5 

	std::cout << "mydeque contains:";
	for(it = mydeque.begin(); it != mydeque.end(); ++it)
		std::cout << ' ' << *it;
	std::cout << '\n';

	return  0;
} // deque::at 
#include <iostream> 
#include <deque> 

int  Test_deque__at()
{
	std::cout << "期望的结果:mydeque contains: 0 1 2 3 4 5 6 7 8 9\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::deque< unsigned > mydeque(10);    // 10 zero-initialized unsigneds 

	// assign some values: 
	for(unsigned i = 0; i < mydeque.size(); i++)
		mydeque.at(i) = i;

	std::cout << "mydeque contains:";
	for(unsigned i = 0; i < mydeque.size(); i++)
		std::cout << ' ' << mydeque.at(i);

	std::cout << '\n';

	return  0;
} // swap (deque overload) 
#include <iostream> 
#include <deque> 

void Test_deque__swapshortfree()
{
	std::cout << "期望的结果:foo contains: 200 200 200 200 200 \nbar contains: 100 100 100 \n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	unsigned   int  i;
	ztl::deque< int > foo(3, 100);    // three ints with a value of 100 
	ztl::deque< int > bar(5, 200);    // five ints with a value of 200 

	foo.swap(bar);

	std::cout << "foo contains:";
	for(ztl::deque< int >::iterator_type it = foo.begin(); it != foo.end(); ++it)
		std::cout << ' ' << *it;
	std::cout << '\n';

	std::cout << "bar contains:";
	for(ztl::deque< int >::iterator_type it = bar.begin(); it != bar.end(); ++it)
		std::cout << ' ' << *it;
	std::cout << '\n';

} // deque::begin 
#include <iostream> 
#include <deque> 

int  Test_deque__begin()
{
	std::cout << "期望的结果:mydeque contains: 1 2 3 4 5\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::deque< int > mydeque;

	for(int i = 1; i <= 5; i++) mydeque.push_back(i);

	std::cout << "mydeque contains:";

	ztl::deque< int >::iterator_type it = mydeque.begin();

	while(it != mydeque.end())
		std::cout << ' ' << *it++;

	std::cout << '\n';

	return  0;
} // deque::cbegin/cend 
#include <iostream> 
#include <deque> 

int  Test_deque__cbegin()
{
	std::cout << "期望的结果:mydeque contains: 10 20 30 40 50\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::deque< int > mydeque = {10, 20, 30, 40, 50};

	std::cout << "mydeque contains:";

	for(auto it = mydeque.cbegin(); it != mydeque.cend(); ++it)
		std::cout << ' ' << *it;

	std::cout << '\n';

	return  0;
} // deque::crbegin/crend 
#include <iostream> 
#include <deque> 

int  Test_deque__crbegin()
{
	std::cout << "期望的结果:mydeque backwards: 5 4 3 2 1\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::deque< int > mydeque = {1, 2, 3, 4, 5};

	std::cout << "mydeque backwards:";
	for(auto rit = mydeque.crbegin(); rit != mydeque.crend(); ++rit)
		std::cout << ' ' << *rit;
	std::cout << '\n';

	return  0;
} // deque::crbegin/crend 
#include <iostream> 
#include <deque> 

int  Test_deque__crend()
{
	std::cout << "期望的结果:mydeque backwards: 5 4 3 2 1\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::deque< int > mydeque = {1, 2, 3, 4, 5};

	std::cout << "mydeque backwards:";
	for(auto rit = mydeque.crbegin(); rit != mydeque.crend(); ++rit)
		std::cout << ' ' << *rit;
	std::cout << '\n';

	return  0;
} // constructing deques 
#include <iostream> 
#include <deque> 

int  Test_deque__deque()
{
	std::cout << "期望的结果:The contents of fifth are: 16 2 77 29 \n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	unsigned   int  i;

	// constructors used in the same order as described above: 
	ztl::deque< int > first;                                 // empty deque of ints 
	ztl::deque< int > second(4, 100);                        // four ints with value 100 
	ztl::deque< int > third(second.begin(), second.end());   // iterating through second 
	ztl::deque< int > fourth(third);                        // a copy of third 

	// the iterator_type constructor can be used to copy arrays: 
	int  myints[] = {16, 2, 77, 29};
	ztl::deque< int > fifth(myints, myints + sizeof (myints) / sizeof (int));

	std::cout << "The contents of fifth are:";
	for(ztl::deque< int >::iterator_type it = fifth.begin(); it != fifth.end(); ++it)
		std::cout << ' ' << *it;

	std::cout << '\n';

	return  0;
} // deque::emplace 
#include <iostream> 
#include <deque> 

int  Test_deque__emplace()
{
	std::cout << "期望的结果:mydeque contains: 10 200 100 20 30 300\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::deque< int > mydeque = {10, 20, 30};

	auto  it = mydeque.emplace(mydeque.begin() + 1, 100);
	mydeque.emplace(it, 200);
	mydeque.emplace(mydeque.end(), 300);

	std::cout << "mydeque contains:";
	for(auto & x : mydeque)
		std::cout << ' ' << x;
	std::cout << '\n';

	return  0;
} // deque::emplace_from 
#include <iostream> 
#include <deque> 

int  Test_deque__emplace_back()
{
	std::cout << "期望的结果:mydeque contains: 10 20 30 100 200\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::deque< int > mydeque = {10, 20, 30};

	mydeque.emplace_back(100);
	mydeque.emplace_back(200);

	std::cout << "mydeque contains:";
	for(auto & x : mydeque)
		std::cout << ' ' << x;
	std::cout << '\n';

	return  0;
} // deque::emplace_from 
#include <iostream> 
#include <deque> 

int  Test_deque__emplace_front()
{
	std::cout << "期望的结果:mydeque contains: 222 111 10 20 30\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::deque< int > mydeque = {10, 20, 30};

	mydeque.emplace_front(111);
	mydeque.emplace_front(222);

	std::cout << "mydeque contains:";
	for(auto & x : mydeque)
		std::cout << ' ' << x;
	std::cout << '\n';

	return  0;
} // resizing deque 
#include <iostream> 
#include <deque> 

int  Test_deque__resize()
{
	std::cout << "期望的结果:mydeque contains: 1 2 3 4 5 100 100 100 0 0 0 0\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::deque< int > mydeque;
	ztl::deque< int >::iterator_type it;

	// set some initial content: 
	for(int i = 1; i < 10; ++i) mydeque.push_back(i);

	mydeque.resize(5);
	mydeque.resize(8, 100);
	mydeque.resize(12);

	std::cout << "mydeque contains:";
	for(ztl::deque< int >::iterator_type it = mydeque.begin(); it != mydeque.end(); ++it)
		std::cout << ' ' << *it;
	std::cout << '\n';

	return  0;
} // deque::empty 
#include <iostream> 
#include <deque> 

int  Test_deque__empty()
{
	std::cout << "期望的结果:total: 55\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::deque< int > mydeque;
	int  sum(0);

	for(int i = 1; i <= 10; i++) mydeque.push_back(i);

	while(!mydeque.empty())
	{
		sum += mydeque.front();
		mydeque.pop_front();
	}

	std::cout << "total: " << sum << '\n';

	return  0;
} // deque::push_front 
#include <iostream> 
#include <deque> 

int  Test_deque__push_front()
{
	std::cout << "期望的结果:300 200 100 100 \n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::deque< int > mydeque(2, 100);      // two ints with a value of 100 
	mydeque.push_front(200);
	mydeque.push_front(300);

	std::cout << "mydeque contains:";
	for(ztl::deque< int >::iterator_type it = mydeque.begin(); it != mydeque.end(); ++it)
		std::cout << ' ' << *it;
	std::cout << '\n';

	return  0;
} // deque::end 
#include <iostream> 
#include <deque> 

int  Test_deque__end()
{
	std::cout << "期望的结果:mydeque contains: 1 2 3 4 5\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::deque< int > mydeque;

	for(int i = 1; i <= 5; i++) mydeque.insert(mydeque.end(), i);

	std::cout << "mydeque contains:";

	ztl::deque< int >::iterator_type it = mydeque.begin();

	while(it != mydeque.end())
		std::cout << ' ' << *it++;

	std::cout << '\n';

	return  0;
} // erasing from deque 
#include <iostream> 
#include <deque> 

int  Test_deque__erase()
{
	std::cout << "期望的结果:mydeque contains: 4 5 7 8 9 10\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::deque< int > mydeque;

	// set some values (from 1 to 10) 
	for(int i = 1; i <= 10; i++) mydeque.push_back(i);

	// erase the 6th element 
	mydeque.erase(mydeque.begin() + 5);

	// erase the first 3 elements: 
	mydeque.erase(mydeque.begin(), mydeque.begin() + 3);

	std::cout << "mydeque contains:";
	for(ztl::deque< int >::iterator_type it = mydeque.begin(); it != mydeque.end(); ++it)
		std::cout << ' ' << *it;
	std::cout << '\n';

	return  0;
} // deque::push_back 
#include <iostream> 
#include <deque> 

int  Test_deque__push_back()
{
	std::cout << "期望的结果:" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::deque< int > mydeque;
	int  myint;

	std::cout << "Please enter some integers (enter 0 to end):\n";

	do
	{
		std::cin >> myint;
		mydeque.push_back(myint);
	}
	while(myint);

	std::cout << "mydeque stores " << (int)mydeque.size() << " numbers.\n";

	return  0;
} // deque::front 
#include <iostream> 
#include <deque> 

int  Test_deque__front()
{
	std::cout << "期望的结果:mydeque.front() is now 57\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::deque< int > mydeque;

	mydeque.push_front(77);
	mydeque.push_back(20);

	mydeque.front() -= mydeque.back();

	std::cout << "mydeque.front() is now " << mydeque.front() << '\n';

	return  0;
} // deque::get_allocator 
#include <iostream> 
#include <deque> 

int  Test_deque__get_allocator()
{
	//std::cout << "期望的结果:The allocated array contains: 0 1 2 3 4\n" << std::endl;
	//std::cout << "下面是测试结果:" << std::endl;

	//ztl::deque< int > mydeque;
	//int  * p;
	//unsigned   int  i;

	//// allocate an array with space for 5 elements using deque's allocator: 
	//p = mydeque.get_allocator().allocate(5);

	//// construct values in-place on the array: 
	//for(i = 0; i < 5; i++) mydeque.get_allocator().construct(&p[i], i);

	//std::cout << "The allocated array contains:";
	//for(i = 0; i < 5; i++) std::cout << ' ' << p[i];
	//std::cout << '\n';

	//// destroy and deallocate: 
	//for(i = 0; i < 5; i++) mydeque.get_allocator().destroy(&p[i]);
	//mydeque.get_allocator().deallocate(p, 5);

	return  0;
} // deque::max_size 
#include <iostream> 
#include <deque> 

int  Test_deque__max_size()
{
	std::cout << "期望的结果:" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	unsigned   int  i;
	ztl::deque< int > mydeque;

	std::cout << "Enter number of elements: ";
	std::cin >> i;

	if(i < mydeque.max_size()) mydeque.resize(i);
	else  std::cout << "That size exceeds the limit.\n";

	return  0;
} // assignment operator with deques 
#include <iostream> 
#include <deque> 

int  Test_deque__operatorequal()
{
	std::cout << "期望的结果:Size of first: 0\nSize of second: 3\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::deque< int > first(3);     // deque with 3 zero-initialized ints 
	ztl::deque< int > second(5);    // deque with 5 zero-initialized ints 

	second = first;
	first = ztl::deque< int >();

	std::cout << "Size of first: " << int(first.size()) << '\n';
	std::cout << "Size of second: " << int(second.size()) << '\n';
	return  0;
} // deque comparisons 
#include <iostream> 
#include <deque> 

int  Test_deque__operators()
{
	//std::cout << "期望的结果:foo and bar are not equal\nfoo is less than bar\nfoo is less than or equal to bar\n" << std::endl;
	//std::cout << "下面是测试结果:" << std::endl;

	//ztl::deque< int > foo(3, 100);    // three ints with a value of 100 
	//ztl::deque< int > bar(2, 200);    // two ints with a value of 200 

	//if(foo == bar) std::cout << "foo and bar are equal\n";
	//if(foo != bar) std::cout << "foo and bar are not equal\n";
	//if(foo< bar) std::cout << "foo is less than bar\n";
	//if(foo> bar) std::cout << "foo is greater than bar\n";
	//if(foo <= bar) std::cout << "foo is less than or equal to bar\n";
	//if(foo >= bar) std::cout << "foo is greater than or equal to bar\n";

	return  0;
} // deque::operator[] example: reversing order 
#include <iostream> 
#include <deque> 

int  Test_deque__operatorse()
{
	std::cout << "期望的结果:mydeque contains: 9 8 7 6 5 4 3 2 1 0\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::deque< int > mydeque(10);    // 10 zero-initialized elements 
	ztl::deque< int >::size_type sz = mydeque.size();

	// assign some values: 
	for(unsigned i = 0; i < sz; i++) mydeque[i] = i;

	// reverse order of elements using operator[]: 
	for(unsigned i = 0; i < sz / 2; i++)
	{
		int  temp;
		temp = mydeque[sz - 1 - i];
		mydeque[sz - 1 - i] = mydeque[i];
		mydeque[i] = temp;
	}

	// print content: 
	std::cout << "mydeque contains:";
	for(unsigned i = 0; i < sz; i++)
		std::cout << ' ' << mydeque[i];
	std::cout << '\n';

	return  0;
} // deque::pop_back 
#include <iostream> 
#include <deque> 

int  Test_deque__pop_back()
{
	std::cout << "期望的结果:The elements of mydeque summed 60\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::deque< int > mydeque;
	int  sum(0);
	mydeque.push_back(10);
	mydeque.push_back(20);
	mydeque.push_back(30);

	while(!mydeque.empty())
	{
		sum += mydeque.back();
		mydeque.pop_back();
	}

	std::cout << "The elements of mydeque add up to " << sum << '\n';

	return  0;
} // deque::pop_front 
#include <iostream> 
#include <deque> 

int  Test_deque__pop_front()
{
	std::cout << "期望的结果:Popping out the elements in mydeque: 100 200 300\nThe final size of mydeque is 0\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::deque< int > mydeque;

	mydeque.push_back(100);
	mydeque.push_back(200);
	mydeque.push_back(300);

	std::cout << "Popping out the elements in mydeque:";
	while(!mydeque.empty())
	{
		std::cout << ' ' << mydeque.front();
		mydeque.pop_front();
	}

	std::cout << "\nThe final size of mydeque is " << int(mydeque.size()) << '\n';

	return  0;
} // deque::rbegin/rend 
#include <iostream> 
#include <deque> 

int  Test_deque__rbegin()
{
	std::cout << "期望的结果:mydeque contains: 5 4 3 2 1\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::deque< int > mydeque(5);   // 5 default-constructed ints 

	ztl::deque< int >::reverse_iterator_type rit = mydeque.rbegin();

	int  i = 0;
	for(rit = mydeque.rbegin(); rit != mydeque.rend(); ++rit)
		*rit = ++i;

	std::cout << "mydeque contains:";
	for(ztl::deque< int >::iterator_type it = mydeque.begin(); it != mydeque.end(); ++it)
		std::cout << ' ' << *it;
	std::cout << '\n';

	return  0;
} // deque::rbegin/rend 
#include <iostream> 
#include <deque> 

int  Test_deque__rend()
{
	std::cout << "期望的结果:5 4 3 2 1 \n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::deque< int > mydeque(5);   // 5 default-constructed ints 

	ztl::deque< int >::reverse_iterator_type rit = mydeque.rbegin();

	int  i = 0;
	for(rit = mydeque.rbegin(); rit != mydeque.rend(); ++rit)
		*rit = ++i;

	std::cout << "mydeque contains:";
	for(ztl::deque< int >::iterator_type it = mydeque.begin(); it != mydeque.end(); ++it)
		std::cout << ' ' << *it;
	std::cout << '\n';

	return  0;
} // deque::shrink_to_fit 
#include <iostream> 
#include <deque> 

int  Test_deque__shrink_to_fit()
{
	std::cout << "期望的结果:1. size of mydeque: 100\n2. size of mydeque: 10\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::deque< int > mydeque(100);
	std::cout << "1. size of mydeque: " << mydeque.size() << '\n';

	mydeque.resize(10);
	std::cout << "2. size of mydeque: " << mydeque.size() << '\n';

	mydeque.shrink_to_fit();

	return  0;
} // deque::size 
#include <iostream> 
#include <deque> 

int  Test_deque__size()
{
	std::cout << "期望的结果:0. size: 0\n1. size: 5\n2. size: 10\n3. size: 9\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::deque< int > myints;
	std::cout << "0. size: " << myints.size() << '\n';

	for(int i = 0; i < 5; i++) myints.push_back(i);
	std::cout << "1. size: " << myints.size() << '\n';

	myints.insert(myints.begin(), 5, 100);
	std::cout << "2. size: " << myints.size() << '\n';

	myints.pop_back();
	std::cout << "3. size: " << myints.size() << '\n';

	return  0;
} // swap deques 
#include <iostream> 
#include <deque> 

void Test_deque__swap()
{
	std::cout << "期望的结果:foo contains: 200 200 200 200 200 \nbar contains: 100 100 100 \n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	unsigned   int  i;
	ztl::deque< int > foo(3, 100);    // three ints with a value of 100 
	ztl::deque< int > bar(5, 200);    // five ints with a value of 200 

	foo.swap(bar);

	std::cout << "foo contains:";
	for(ztl::deque< int >::iterator_type it = foo.begin(); it != foo.end(); ++it)
		std::cout << ' ' << *it;
	std::cout << '\n';

	std::cout << "bar contains:";
	for(ztl::deque< int >::iterator_type it = bar.begin(); it != bar.end(); ++it)
		std::cout << ' ' << *it;
	std::cout << '\n';

}void test_deque()
{
	Test_deque__back();
	Test_deque__assign();
	Test_deque__clear();
	Test_deque__cend();
	Test_deque__insert();
	Test_deque__at();
	Test_deque__swapshortfree();
	Test_deque__begin();
	Test_deque__cbegin();
	Test_deque__crbegin();
	Test_deque__crend();
	Test_deque__deque();
	Test_deque__emplace();
	Test_deque__emplace_back();
	Test_deque__emplace_front();
	Test_deque__resize();
	Test_deque__empty();
	Test_deque__push_front();
	Test_deque__end();
	Test_deque__erase();
	Test_deque__push_back();
	Test_deque__front();
	Test_deque__get_allocator();
	Test_deque__max_size();
	Test_deque__operatorequal();
	Test_deque__operators();
	Test_deque__operatorse();
	Test_deque__pop_back();
	Test_deque__pop_front();
	Test_deque__rbegin();
	Test_deque__rend();
	Test_deque__shrink_to_fit();
	Test_deque__size();
	Test_deque__swap();
};