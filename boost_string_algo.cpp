/*
 * boost_string_algo.cpp
 *
 *  Created on: 2016-1-10
 *      Author: root
 */

#include <boost/test/unit_test.hpp>

#include <iostream>
using namespace std;

//#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

#include <boost/algorithm/string/std_containers_traits.hpp>
#include <boost/algorithm/string/trim.hpp>							// 清除空格函数
#include <boost/algorithm/string/case_conv.hpp>						// 大小写转换函数
#include <boost/algorithm/string/predicate.hpp>						// 带谓语的处理函数
#include <boost/algorithm/string/find.hpp>							// 查找函数
#include <boost/algorithm/string/split.hpp>							// 分割字符串
#include <boost/algorithm/string/join.hpp>							// 连接字符串
#include <boost/algorithm/string/replace.hpp>						// 替换函数
#include <boost/algorithm/string/erase.hpp>							// 删除函数
#include <boost/algorithm/string/classification.hpp>				// 分类函数,用于检查一个字符是否符合某种特性,主要用于搭配其他算法,比如all算法,
																	// 分类函数返回类型为is_classifiedF函数对象,这个函数对象的operator()才是真正的分类函数,is_classifiedF函数对象重载了运算符||、&&、!,可以使用运算进行组合判断.
#include <boost/algorithm/string/find_iterator.hpp>					// 查找所有满足条件子串
#include <boost/algorithm/string/regex.hpp>							// 正在表达式
#include <boost/algorithm/string/sequence_traits.hpp>
using namespace boost;

BOOST_AUTO_TEST_SUITE(boost_string_algo)

/*
 * to_upper		:转换str为大写
 * to_lower		:转换str为小写
 */
BOOST_AUTO_TEST_CASE(t_case_conversion){

	string str1("HeLlO WoRld!");
	boost::to_upper(str1); // str1=="HELLO WORLD!"

	BOOST_CHECK_EQUAL(str1, "HELLO WORLD!");

	boost::to_lower(str1); // str1=="hello world!"

	BOOST_CHECK_EQUAL(str1, "hello world!");
}

/*
 * starts_with				：input中是否以test字符串开始，是返回true
 * istarts_with				：istarts_with的忽略大小写版本
 * ends_with				：input中是否以test字符串结尾，是返回true
 * iends_with				：ends_with的忽略大小写版本
 * contains					：input中是否包含test字符串，是返回true
 * icontains				：contains的忽略大小写版本
 * equals					：test与input中所有元素位置和值相同，则返回true
 * iequals					：equals的忽略大小写版本
 * lexicographical_compare	：按字典序检查是否第1个参数小于第2个参数，是返回true
 * ilexicographical_compare	：lexicographical_compare的忽略大小写版本
 * all						：检查input中所有元素是否满足条件，是返回true
 */
BOOST_AUTO_TEST_CASE(t_predicates){}

/*
 * is_space				:字符是否为空
 * is_alnum				:字符是否为字母或数字字符
 * is_alpha				:字符是否为字母
 * is_cntrl				:字符是否为控制字符
 * is_digit				:字符是否为十进制数字
 * is_graph				:字符是否为图形字符
 * is_lower				:字符是否为小写
 * is_upper				:字符是否为大写
 * is_print				:字符是否为可打印字符
 * is_punct				:字符是否为标点符号字符
 * is_xdigit			:字符是否为十六进制数字
 * is_any_of			:字符是否是参数字符序列中的任意字符
 * is_from_range		:字符是否位于指定区间内,即from <= ch <= to.
 */
BOOST_AUTO_TEST_CASE(t_classification){
	string str="hello";

	BOOST_CHECK_EQUAL(boost::all(str, boost::is_lower()), true);

}

/*
 * trim_left			：清除左边空格
 * trim_left_if
 * trim_left_copy
 * trim_left_copy_if
 * trim_right			：清除右边空格
 * trim_right_if
 * trim_right_copy
 * trim_right_copy_if
 * trim					：清除两边空格
 * trim_if
 * trim_copy
 * trim_copy_if
 */
BOOST_AUTO_TEST_CASE(t_trimming){
	string str1="     hello world!     ";
	string str2= boost::trim_left_copy(str1);   // str2 == "hello world!     "
	string str3= boost::trim_right_copy(str1);  // str3 == "     hello world!"
	boost::trim(str1);                          // str1 == "hello world!"

	string phone="00423333444";
	// remove leading 0 from the phone number
	boost::trim_left_if(phone,boost::is_any_of("0")); // phone == "423333444"
}

/*
 * find				：在input中查找finder,返回找到的iterator_range
 * find_first		：在input中查找Search第1次出现的位置，返回找到的iterator_range
 * ifind_first		：find_first的忽略大小写版本
 * find_last		：在input中查找Search最后1次出现的位置，返回找到的iterator_range
 * ifind_last		：
 * find_nth			：在input中查找Search第n次出现位置,n从0开始计数,返回找到的iterator_range
 * ifind_nth		：
 * find_head		：返回input中指定长度[begin,begin + n)的子字符串，如果n>input.size,返回input
 * find_tail		：返回input中指定长度[n,end)的子字符串，如果n>input.size,返回input
 * find_token		：查找满足token的字符串
 * find_regex		：正则表达式
 */
BOOST_AUTO_TEST_CASE(t_find){

	using namespace boost;

	//          0    5    10   15    21  25 28        38   43
	string str("Long long age, there was a  king.1234 long is.");

	iterator_range<string::iterator> rge;	//迭代器区间

	rge = find_first(str, "long");			//find_first()找第一次出现的位置

	string str1(rge.begin(), rge.end());
	BOOST_CHECK_EQUAL(str1, "long");
	BOOST_CHECK_EQUAL(rge.begin() - str.begin(), 5);
	BOOST_CHECK_EQUAL(rge.end() - str.begin(), 9);
	//cout << *rge.begin() << ":" << *rge.end() << ":" <<rge.begin() - str.begin() << ":" << rge.end()-str.begin() << endl;

	rge = ifind_first(str, "long");			//ifind_first()大小写无关找第一次出现的位置
	string str2(rge.begin(), rge.end());
	BOOST_CHECK_EQUAL(str2, "Long");
	BOOST_CHECK_EQUAL(rge.begin() - str.begin(), 0);
	BOOST_CHECK_EQUAL(rge.end() - str.begin(), 4);

	rge = find_nth(str, "ng", 2);			//find_nth()查找字符串在输入的第n次出现位置,n从0开始计数.
	BOOST_CHECK_EQUAL(rge.begin() - str.begin(), 30);

	rge = ifind_nth(str, "lo", 1);			//ifind_nth()大小写无关查找
	BOOST_CHECK_EQUAL(rge.begin() - str.begin(), 5);

	rge = find_last(str, "Lo");				//find_last()找最后一次出现的位置
	BOOST_CHECK_EQUAL(rge.begin() - str.begin(), 0);

	rge = ifind_last(str, "Lo");			//ifind_last()大小写无关找最后一次出现的位置
	BOOST_CHECK_EQUAL(rge.begin() - str.begin(), 38);

	rge = find_head(str, 9);				//find_head()取前9个字符
	string str3(rge.begin(), rge.end());
	BOOST_CHECK_EQUAL(str3, "Long long");

	rge = find_tail(str, 7);				//find_tail()取尾部7个字符
	string str4(rge.begin(), rge.end());
	BOOST_CHECK_EQUAL(str4, "ong is.");

	rge = find_first(str, "abcd");
	BOOST_CHECK(rge.empty() && !rge);		//如果找不到,rge可以隐式转换为bool

	boost::regex rx1("a[0-9]+c");			//不存在
	rge = find_regex( str, rx1 );
	BOOST_CHECK(rge.empty() && !rge);

	boost::regex rx2("there");				//查找在中间的子串
	rge = find_regex( str, rx2 );
	BOOST_CHECK_EQUAL(rge.begin() - str.begin(), 15);
	BOOST_CHECK(!rge.empty());

	boost::regex rx3("^Long");				//查找在开始的子串
	rge = find_regex( str, rx3 );
	BOOST_CHECK_EQUAL(rge.begin() - str.begin(), 0);
	BOOST_CHECK(!rge.empty());

	boost::regex rx4("is.$");				//查找在结束的子串
	rge = find_regex( str, rx4 );
	BOOST_CHECK_EQUAL(rge.begin() - str.begin(), 43);
	BOOST_CHECK(!rge.empty());

}

/*
 * replace_range		：用字符串Format替换input的指定区间SearchRange
 * replace_range_copy
 * replace_first		：用字符串Format替换input的第1个搜索到的满足条件子串
 * replace_first_copy
 * ireplace_first
 * ireplace_first_copy
 * replace_last			：用字符串Format替换input的最后1个搜索到的满足条件子串
 * replace_last_copy
 * ireplace_last
 * ireplace_last_copy
 * replace_nth			：用字符串Format替换在input中查找Search第n次出现的子串,n从0开始计数
 * replace_nth_copy
 * ireplace_nth
 * ireplace_nth_copy
 * replace_all			：用字符串Format替换input的所有搜索到的满足条件子串
 * replace_all_copy
 * ireplace_all
 * ireplace_all_copy
 * replace_head			：用字符串Format替换input指定长度[begin,begin + n)的子字符串
 * replace_head_copy
 * replace_tail			：用字符串Format替换input指定长度[n,end)的子字符串
 * replace_tail_copy
 */
BOOST_AUTO_TEST_CASE(t_replace){}

/*
 * erase_range			：删除input的指定区间SearchRange
 * erase_range_copy
 * erase_first			：删除input的第1个搜索到的满足条件子串
 * erase_first_copy
 * ierase_first
 * ierase_first_copy
 * erase_last			：删除input的最后1个搜索到的满足条件子串
 * erase_last_copy
 * ierase_last
 * ierase_last_copy
 * erase_nth			：删除在input中查找Search第n次出现的子串,n从0开始计数
 * erase_nth_copy
 * ierase_nth
 * ierase_nth_copy
 * erase_all			：删除input的所有搜索到的满足条件子串
 * erase_all_copy
 * ierase_all
 * ierase_all_copy
 * erase_head			：删除input指定长度[begin,begin + n)的子字符串
 * erase_head_copy
 * erase_tail			：删除input指定长度[n,end)的子字符串
 * erase_tail_copy
 */
BOOST_AUTO_TEST_CASE(t_erase){}

BOOST_AUTO_TEST_CASE(t_find_iterator){

	using namespace boost;

	string str1("abc-*-ABC-*-aBc");
	// Find all 'abc' substrings (ignoring the case)
	// Create a find_iterator
	typedef find_iterator<string::iterator> string_find_iterator;

	for(string_find_iterator It = make_find_iterator(str1, boost::first_finder("abc", is_iequal()));
		It!=string_find_iterator();
		++It)
	{
		cout << copy_range<std::string>(*It) << endl;
	}


	// Output will be:
	// abc
	// ABC
	// aBC

	typedef split_iterator<string::iterator> string_split_iterator;
	for(string_split_iterator It=
		make_split_iterator(str1, first_finder("-*-", is_iequal()));
		It!=string_split_iterator();
		++It)
	{
		cout << copy_range<std::string>(*It) << endl;
	}

	// Output will be:
	// abc
	// ABC
	// aBC
}

/*
 * find_all			：在input中查找所有满足条件的子串，输入到新容器
 * ifind_all
 * split			：分割字符串
 */
BOOST_AUTO_TEST_CASE(t_split){
	using namespace boost;

	string str1("hello abc-*-ABC-*-aBc goodbye");

	typedef vector< iterator_range<string::iterator> > find_vector_type;

	find_vector_type FindVec; // #1: Search for separators
	ifind_all( FindVec, str1, "abc" ); // FindVec == { [abc],[ABC],[aBc] }

	vector<string> v1;
	ifind_all( v1, str1, "abc" );

	BOOST_CHECK_EQUAL(v1[0], "abc");

	typedef vector< string > split_vector_type;

	split_vector_type SplitVec; // #2: Search for tokens
	// 最后一个参数为token_compress_on时,当两个分割符出现的时候视为一个,token_compress_off则两个连续分隔符标记为一个空字符串.
	split( SplitVec, str1, is_any_of("-*"), token_compress_on ); // SplitVec == { "hello abc","ABC","aBc goodbye" }
}

/*
 * join			:连接2个字符串
 * join_if
 */
BOOST_AUTO_TEST_CASE(t_join){

	vector<string> v;
	v.push_back("one");
	v.push_back("two");

	string out = boost::join(v, ",");

	BOOST_CHECK_EQUAL(out, "one,two");
}


BOOST_AUTO_TEST_SUITE_END()
