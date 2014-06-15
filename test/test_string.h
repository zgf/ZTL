#pragma once
// appending to string 
#include <iostream> 
#include <string> 

int  Test_basic_string__append()
{
	std::cout << "期望的结果:Writing 10 dots here: .......... and then 5 more.....\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string str;
	ztl::string str2 = "Writing ";
	ztl::string str3 = "print 10 and then 5 more";

	// used in the same order as described above: 
	str.append(str2);                        // "Writing " 
	str.append(str3, 6, 3);                    // "10 " 
	str.append("dots are cool", 5);           // "dots " 
	str.append("here: ");                    // "here: " 
	str.append(10u, '.');                     // ".........." 
	str.append(str3.begin() + 8, str3.end());   // " and then 5 more" 
	str.append(5, 0x2E);                 // "....." 

	std::cout << str << '\n';
	return  0;
} // string::back 
#include <iostream> 
#include <string> 

int  Test_basic_string__back()
{
	std::cout << "期望的结果:hello world!\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string str("hello world.");
	std::cout << str << '\n';

	str.back() = '!';
	std::cout << str << '\n';
	return  0;
} // string::operator+= 
#include <iostream> 
#include <string> 

int  Test_string__operatoraddequal()
{
	std::cout << "期望的结果:John K. Smith\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string name("John");
	ztl::string family("Smith");
	name += " K. ";          // c-string 
	name += family;          // string 
	name += '\n';            // character 

	std::cout << name;
	return  0;
} // string::assign 
#include <iostream> 
#include <string> 

int  Test_basic_string__assign()
{
	std::cout << "期望的结果:The quick brown fox jumps over a lazy dog.\nbrown fox\npangram\nc-string\n**********\n----------\nfox jumps over\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string str;
	ztl::string base = "The quick brown fox jumps over a lazy dog.";

	// used in the same order as described above: 

	str.assign(base);
	std::cout << str << '\n';

	str.assign(base, 10, 9);
	std::cout << str << '\n';          // "brown fox" 

	str.assign("pangrams are cool", 7);
	std::cout << str << '\n';          // "pangram" 

	str.assign("c-string");
	std::cout << str << '\n';          // "c-string" 

	str.assign(10, '*');
	std::cout << str << '\n';          // "**********" 

	str.assign(10, 0x2D);
	std::cout << str << '\n';          // "----------" 

	str.assign(base.begin() + 16, base.end() - 12);
	std::cout << str << '\n';          // "fox jumps over" 

	return  0;
} // inserting into a string 
#include <iostream> 
#include <string> 

int  Test_basic_string__insert()
{
	std::cout << "期望的结果:to be, or not to be: that is the question...\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string str = "to be question";
	ztl::string str2 = "the ";
	ztl::string str3 = "or not to be";
	ztl::string::iterator_type it;
	// used in the same order as described above: 
	str.insert(6, str2);                  // to be (the )question 
	str.insert(6, str3, 3, 4);              // to be (not )the question 
	str.insert(10, "that is cool", 8);     // to be not (that is )the question 
	str.insert(10, "to be ");             // to be not (to be )that is the question 
	str.insert(15, 1, ':');                // to be not to be(:) that is the question 
	it = str.insert(str.begin() + 5, ',');  // to be(,) not to be: that is the question 
	str.insert(str.end(), 3, '.');        // to be, not to be: that is the question(...) 
	str.insert(it + 2, str3.begin(), str3.begin() + 3);  // (or ) 

	std::cout << str << '\n';
	return  0;
} // string::at 
#include <iostream> 
#include <string> 

int  Test_basic_string__at()
{
	std::cout << "期望的结果:Test string\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string str("Test string");
	for(unsigned i = 0; i < str.length(); ++i)
	{
		std::cout << str.at(i);
	}
	return  0;
} // string::rbegin/rend 
#include <iostream> 
#include <string> 
int  Test_basic_string__rend()
{
	std::cout << "期望的结果:...evil pets won\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string str("now step live...");
	for(ztl::string::reverse_iterator_type rit = str.rbegin(); rit != str.rend(); ++rit)
		std::cout << *rit;
	return  0;
} // string constructor 
#include <iostream> 
#include <string> 

int  Test_basic_string__basic_string()
{
	std::cout << "期望的结果:s1: \ns2: Initial string\ns3: str\ns4: A char\ns5: Another character sequence\ns6: xxxxxxxxxx\ns7a: **********\ns7b: Initial\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string s0("Initial string");

	// constructors used in the same order as described above: 
	ztl::string s1;
	ztl::string s2(s0);
	ztl::string s3(s0, 8, 3);
	ztl::string s4("A character sequence", 6);
	ztl::string s5("Another character sequence");
	ztl::string s6(10, 'x');
	ztl::string s7a(10, 42);
	ztl::string s7b(s0.begin(), s0.begin() + 7);

	std::cout << "s1: " << s1 << "\ns2: " << s2 << "\ns3: " << s3;
	std::cout << "\ns4: " << s4 << "\ns5: " << s5 << "\ns6: " << s6;
	std::cout << "\ns7a: " << s7a << "\ns7b: " << s7b << '\n';
	return  0;
} // string::cbegin/cend 
#include <iostream> 
#include <string> 

int  Test_basic_string__cbegin()
{
	std::cout << "期望的结果:Lorem ipsum\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string str("Lorem ipsum");
	for(auto it = str.cbegin(); it != str.cend(); ++it)
		std::cout << *it;
	std::cout << '\n';

	return  0;
} // concatenating strings 
#include <iostream> 
#include <string> 

int Test_basic_string__operatoraddequal()
{
	std::cout << "期望的结果:http://www.cplusplus.com\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string firstlevel("com");
	ztl::string secondlevel("cplusplus");
	ztl::string scheme("http://");
	ztl::string hostname;
	ztl::string url;

	hostname = "www." + secondlevel + '.' + firstlevel;
	url = scheme + hostname;

	std::cout << url << '\n';

	return  0;
} // comparing size, length, capacity and max_size 
#include <iostream> 
#include <string> 

int  Test_basic_string__capacity()
{
	std::cout << "期望的结果:size: 11\nlength: 11\ncapacity: 15\nmax_size: 429496729\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string str("Test string");
	std::cout << "size: " << str.size() << "\n";
	std::cout << "length: " << str.length() << "\n";
	std::cout << "capacity: " << str.capacity() << "\n";
	std::cout << "max_size: " << str.max_size() << "\n";
	return  0;
} // swap strings 
#include <iostream> 
#include <string> 

int Test_basic_string__swapshortfree()
{
	std::cout << "期望的结果:Before the swap, buyer has money and seller has goods\n After the swap, buyer has goods and seller has money\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string buyer("money");
	ztl::string seller("goods");

	std::cout << "Before the swap, buyer has " << buyer;
	std::cout << " and seller has " << seller << '\n';

	swap(buyer, seller);

	std::cout << " After the swap, buyer has " << buyer;
	std::cout << " and seller has " << seller << '\n';

	return  0;
} // string::rfind 
#include <iostream> 
#include <string> 

int  Test_basic_string__rfind()
{
	std::cout << "期望的结果:The sixth sick sheik's seventh sheep's sick.\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string str("The sixth sick sheik's sixth sheep's sick.");
	ztl::string key("sixth");

	ztl::string::size_type found = str.rfind(key);
	if(found != -1)
		str.replace(found, key.length(), "seventh");

	std::cout << str << '\n';

	return  0;
} // string::begin/end 
#include <iostream> 
#include <string> 

int  Test_basic_string__begin()
{
	std::cout << "期望的结果:Test string\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string str("Test string");
	for(ztl::string::iterator_type it = str.begin(); it != str.end(); ++it)
		std::cout << *it;
	std::cout << '\n';

	return  0;
} // string::cbegin/cend 
#include <iostream> 
#include <string> 

int  Test_basic_string__cend()
{
	std::cout << "期望的结果:Lorem ipsum\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string str("Lorem ipsum");
	for(auto it = str.cbegin(); it != str.cend(); ++it)
		std::cout << *it;
	std::cout << '\n';

	return  0;
} // string::clear 
#include <iostream> 
#include <string> 

int  Test_basic_string__clear()
{
	std::cout << "期望的结果:" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	char  c;
	ztl::string str;
	std::cout << "Please type some lines of text. Enter a dot (.) to finish:\n";
	do
	{
		c = std::cin.get();
		str += c;
		if(c == '\n')
		{
			std::cout << str;
			str.clear();
		}
	}
	while(c != '.');
	return  0;
} // comparing apples with apples 
#include <iostream> 
#include <string> 

int  Test_basic_string__compare()
{
	std::cout << "期望的结果:green apple is not red apple\nstill, green apple is an apple\nand red apple is also an apple\ntherefore, both are apples\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string str1("green apple");
	ztl::string str2("red apple");

	if(str1.compare(str2) != 0)
		std::cout << str1 << " is not " << str2 << '\n';

	if(str1.compare(6, 5, "apple") == 0)
		std::cout << "still, " << str1 << " is an apple\n";

	if(str2.compare(str2.size() - 5, 5, "apple") == 0)
		std::cout << "and " << str2 << " is also an apple\n";

	if(str1.compare(6, 5, str2, 4, 5) == 0)
		std::cout << "therefore, both are apples\n";

	return  0;
} // string::copy 
#include <iostream> 
#include <string> 

int  Test_basic_string__copy()
{
	std::cout << "期望的结果:buffer contains: string\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	char  buffer[20];
	ztl::string str("Test string...");
	std::size_t length = str.copy(buffer, 6, 5);
	buffer[length] = '\0';
	std::cout << "buffer contains: " << buffer << '\n';
	return  0;
} // string::crbegin/crend 
#include <iostream> 
#include <string> 

int  Test_basic_string__crbegin()
{
	std::cout << "期望的结果:muspi merol\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string str("lorem ipsum");
	for(auto rit = str.crbegin(); rit != str.crend(); ++rit)
		std::cout << *rit;
	std::cout << '\n';

	return  0;
} // string::find_first_not_of 
#include <iostream> 
#include <string> 

int  Test_basic_string__find_first_not_of()
{
	std::cout << "期望的结果:The first non-alphabetic character is - at position 12\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string str("look for non-alphabetic characters...");

	ztl::string::size_type found = str.find_first_not_of("abcdefghijklmnopqrstuvwxyz ");

	if(found != -1)
	{
		std::cout << "The first non-alphabetic character is " << str[found];
		std::cout << " at position " << found << '\n';
	}

	return  0;
} // string::crbegin/crend 
#include <iostream> 
#include <string> 

int  Test_basic_string__crend()
{
	std::cout << "期望的结果:muspi merol\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string str("lorem ipsum");
	for(auto rit = str.crbegin(); rit != str.crend(); ++rit)
		std::cout << *rit;
	std::cout << '\n';

	return  0;
} // strings and c-strings 
#include <iostream> 
#include <cstring> 
#include <string> 

int  Test_basic_string__c_str()
{
	std::cout << "期望的结果:Please\nsplit\nthis\nsentence\ninto\ntokens\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string str("Please split this sentence into tokens");

	char  * cstr = new   char[str.length() + 1];
	std::strcpy(cstr, str.c_str());

	// cstr now contains a c-string copy of str 

	char  * p = std::strtok(cstr, " ");
	while(p != 0)
	{
		std::cout << p << '\n';
		p = strtok(NULL, " ");
	}

	delete[] cstr;
	return  0;
} // string::find 
#include <iostream> 
#include <string> 

int  Test_basic_string__find()
{
	std::cout << "期望的结果:first 'needle' found at: 14\nsecond 'needle' found at: 44\n'haystack' also found at: 30\nPeriod found at: 51\nThere are two prepositions in this haystack with needles.\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string str("There are two needles in this haystack with needles.");
	ztl::string str2("needle");

	// different member versions of find in the same order as above: 
	ztl::string::size_type found = str.find(str2);
	if(found != -1)
		std::cout << "first 'needle' found at: " << found << '\n';

	found = str.find("needles are small", found + 1, 6);
	if(found != -1)
		std::cout << "second 'needle' found at: " << found << '\n';

	found = str.find("haystack");
	if(found != -1)
		std::cout << "'haystack' also found at: " << found << '\n';

	found = str.find('.');
	if(found != -1)
		std::cout << "Period found at: " << found << '\n';

	// let's replace the first needle: 
	str.replace(str.find(str2), str2.length(), "preposition");
	std::cout << str << '\n';

	return  0;
} // string::data 
#include <iostream> 
#include <string> 
#include <cstring> 

int  Test_basic_string__data()
{
	std::cout << "期望的结果:str and cstr have the same length.\nstr and cstr have the same content.\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;


	ztl::string str = "Test string";
	char * cstr = "Test string";

	if(str.length() == std::strlen(cstr))
	{
		std::cout << "str and cstr have the same length.\n";

		if(memcmp(cstr, str.data(), str.length()) == 0)
			std::cout << "str and cstr have the same content.\n";
	}
	return  0;
} // resizing string 
#include <iostream> 
#include <string> 

int  Test_basic_string__resize()
{
	std::cout << "期望的结果:I like to code in C\nI like to code in C++\nI like to code\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string str("I like to code in C");
	std::cout << str << '\n';

	ztl::string::size_type sz = str.size();

	str.resize(sz + 2, '+');
	std::cout << str << '\n';

	str.resize(14);
	std::cout << str << '\n';
	return  0;
} // string::empty 
#include <iostream> 
#include <string> 

int  Test_basic_string__empty()
{
	std::cout << "期望的结果:" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string content;
	ztl::string line;
	std::cout << "Please introduce a text. Enter an empty line to finish:\n";
	do
	{
		line = "11";
		//getline(std::cin, line);
		content += line + '\n';
	}
	while(line.empty());
	std::cout << "The text you introduced was:\n" << content;
	return  0;
} // string::begin/end 
#include <iostream> 
#include <string> 

int  Test_basic_string__end()
{
	std::cout << "期望的结果:Test string\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string str("Test string");
	for(ztl::string::iterator_type it = str.begin(); it != str.end(); ++it)
		std::cout << *it;
	std::cout << '\n';

	return  0;
} // string::erase 
#include <iostream> 
#include <string> 

int  Test_basic_string__erase()
{
	std::cout << "期望的结果:This is an example sentence.\nThis is an sentence.\nThis is a sentence.\nThis sentence.\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string str("This is an example sentence.");
	std::cout << str << '\n';
	// "This is an example sentence." 
	str.erase(10, 8);                         //            ^^^^^^^^ 
	std::cout << str << '\n';
	// "This is an sentence." 
	str.erase(str.begin() + 9);                //           ^ 
	std::cout << str << '\n';
	// "This is a sentence." 
	str.erase(str.begin() + 5, str.end() - 9);   //       ^^^^^ 
	std::cout << str << '\n';
	// "This sentence." 
	return  0;
} // swap strings 
#include <iostream> 
#include <string> 

int Test_basic_string__swap()
{
	std::cout << "期望的结果:Before the swap, buyer has money and seller has goods\n After the swap, buyer has goods and seller has money\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string buyer("money");
	ztl::string seller("goods");

	std::cout << "Before the swap, buyer has " << buyer;
	std::cout << " and seller has " << seller << '\n';

	seller.swap(buyer);

	std::cout << " After the swap, buyer has " << buyer;
	std::cout << " and seller has " << seller << '\n';

	return  0;
} // string::find_first_of 
#include <iostream> 
#include <string> 

int  Test_basic_string__find_first_of()
{
	std::cout << "期望的结果:Pl**s*, r*pl*c* th* v*w*ls *n th*s s*nt*nc* by *st*r*sks.\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string str("PLease, replace the vowels in this sentence by asterisks.");
	ztl::string::size_type found = str.find_first_of("aeiou");
	while(found != -1)
	{
		str[found] = '*';
		found = str.find_first_of("aeiou", found + 1);
	}

	std::cout << str << '\n';

	return  0;
} // string::find_last_not_of 
#include <iostream> 
#include <string> 

int  Test_basic_string__find_last_not_of()
{
	std::cout << "期望的结果:[Please, erase trailing white-spaces]\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string str("Please, erase trailing white-spaces   \n");
	ztl::string whitespaces(" \t\f\v\n\r");

	ztl::string::size_type found = str.find_last_not_of(whitespaces);
	if(found != -1)
		str.erase(found + 1);
	else
		str.clear();             // str is all whitespace 

	std::cout << '[' << str << "]\n";

	return  0;
} // extract to string 
#include <iostream> 
#include <string> 

int Test_basic_string__getline()
{
	/*std::cout << "期望的结果:" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string name;

	std::cout << "Please, enter your full name: ";
	std::getline(std::cin, name);
	std::cout << "Hello, " << name << "!\n";*/

	return  0;
} // string::find_last_of 
#include <iostream> 
#include <string> 

void  SplitFilename(const  ztl::string& str)
{
	std::cout << "Splitting: " << str << '\n';
	ztl::string::size_type found = str.find_last_of("/\\");
	std::cout << " path: " << str.substr(0, found) << '\n';
	std::cout << " file: " << str.substr(found + 1) << '\n';
}

int  Test_basic_string__find_last_of()
{
	std::cout << "期望的结果:Splitting: /usr/bin/man\n path: /usr/bin\n file: man\nSplitting: c:/windows/winhelp.exe\n path: c:/windows\n file: winhelp.exe\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string str1("/usr/bin/man");
	ztl::string str2("c:\\windows\\winhelp.exe");

	SplitFilename(str1);
	SplitFilename(str2);

	return  0;
} // string::reserve 
#include <iostream> 
#include <fstream> 
#include <string> 

int  Test_basic_string__reserve()
{
	std::cout << "期望的结果:" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string str;

	std::ifstream file("test.txt", std::ios::in | std::ios::ate);
	if(file)
	{
		std::ifstream::streampos filesize = file.tellg();
		str.reserve(filesize);

		file.seekg(0);
		while(!file.eof())
		{
			str += file.get();
		}
		std::cout << str;
	}
	return  0;
} // string::push_back 
#include <iostream> 
#include <fstream> 
#include <string> 

int  Test_basic_string__push_back()
{
	std::cout << "期望的结果:" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string str;
	std::ifstream file("test.txt", std::ios::in);
	if(file)
	{
		while(!file.eof()) str.push_back(file.get());
	}
	std::cout << str << '\n';
	return  0;
} // string::front 
#include <iostream> 
#include <string> 

int  Test_basic_string__front()
{
	std::cout << "期望的结果:Test string\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string str("test string");
	str.front() = 'T';
	std::cout << str << '\n';
	return  0;
} // string::length 
#include <iostream> 
#include <string> 

int  Test_basic_string__length()
{
	std::cout << "期望的结果:The size of str is 11 characters\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string str("Test string");
	std::cout << "The size of str is " << str.length() << " characters.\n";
	return  0;
} // comparing size, length, capacity and max_size 
#include <iostream> 
#include <string> 

int  Test_basic_string__max_size()
{
	std::cout << "期望的结果:size: 11\nlength: 11\ncapacity: 15\nmax_size: 4294967291\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string str("Test string");
	std::cout << "size: " << str.size() << "\n";
	std::cout << "length: " << str.length() << "\n";
	std::cout << "capacity: " << str.capacity() << "\n";
	std::cout << "max_size: " << str.max_size() << "\n";
	return  0;
} // string assigning 
#include <iostream> 
#include <string> 

int  Test_basic_string__operatorequal()
{
	std::cout << "期望的结果:Test string: x\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string str1, str2, str3;
	str1 = "Test string: ";    // c-string 
	str2 = 'x';                // single character 
	str3 = str1 + str2;        // string 

	std::cout << str3 << '\n';
	return  0;
} // string comparisons 
#include <iostream> 
#include <vector> 

int  Test_basic_string__operators()
{
	std::cout << "期望的结果:foo and bar are not equal\nfoo is less than bar\nfoo is less than or equal to bar\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string foo = "alpha";
	ztl::string bar = "beta";

	if(foo == bar) std::cout << "foo and bar are equal\n";
	if(foo != bar) std::cout << "foo and bar are not equal\n";
	if(foo< bar) std::cout << "foo is less than bar\n";
	if(foo> bar) std::cout << "foo is greater than bar\n";
	if(foo <= bar) std::cout << "foo is less than or equal to bar\n";
	if(foo >= bar) std::cout << "foo is greater than or equal to bar\n";

	return  0;
} // string::operator[] 
#include <iostream> 
#include <string> 

int  Test_basic_string__operatorse()
{
	std::cout << "期望的结果:Test string\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string str("Test string");
	for(int i = 0; i < str.length(); ++i)
	{
		std::cout << str[i];
	}
	return  0;
} // inserting strings into output streams 
#include <iostream> 
#include <string> 

int Test_basic_string__operator__()
{
	std::cout << "期望的结果:" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string str = "Hello world!";
	std::cout << str << '\n';
	return  0;
} // string::pop_back 
#include <iostream> 
#include <string> 

int  Test_basic_string__pop_back()
{
	std::cout << "期望的结果:hello world\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string str("hello world!");
	str.pop_back();
	std::cout << str << '\n';
	return  0;
} // string::rbegin/rend 
#include <iostream> 
#include <string> 

int  Test_basic_string__rbegin()
{
	std::cout << "期望的结果:...evil pets won\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string str("now step live...");
	for(ztl::string::reverse_iterator_type rit = str.rbegin(); rit != str.rend(); ++rit)
		std::cout << *rit;
	return  0;
} // replacing in a string 
#include <iostream> 
#include <string> 

int  Test_basic_string__replace()
{
	std::cout << "期望的结果:replace is useful.\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string base = "this is a test string.";
	ztl::string str2 = "n example";
	ztl::string str3 = "sample phrase";
	ztl::string str4 = "useful.";

	// replace signatures used in the same order as described above: 

	// Using positions:                 0123456789*123456789*12345 
	ztl::string str = base;            // "this is a test string." 
	str.replace(9, 5, str2);           // "this is an example string." (1) 
	str.replace(19, 6, str3, 7, 6);      // "this is an example phrase." (2) 
	str.replace(8, 10, "just a");      // "this is just a phrase."     (3) 
	str.replace(8, 6, "a shorty", 7);   // "this is a short phrase."    (4) 
	str.replace(22, 1, 3, '!');         // "this is a short phrase!!!"  (5) 

	// Using iterators:                                               0123456789*123456789* 
	str.replace(str.begin(), str.end() - 3, str3);                     // "sample phrase!!!"      (1) 
	str.replace(str.begin(), str.begin() + 6, "replace");              // "replace phrase!!!"     (3) 
	str.replace(str.begin() + 8, str.begin() + 14, "is coolness", 7);     // "replace is cool!!!"    (4) 
	str.replace(str.begin() + 12, str.end() - 4, 4, 'o');                 // "replace is cooool!!!"  (5) 
	str.replace(str.begin() + 11, str.end(), str4.begin(), str4.end()); // "replace is useful."    (6) 
	std::cout << str << '\n';
	return  0;
} // string::shrink_to_fit 
#include <iostream> 
#include <string> 

int  Test_basic_string__shrink_to_fit()
{
	std::cout << "期望的结果:1. capacity of str: 100\n2. capacity of str: 100\n3. capacity of str: 10\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string str(100, 'x');
	std::cout << "1. capacity of str: " << str.capacity() << '\n';

	str.resize(10);
	std::cout << "2. capacity of str: " << str.capacity() << '\n';

	str.shrink_to_fit();
	std::cout << "3. capacity of str: " << str.capacity() << '\n';

	return  0;
} // string::size 
#include <iostream> 
#include <string> 

int  Test_basic_string__size()
{
	std::cout << "期望的结果:The size of str is 11 characters\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string str("Test string");
	std::cout << "The size of str is " << str.size() << " characters.\n";
	return  0;
} // string::substr 
#include <iostream> 
#include <string> 

int  Test_basic_string__substr()
{
	std::cout << "期望的结果:generalities live in details.\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string str = "We think in generalities, but we live in details.";
	// (quoting Alfred N. Whitehead) 

	ztl::string str2 = str.substr(12, 12);          // "generalities" 

	ztl::string::size_type pos = str.find("live");  // position of "live" in str 

	ztl::string str3 = str.substr(pos);            // get from "live" to the end 

	std::cout << str2 << ' ' << str3 << '\n';

	return  0;
} // char_traits::eof 
#include <iostream>     // std::wcin, std::wcout 
#include <string>       // std::wstring, std::char_traits 

int  Test_char_traits__eof()
{
	std::cout << "期望的结果:" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	std::wcout << "Please enter some text: ";

	if(std::wcin.peek() == std::char_traits< wchar_t >::eof())
	{
		std::wcout << "Reading error";
	}
	else
	{
		std::wstring ws;
		std::getline(std::wcin, ws);
		std::wcout << "You entered a word: " << ws << '\n';
	}

	return  0;
} // char_traits::eq 
#include <iostream>     // std::cout 
#include <string>       // std::basic_string, std::char_traits 
#include <cctype>       // std::tolower 
#include <cstddef>      // std::size_t 

// traits with case-insensitive eq: 
struct  custom_traits : std::char_traits< char >
{
	static   bool  eq(char  c, char  d)
	{
		return  std::tolower(c) == std::tolower(d);
	}
	// some (non-conforming) implementations of basic_string::find call this instead of eq: 
	static   const   char * find(const   char * s, std::size_t n, char  c)
	{
		while(n-- && (!eq(*s, c))) ++s;  return  s;
	}
};

int  Test_char_traits__eq()
{
	std::cout << "期望的结果:T found at position 0\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	std::basic_string< char, custom_traits> str("Test");
	std::cout << "T found at position " << str.find('T') << '\n';
	return  0;
} // char_traits::compare 
#include <iostream>     // std::cout 
#include <string>       // std::basic_string, std::char_traits 
#include <cctype>       // std::tolower 
#include <cstddef>      // std::size_t 

// case-insensitive traits: 
struct  customs_traits : std::char_traits< char >
{
	static   bool  eq(char  c, char  d)
	{
		return  std::tolower(c) == std::tolower(d);
	}
	static   bool  lt(char  c, char  d)
	{
		return  std::tolower(c) < std::tolower(d);
	}
	static   int  compare(const   char * p, const   char * q, std::size_t n)
	{
		while(n--)
		{
			if(!eq(*p, *q))  return  lt(*p, *q); ++p; ++q;
		}
		return  0;
	}
};

int  Test_char_traits__compare()
{
	std::cout << "期望的结果:foo and bar are equal\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	std::basic_string< char, customs_traits> foo, bar;
	foo = "Test";
	bar = "test";
	if(foo == bar) std::cout << "foo and bar are equal\n";
	return  0;
} // char_traits::copy 
#include <iostream>     // std::cout 
#include <string>       // std::char_traits 

int  Test_char_traits__copy()
{
	std::cout << "期望的结果:foo contains: test string\nbar contains: test string\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	char  foo[] = "test string";
	char  bar[20];

	auto  len = std::char_traits< char >::length(foo);
	std::char_traits< char >::copy(bar, foo, len);

	bar[len] = '\0';  // append null-character 

	std::cout << "foo contains: " << foo << '\n';
	std::cout << "bar contains: " << bar << '\n';
	return  0;
} // char_traits::length 
#include <iostream>     // std::cout 
#include <string>       // std::char_traits 

int  Test_char_traits__length()
{
	std::cout << "期望的结果:foo has a length of 14 characters.\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	const   char  * foo = "String literal";
	std::cout << "foo has a length of ";
	std::cout << std::char_traits< char >::length(foo);
	std::cout << " characters.\n";
	return  0;
} // char_traits::find 
#include <iostream>     // std::cout 
#include <string>       // std::char_traits 

int  Test_char_traits__find()
{
	std::cout << "期望的结果:the first 'i' in test string is at 8.\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	const   char  foo[] = "test string";
	const   char * p = std::char_traits< char >::find(foo, std::char_traits< char >::length(foo), 'i');
	if(p) std::cout << "the first 'i' in \"" << foo << "\" is at " << (p - foo) << ".\n";
	return  0;
} // char_traits::move 
#include <iostream>     // std::cout 
#include <string>       // std::char_traits 

int  Test_char_traits__move()
{
	std::cout << "期望的结果:---o............\n------o.........\n------------o...\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	char  foo[] = "---o............";
	std::cout << foo << '\n';
	std::char_traits< char >::move(foo + 3, foo, 4);
	std::cout << foo << '\n';
	std::char_traits< char >::move(foo + 6, foo, 7);
	std::cout << foo << '\n';
	return  0;
} // stol example 
#include <iostream>     // std::cout 
#include <string>       // ztl::string, std::stol 

int  Test_stol__stol()
{
	//std::cout << "期望的结果:1987520: 1987520\n2f04e009: 788848649\n-11101001100100111010: -956730\n0x7fffff: 8388607\n" << std::endl;
	//std::cout << "下面是测试结果:" << std::endl;

	//ztl::string str_dec = "1987520";
	//ztl::string str_hex = "2f04e009";
	//ztl::string str_bin = "-11101001100100111010";
	//ztl::string str_auto = "0x7fffff";

	//ztl::string::size_type sz;    // alias of size_t 

	//long  li_dec = std::stol(str_dec, &sz);
	//long  li_hex = std::stol(str_hex, nullptr, 16);
	//long  li_bin = std::stol(str_bin, nullptr, 2);
	//long  li_auto = std::stol(str_auto, nullptr, 0);

	//std::cout << str_dec << ": " << li_dec << '\n';
	//std::cout << str_hex << ": " << li_hex << '\n';
	//std::cout << str_bin << ": " << li_bin << '\n';
	//std::cout << str_auto << ": " << li_auto << '\n';

	return  0;
} // stod example 
#include <iostream>     // std::cout 
#include <string>       // ztl::string, std::stod 

int  Test_stod__stod()
{
	//std::cout << "期望的结果:The moon completes 12.3684 orbits per Earth year.\n" << std::endl;
	//std::cout << "下面是测试结果:" << std::endl;

	//ztl::string orbits("365.24 29.53");
	//ztl::string::size_type sz;      // alias of size_t 

	//double  earth = std::stod(orbits, &sz);
	//double  moon = std::stod(orbits.substr(sz));
	//std::cout << "The moon completes " << (earth / moon) << " orbits per Earth year.\n";
	return  0;
} // stof example 
#include <iostream>     // std::cout 
#include <string>       // ztl::string, std::stof 

int  Test_stof__stof()
{
	//std::cout << "期望的结果:One martian year takes 1.88087 Earth years.\n" << std::endl;
	//std::cout << "下面是测试结果:" << std::endl;

	//ztl::string orbits("686.97 365.24");
	//ztl::string::size_type sz;      // alias of size_t 

	//float  mars = std::stof(orbits, &sz);
	//float  earth = std::stof(orbits.substr(sz));
	//std::cout << "One martian year takes " << (mars / earth) << " Earth years.\n";
	return  0;
} // stoll example 
#include <iostream>     // std::cout 
#include <string>       // ztl::string, std::stoll 

int  Test_stoll__stoll()
{
	//std::cout << "期望的结果:8246821 interpreted as 8246821\n 0xffff interpreted as 65535\n 020 interpreted as 16\n" << std::endl;
	//std::cout << "下面是测试结果:" << std::endl;

	//ztl::string str = "8246821 0xffff 020";

	//ztl::string::size_type sz = 0;    // alias of size_t 

	//while(!str.empty())
	//{
	//	long   long  ll = std::stoll(str, &sz, 0);
	//	std::cout << str.substr(0, sz) << " interpreted as " << ll << '\n';
	//	str = str.substr(sz);
	//}

	return  0;
} // stold example 
#include <iostream>     // std::cout 
#include <string>       // ztl::string, std::stod 

int  Test_stold__stold()
{
	//std::cout << "期望的结果:Pluto takes 248.093 years to complete an orbit.\n" << std::endl;
	//std::cout << "下面是测试结果:" << std::endl;

	//ztl::string orbits("90613.305 365.24");
	//ztl::string::size_type sz;      // alias of size_t 

	//long   double  pluto = std::stod(orbits, &sz);
	//long   double  earth = std::stod(orbits.substr(sz));
	//std::cout << "Pluto takes " << (pluto / earth) << " years to complete an orbit.\n";
	return  0;
} // stoul example 
#include <iostream>     // std::cin, std::cout 
#include <string>       // ztl::string, std::stoul, std::getline 

int  Test_stoul__stoul()
{
	/*std::cout << "期望的结果:" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string str;
	std::cout << "Enter an unsigned number: ";
	std::getline(std::cin, str);
	unsigned   long  ul = std::stoul(str, nullptr, 0);
	std::cout << "You entered: " << ul << '\n';*/
	return  0;
}
// stoi example 
#include <iostream>     // std::cout 
#include <string>       // ztl::string, std::stoi 

int  Test_stoi__stoi()
{
	//std::cout << "期望的结果:\n2001, A Space Odyssey: 2001 and [, A Space Odyssey]\n40c3:  16579\n-10010110001: -1201\n0x7f: 127\n" << std::endl;
	//std::cout << "下面是测试结果:" << std::endl;

	//ztl::string str_dec = "2001, A Space Odyssey";
	//ztl::string str_hex = "40c3";
	//ztl::string str_bin = "-10010110001";
	//ztl::string str_auto = "0x7f";

	//ztl::string::size_type sz;    // alias of size_t 

	//int  i_dec = std::stoi(str_dec, &sz);
	//int  i_hex = std::stoi(str_hex, nullptr, 16);
	//int  i_bin = std::stoi(str_bin, nullptr, 2);
	//int  i_auto = std::stoi(str_auto, nullptr, 0);

	//std::cout << str_dec << ": " << i_dec << " and [" << str_dec.substr(sz) << "]\n";
	//std::cout << str_hex << ": " << i_hex << '\n';
	//std::cout << str_bin << ": " << i_bin << '\n';
	//std::cout << str_auto << ": " << i_auto << '\n';

	return  0;
} // stoull example 
#include <iostream>     // std::cout 
#include <string>       // ztl::string, std::stoull 

int  Test_stoull__stoull()
{
	//std::cout << "期望的结果:8246821 interpreted as 8246821\n 0xffff interpreted as 65535\n 020 interpreted as 16\n -1 interpreted as 18446744073709551615\n" << std::endl;
	//std::cout << "下面是测试结果:" << std::endl;

	//ztl::string str = "8246821 0xffff 020 -1";

	//ztl::string::size_type sz = 0;    // alias of size_t 

	//while(!str.empty())
	//{
	//	unsigned   long   long  ull = std::stoull(str, &sz, 0);
	//	std::cout << str.substr(0, sz) << " interpreted as " << ull << '\n';
	//	str = str.substr(sz);
	//}

	return  0;
} // appending to string 
#include <iostream> 
#include <string> 

int  Test_string__append()
{
	std::cout << "期望的结果:Writing 10 dots here: .......... and then 5 more.....\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string str;
	ztl::string str2 = "Writing ";
	ztl::string str3 = "print 10 and then 5 more";

	// used in the same order as described above: 
	str.append(str2);                        // "Writing " 
	str.append(str3, 6, 3);                    // "10 " 
	str.append("dots are cool", 5);           // "dots " 
	str.append("here: ");                    // "here: " 
	str.append(10u, '.');                     // ".........." 
	str.append(str3.begin() + 8, str3.end());   // " and then 5 more" 
	str.append(5, 0x2E);                 // "....." 

	std::cout << str << '\n';
	return  0;
} // string::back 
#include <iostream> 
#include <string> 

int  Test_string__back()
{
	std::cout << "期望的结果:hello world!\n" << std::endl;
	std::cout << "下面是测试结果:" << std::endl;

	ztl::string str("hello world.");
	str.back() = '!';
	std::cout << str << '\n';
	return  0;
} // string::operator+= 

void test_string()
{
	Test_basic_string__append();
	Test_basic_string__back();
	Test_basic_string__operatoraddequal();
	Test_basic_string__assign();
	Test_basic_string__insert();
	Test_basic_string__at();
	Test_basic_string__rend();
	Test_basic_string__basic_string();
	Test_basic_string__cbegin();
	Test_basic_string__capacity();
	Test_basic_string__swapshortfree();
	Test_basic_string__rfind();
	Test_basic_string__begin();
	Test_basic_string__cend();
	Test_basic_string__clear();
	Test_basic_string__compare();
	Test_basic_string__copy();
	Test_basic_string__crbegin();
	Test_basic_string__find_first_not_of();
	Test_basic_string__crend();
	Test_basic_string__c_str();
	Test_basic_string__find();
	Test_basic_string__data();
	Test_basic_string__resize();
	Test_basic_string__empty();
	Test_basic_string__end();
	Test_basic_string__erase();
	Test_basic_string__swap();
	Test_basic_string__find_first_of();
	Test_basic_string__find_last_not_of();
	Test_basic_string__getline();
	Test_basic_string__find_last_of();
	Test_basic_string__reserve();
	Test_basic_string__push_back();
	Test_basic_string__front();
	Test_basic_string__length();
	Test_basic_string__max_size();
	Test_basic_string__operatorequal();
	Test_basic_string__operators();
	Test_basic_string__operatorse();
	Test_basic_string__operator__();
	Test_basic_string__pop_back();
	Test_basic_string__rbegin();
	Test_basic_string__replace();
	Test_basic_string__shrink_to_fit();
	Test_basic_string__size();
	Test_basic_string__substr();
	Test_char_traits__eof();
	Test_char_traits__eq();
	Test_char_traits__compare();
	Test_char_traits__copy();
	Test_char_traits__length();
	Test_char_traits__find();
	Test_char_traits__move();
	Test_stol__stol();
	Test_stod__stod();
	Test_stof__stof();
	Test_stoll__stoll();
	Test_stold__stold();
	Test_stoul__stoul();
	Test_stoi__stoi();
	Test_stoull__stoull();
	Test_string__append();
	Test_string__back();
	Test_string__operatoraddequal();
	
};