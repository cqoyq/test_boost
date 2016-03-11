/*
 * boost_range3.cpp
 *
 *  Created on: 2016-1-7
 *      Author: root
 */

#include <boost/test/unit_test.hpp>

#include <iostream>
using namespace std;

#include <boost/foreach.hpp>
#define foreach_         BOOST_FOREACH
#define foreach_r_       BOOST_REVERSE_FOREACH

#include <boost/range/adaptors.hpp>
using namespace boost::adaptors;

#include <boost/range/algorithm/copy.hpp>

// Non-mutating algorithms
#include <boost/range/algorithm/adjacent_find.hpp>				// 查找序列中第1个相邻并相同的元素，找到返回元素迭代器
#include <boost/range/algorithm/count.hpp>						// 统计序列中等于val值的元素数量
#include <boost/range/algorithm/count_if.hpp>
#include <boost/range/algorithm/equal.hpp>						// 比较rng1和rng2是否相同,相同返回true
#include <boost/range/algorithm/for_each.hpp>					// 遍历序列
#include <boost/range/algorithm/find.hpp>						// 查找元素，返回满足条件第1个元素的迭代器
#include <boost/range/algorithm/find_end.hpp>					// 在rng1中搜索出最后一个与另一序列rng2匹配的子序列，返回rng1中最后一个子序列的首元素的迭代器
#include <boost/range/algorithm/find_first_of.hpp>				// 在rng1中搜索rng2中任意一个元素第1次出现在rng1中的位置
#include <boost/range/algorithm/find_if.hpp>					// 根据条件查找元素，返回满足条件的第1个元素的迭代器
#include <boost/range/algorithm/lexicographical_compare.hpp> 	// 按字典序比较rng1和rng2，如果[first1, last1)按字典序列小于[first2, last2)，返回true，否则返回false
#include <boost/range/algorithm/mismatch.hpp>					// 查找rng1和rng2中第1个不相等的元素的位置，以std::pair对返回
#include <boost/range/algorithm/search.hpp>						// 在rng1中搜索是否存在与rng2完全一致的子序列，如果存在返回rng1子序列的首元素的迭代器，不存在返回end(rng1)
#include <boost/range/algorithm/search_n.hpp>					// 查找rng中是否有一系列元素值均为给定值val的子序列，它有如下两个函数原型，分别在迭代器区间[first, last)上搜索是否有n个连续元素，其值均等于value（或者满足谓词判断binary_pred的条件），返回子序列首元素的迭代器，或last以表示没有重复元素的子序列

// Binary search, rng must be asc-ordered.
#include <boost/range/algorithm/binary_search.hpp>				// 二分法查找序列中元素，找到返回true
#include <boost/range/algorithm/equal_range.hpp>				// equal的range版本，返回的是iterator的pair对
#include <boost/range/algorithm/lower_bound.hpp>				// 在first和last中的前闭后开区间进行二分查找，返回大于或等于val的第一个元素位置。如果所有元素都小于val，则返回last的位置
#include <boost/range/algorithm/upper_bound.hpp>				// 在first和last中的前闭后开区间进行二分查找，返回大于val的第一个元素位置。如果所有元素都小于val，则返回last的位置

// Minimum and Maximum
#include <boost/range/algorithm/max_element.hpp>				// 查找序列中最大元素，返回找元素的迭代器
#include <boost/range/algorithm/min_element.hpp>				// 查找序列中最小元素，返回找元素的迭代器

BOOST_AUTO_TEST_SUITE(boost_range_nonmutating_algorithms)

/* ------------------ int数组 ---------------------- */
vector<int> g_v;
vector<int> g_v2;
vector<int> g_v3;
vector<int> g_v4;

/* ------------------ 对象数组 ---------------------- */
class g_class{
public:
	g_class(int v, int c){
		a = v;
		count = c;
	};
	virtual ~g_class(){};
	int a;
	int count;
};

vector<g_class*> g_v_class;
vector<g_class*> g_v_class2;
vector<g_class*> g_v_class3;

// 测试夹具，每个测试用例都会调用
struct s_boost_range_fixture
{
	//测试用例执行前调用
	s_boost_range_fixture()
	{
		g_v.push_back(1);
		g_v.push_back(2);
		g_v.push_back(3);
		g_v.push_back(4);
		g_v.push_back(5);

		g_v2.push_back(1);
		g_v2.push_back(3);
		g_v2.push_back(4);
		g_v2.push_back(5);
		g_v2.push_back(7);

		g_v3.push_back(1);
		g_v3.push_back(3);
		g_v3.push_back(4);
		g_v3.push_back(3);
		g_v3.push_back(6);

		g_v4.push_back(1);
		g_v4.push_back(3);
		g_v4.push_back(3);
		g_v4.push_back(4);
		g_v4.push_back(6);

		g_v_class.push_back(new g_class(1, 1));
		g_v_class.push_back(new g_class(2, 1));
		g_v_class.push_back(new g_class(3, 1));
		g_v_class.push_back(new g_class(4, 1));
		g_v_class.push_back(new g_class(5, 1));

		g_v_class2.push_back(new g_class(1, 1));
		g_v_class2.push_back(new g_class(2, 1));
		g_v_class2.push_back(new g_class(4, 1));
		g_v_class2.push_back(new g_class(5, 1));
		g_v_class2.push_back(new g_class(7, 1));

		g_v_class3.push_back(new g_class(1, 1));
		g_v_class3.push_back(new g_class(3, 1));
		g_v_class3.push_back(new g_class(3, 1));
		g_v_class3.push_back(new g_class(7, 1));
		g_v_class3.push_back(new g_class(4, 1));
		g_v_class3.push_back(new g_class(4, 1));
	}

	//测试用例测试后调用
	~s_boost_range_fixture()
	{
		g_v.clear();
		g_v2.clear();
		g_v3.clear();

		vector<g_class*>::iterator iter = g_v_class.begin();
		while(iter != g_v_class.end()){

			delete (*iter);

			iter++;
		}
		g_v_class.clear();

		vector<g_class*>::iterator iter2 = g_v_class2.begin();
		while(iter2 != g_v_class2.end()){

			delete (*iter2);

			iter2++;
		}
		g_v_class2.clear();

		vector<g_class*>::iterator iter3 = g_v_class3.begin();
		while(iter3 != g_v_class3.end()){

			delete (*iter3);

			iter3++;
		}
		g_v_class3.clear();

	}
};

BOOST_FIXTURE_TEST_SUITE(sxx,s_boost_range_fixture)

/*
 * adjacent_find: 查找序列中第1个相邻并相同的元素，找到返回元素迭代器
 	Non-predicate versions:
	adjacent_find finds the first adjacent elements [x,y] in rng where x == y

	Predicate versions:
	adjacent_find finds the first adjacent elements [x,y] in rng where pred(x,y) is true.
 * Prototype

	template<class ForwardRange>
	typename range_iterator<ForwardRange>::type
	adjacent_find(ForwardRange& rng);

	template<class ForwardRange>
	typename range_iterator<const ForwardRange>::type
	adjacent_find(const ForwardRange& rng);

	template<class ForwardRange, class BinaryPredicate>
	typename range_iterator<ForwardRange>::type
	adjacent_find(ForwardRange& rng, BinaryPred pred);

	template<class ForwardRange, class BinaryPredicate>
	typename range_iterator<const ForwardRange>::type
	adjacent_find(const ForwardRange& rng, BinaryPred pred);

	template<range_return_value_re, class ForwardRange>
	typename range_return<ForwardRange, re>::type
	adjacent_find(ForwardRange& rng);

	template<range_return_value_re, class ForwardRange>
	typename range_return<const ForwardRange, re>::type
	adjacent_find(const ForwardRange& rng);

	template<
		range_return_value re,
		class ForwardRange,
		class BinaryPredicate
		>
	typename range_return<ForwardRange, re>::type
	adjacent_find(ForwardRange& rng, BinaryPredicate pred);

	template<
		range_return_value re,
		class ForwardRange,
		class BinaryPredicate
		>
	typename range_return<const ForwardRange, re>::type
	adjacent_find(const ForwardRange& rng, BinaryPredicate pred);
 */
bool adjacent_comparer(const g_class* x, const g_class* y) {
		bool re = (x->a == y->a)?true:false;
		return re;
};
BOOST_AUTO_TEST_CASE(t_adjacent_find){

	// g_v_class3: {1, 3, 3, 7, 4, 4}

	vector<g_class*> v1;

	foreach_(g_class*& n, g_v_class3){

		// output:
		// k:0x5511050 -- 1
		// k:0x55110f0 -- 3
		// k:0x5511190 -- 3
		// k:0x5511240 -- 7
		// k:0x5511290 -- 4
		// k:0x5511360 -- 4

		cout << "k:" << n << " -- " << n->a << endl;
	}
	cout << endl;

	vector<g_class*>::iterator iter = boost::adjacent_find(g_v_class3, adjacent_comparer);

	// output:
	// m:0x55110f0 -- 3
	cout << "m:" << (*iter) << " -- " << (*iter)->a << endl;
	cout << endl;

	foreach_(g_class*& n, g_v_class3){

		// output:
		// n:0x5511050 -- 1
		// n:0x55110f0 -- 3
		// n:0x5511190 -- 3
		// n:0x5511240 -- 7
		// n:0x5511290 -- 4
		// n:0x5511360 -- 4
		cout << "n:" << n << " -- " << n->a << endl;
	}
	cout << endl;
}

/*
 * binary_search: 二分法查找序列中元素，找到返回true
 	 binary_search returns true if and only if the value val exists in the range rng.
 * Precondition:
	For the non-predicate version:
	rng is ordered in ascending order according to operator<.

	For the predicate version:
	rng is ordered in ascending order according to the function object pred.
 * Prototype

	template<class ForwardRange, class Value>
	bool binary_search(const ForwardRange& rng, const Value& val);

	template<class ForwardRange, class Value, class BinaryPredicate>
	bool binary_search(const ForwardRange& rng, const Value& val, BinaryPredicate pred);
 */
BOOST_AUTO_TEST_CASE(t_binary_search){}

/*
 * count: 统计序列中等于val值的元素数量
 	 count returns the number of elements x in rng where x == val is true.
 * Prototype

	template<class SinglePassRange, class Value>
	typename range_difference<SinglePassRange>::type
	count(SinglePassRange& rng, const Value& val);

	template<class SinglePassRange, class Value>
	typename range_difference<const SinglePassRange>::type
	count(const SinglePassRange& rng, const Value& val);
 */
BOOST_AUTO_TEST_CASE(t_count){

	// g_v3 : {1, 3, 4, 3, 6}

	BOOST_CHECK_EQUAL(boost::count(g_v3, 3), 2);
}

/*
 * count_if: count的if版本
 	 count_if returns the number of elements x in rng where pred(x) is true.
 * Prototype

	template<class SinglePassRange, class UnaryPredicate>
	typename range_difference<const SinglePassRange>::type
	count_if(const SinglePassRange& rng, UnaryPredicate pred);
 */
BOOST_AUTO_TEST_CASE(t_count_if){}

/*
 * equal: 比较序列rng1和rng2是否相同,相同返回true
 	 equal returns true if distance(rng1) is equal to the distance(rng2) and for each element x in rng1,
 	 the corresponding element y in rng2 is equal. Otherwise false is returned.

	 In this range version of equal it is perfectly acceptable to pass in two ranges of unequal lengths.

	 Elements are considered equal in the non-predicate version if operator== returns true.
	 Elements are considered equal in the predicate version if pred(x,y) is true.
 * Prototype

	template<
		class SinglePassRange1,
		class SinglePassRange2
	>
	bool equal(const SinglePassRange1& rng1,
			   const SinglePassRange2& rng2);

	template<
		class SinglePassRange1,
		class SinglePassRange2,
		class BinaryPredicate
	>
	bool equal(const SinglePassRange1& rng1,
			   const SinglePassRange2& rng2,
			   BinaryPredicate         pred);
 */
bool equal_comparer(const g_class* x, const g_class* y) {
		return (x->a == y->a)?true:false;
};
BOOST_AUTO_TEST_CASE(t_equal){

	// g_v_class : {1, 2, 3, 4, 5}

	vector<g_class*> v1;

	boost::copy(g_v_class,  std::back_inserter(v1));

	BOOST_CHECK_EQUAL(boost::equal(g_v_class, v1, equal_comparer), true);
}

/*
 * equal_range: equal的range版本，返回的是iterator的pair对
 	 equal_range returns a range in the form of a pair of iterators where all of the elements are equal to val.
 	 If no values are found that are equal to val, then an empty range is returned, hence result.first == result.second.
 	 For the non-predicate versions of equal_range the equality of elements is determined by operator<.
 	 For the predicate versions of equal_range the equality of elements is determined by pred.
 * Precondition:
	For the non-predicate versions: rng is ordered in ascending order according to operator<.
	For the predicate versions: rng is ordered in ascending order according to pred.
 * Prototype

	template<
		class ForwardRange,
		class Value
		>
	std::pair<typename range_iterator<ForwardRange>::type,
			  typename range_iterator<ForwardRange>::type>
	equal_range(ForwardRange& rng, const Value& val);

	template<
		class ForwardRange,
		class Value
		>
	std::pair<typename range_iterator<const ForwardRange>::type,
			  typename range_iterator<const ForwardRange>::type>
	equal_range(const ForwardRange& rng, const Value& val);

	template<
		class ForwardRange,
		class Value,
		class SortPredicate
		>
	std::pair<typename range_iterator<ForwardRange>::type,
			  typename range_iterator<ForwardRange>::type>
	equal_range(ForwardRange& rng, const Value& val, SortPredicate pred);

	template<
		class ForwardRange,
		class Value,
		class SortPredicate
		>
	std::pair<typename range_iterator<const ForwardRange>::type,
			  typename range_iterator<const ForwardRange>::type>
	equal_range(const ForwardRange& rng, const Value& val, SortPredicate pred);
 */
BOOST_AUTO_TEST_CASE(t_equal_range){}

/*
 * for_each: 遍历序列
 	 for_each traverses forward through rng and for each element x it invokes fun(x).
 * Prototype

	template<
		class SinglePassRange,
		class UnaryFunction
		>
	UnaryFunction for_each(SinglePassRange& rng, UnaryFunction fun);

	template<
		class SinglePassRange,
		class UnaryFunction
		>
	UnaryFunction for_each(const SinglePassRange& rng, UnaryFunction fun);
 */
BOOST_AUTO_TEST_CASE(t_for_each){}

/*
 * find：查找元素，返回满足条件第1个元素的迭代器
 	 The versions of find that return an iterator, returns the first iterator in the range rng such that *i == value. end(rng) is returned if no such iterator exists.
 	 The versions of find that return a range_return, defines found in the same manner as the returned iterator described above.
 * Prototype

	template<class SinglePassRange, class Value>
	typename range_iterator<SinglePassRange>::type
	find(SinglePassRange& rng, Value val);

	template<
		range_return_value re,
		class SinglePassRange,
		class Value
		>
	typename range_return<SinglePassRange, re>::type
	find(SinglePassRange& rng, Value val);
 */
BOOST_AUTO_TEST_CASE(t_find){}

/*
 * find_end: 在一个序列rng1中搜索出最后一个与另一序列rng2匹配的子序列，返回rng1中最后一个子序列的首元素的迭代器
 	 The versions of find_end that return an iterator, return an iterator to the beginning of the last sub-sequence equal to rng2 within rng1.
 	 Equality is determined by operator== for non-predicate versions of find_end, and by satisfying pred in the predicate versions.
 	 The versions of find_end that return a range_return, defines found in the same manner as the returned iterator described above.
 * Prototype

	template<class ForwardRange1, class ForwardRange2>
	typename range_iterator<ForwardRange1>::type
	find_end(ForwardRange1& rng1, const ForwardRange2& rng2);

	template<
		class ForwardRange1,
		class ForwardRange2,
		class BinaryPredicate
		>
	typename range_iterator<ForwardRange1>::type
	find_end(ForwardRange1& rng1, const ForwardRange2& rng2, BinaryPredicate pred);

	template<
		range_return_value re,
		class ForwardRange1,
		class ForwardRange2
		>
	typename range_return<ForwardRange1, re>::type
	find_end(ForwardRange1& rng1, const ForwardRange2& rng2);

	template<
		range_return_value re,
		class ForwardRange1,
		class ForwardRange2,
		class BinaryPredicate
		>
	typename range_return<ForwardRange1, re>::type
	find_end(ForwardRange1& rng1, const ForwardRange2& rng2, BinaryPredicate pred);
 */
BOOST_AUTO_TEST_CASE(t_find_end){}

/*
 * find_first_of: 在序列rng1中搜索序列rng2中任意一个元素第1次出现在rng1中的位置
 	 The versions of find_first_of that return an iterator, return an iterator to the first occurrence in rng1 of any of the elements in rng2.
 	 Equality is determined by operator== for non-predicate versions of find_first_of, and by satisfying pred in the predicate versions.

	 The versions of find_first_of that return a range_return, defines found in the same manner as the returned iterator described above.
 * Prototype

	template<class SinglePassRange1, class ForwardRange2>
	typename range_iterator<SinglePassRange1>::type
	find_first_of(SinglePassRange1& rng1, const ForwardRange2& rng2);

	template<
		class SinglePassRange1,
		class ForwardRange2,
		class BinaryPredicate
		>
	typename range_iterator<SinglePassRange1>::type
	find_first_of(SinglePassRange1& rng1, const ForwardRange2& rng2, BinaryPredicate pred);

	template<
		range_return_value re,
		class SinglePassRange1,
		class ForwardRange2
		>
	typename range_return<SinglePassRange1, re>::type
	find_first_of(SinglePassRange1& rng1, const ForwardRange2& rng2);

	template<
		range_return_value re,
		class SinglePassRange1,
		class ForwardRange2,
		class BinaryPredicate
		>
	typename range_return<SinglePassRange1, re>::type
	find_first_of(SinglePassRange1& rng1, const ForwardRange2& rng2, BinaryPredicate pred);
 */
BOOST_AUTO_TEST_CASE(t_find_first_of){}

/*
 * find_if： 根据条件查找元素，返回满足条件的第1个元素的迭代器
 	 The versions of find_if that return an iterator, returns the first iterator in the range rng such that pred(*i) is true.
 	 end(rng) is returned if no such iterator exists.

	The versions of find_if that return a range_return, defines found in the same manner as the returned iterator described above.
 * Precondition:
	For each iterator i in rng, *i is in the domain of UnaryPredicate.
 * Prototype

	template<class SinglePassRange, class UnaryPredicate>
	typename range_iterator<SinglePassRange>::type
	find_if(SinglePassRange& rng, UnaryPredicate pred);

	template<
		range_return_value re,
		class SinglePassRange,
		class UnaryPredicate
		>
	typename range_return<SinglePassRange, re>::type
	find_if(SinglePassRange& rng, UnaryPredicate pred);
 */
BOOST_AUTO_TEST_CASE(t_find_if){}

/*
 * lexicographical_compare： 按字典序比较序列rng1和rng2，如果[first1, last1)按字典序列小于[first2, last2)，返回true，否则返回false
 	 lexicographical_compare compares element by element rng1 against rng2. If the element from rng1 is less than the element from rng2 then true is returned.
 	 If the end of rng1 without reaching the end of rng2 this also causes the return value to be true. The return value is false in all other circumstances.
 	 The elements are compared using operator< in the non-predicate versions of lexicographical_compare and using pred in the predicate versions.
 * Prototype

	template<
		class SinglePassRange1,
		class SinglePassRange2
		>
	bool lexicographical_compare(const SinglePassRange1& rng1,
								 const SinglePassRange2& rng2);

	template<
		class SinglePassRange1,
		class SinglePassRange2,
		class BinaryPredicate
		>
	bool lexicographical_compare(const SinglePassRange1& rng1,
								 const SinglePassRange2& rng2,
								 BinaryPredicate pred);
 */
BOOST_AUTO_TEST_CASE(t_lexicographical_compare){}

/*
 * lower_bound: 在first和last中的前闭后开区间进行二分查找，返回大于或等于val的第一个元素位置。如果所有元素都小于val，则返回last的位置
 	 The versions of lower_bound that return an iterator, returns the first iterator in the range rng such that: without predicate - *i < value is false, with predicate - pred(*i, value) is false.

	 end(rng) is returned if no such iterator exists.

	 The versions of lower_bound that return a range_return, defines found in the same manner as the returned iterator described above.
 * Precondition:

	For the non-predicate versions:
	rng is sorted in ascending order according to operator<.

	For the predicate versions:
	rng is sorted in ascending order according to pred.
 * Prototype

	template<
		class ForwardRange,
		class Value
		>
	typename range_iterator<ForwardRange>::type
	lower_bound(ForwardRange& rng, Value val);

	template<
		range_return_value re,
		class ForwardRange,
		class Value
		>
	typename range_return<ForwardRange, re>::type
	lower_bound(ForwardRange& rng, Value val);

	template<
		class ForwardRange,
		class Value,
		class SortPredicate
		>
	typename range_iterator<ForwardRange>::type
	lower_bound(ForwardRange& rng, Value val, SortPredicate pred);

	template<
		range_return_value re,
		class ForwardRange,
		class Value,
		class SortPredicate
		>
	typename range_return<ForwardRange,re>::type
	lower_bound(ForwardRange& rng, Value val, SortPredicate pred);
 */
bool lower_bound_3(int n, int v){
		return (n < v)?true:false;
	}
BOOST_AUTO_TEST_CASE(t_lower_bound){

	// g_v : {1, 2, 3, 4, 5}

	BOOST_CHECK_EQUAL((*boost::lower_bound(g_v, 3)), 3);
	BOOST_CHECK_EQUAL((*boost::lower_bound(g_v, 3, lower_bound_3)), 3);
}

/*
 * upper_bound: 在first和last中的前闭后开区间进行二分查找，返回大于val的第一个元素位置。如果所有元素都小于val，则返回last的位置
 	 The versions of upper_bound that return an iterator, returns the first iterator in the range rng such that: without predicate - val < *i is true, with predicate - pred(val, *i) is true.

	 end(rng) is returned if no such iterator exists.

	 The versions of upper_bound that return a range_return, defines found in the same manner as the returned iterator described above.
 * Precondition:

	For the non-predicate versions:
	rng is sorted in ascending order according to operator<.

	For the predicate versions:
	rng is sorted in ascending order according to pred.
 * Prototype

	template<
		class ForwardRange,
		class Value
		>
	typename range_iterator<ForwardRange>::type
	upper_bound(ForwardRange& rng, Value val);

	template<
		range_return_value re,
		class ForwardRange,
		class Value
		>
	typename range_return<ForwardRange, re>::type
	upper_bound(ForwardRange& rng, Value val);

	template<
		class ForwardRange,
		class Value,
		class SortPredicate
		>
	typename range_iterator<ForwardRange>::type
	upper_bound(ForwardRange& rng, Value val, SortPredicate pred);

	template<
		range_return_value re,
		class ForwardRange,
		class Value,
		class SortPredicate
		>
	typename range_return<ForwardRange,re>::type
	upper_bound(ForwardRange& rng, Value val, SortPredicate pred);
 */
bool upper_bound_3(int v, int n){
		return (v < n)?true:false;
	}
BOOST_AUTO_TEST_CASE(t_upper_bound){

	// g_v : {1, 2, 3, 4, 5}

	BOOST_CHECK_EQUAL((*boost::upper_bound(g_v, 3)), 4);
	BOOST_CHECK_EQUAL((*boost::upper_bound(g_v, 3, upper_bound_3)), 4);
}

/*
 * max_element: 查找序列中最大元素，返回找元素的迭代器
 	 The versions of max_element that return an iterator, return the iterator to the maximum value as determined by using operator< if a predicate is not supplied.
 	 Otherwise the predicate pred is used to determine the maximum value.
 	 The versions of max_element that return a range_return, defines found in the same manner as the returned iterator described above.
 * Prototype

	template<class ForwardRange>
	typename range_iterator<ForwardRange>::type
	max_element(ForwardRange& rng);

	template<class ForwardRange>
	typename range_iterator<const ForwardRange>::type
	max_element(const ForwardRange& rng);

	template<class ForwardRange, class BinaryPredicate>
	typename range_iterator<ForwardRange>::type
	max_element(ForwardRange& rng, BinaryPredicate pred);

	template<class ForwardRange, class BinaryPredicate>
	typename range_iterator<const ForwardRange>::type
	max_element(const ForwardRange& rng, BinaryPredicate pred);


	template<
		range_return_value re,
		class ForwardRange
		>
	typename range_return<ForwardRange, re>::type
	max_element(ForwardRange& rng);

	template<
		range_return_value_re,
		class ForwardRange
		>
	typename range_return<const ForwardRange, re>::type
	max_element(const ForwardRange& rng);

	template<
		range_return_value re,
		class ForwardRange,
		class BinaryPredicate
		>
	typename range_return<ForwardRange, re>::type
	max_element(ForwardRange& rng, BinaryPredicate pred);

	template<
		range_return_value re,
		class ForwardRange,
		class BinaryPredicate
		>
	typename range_return<const ForwardRange, re>::type
	max_element(const ForwardRange& rng, BinaryPredicate pred);
 */
BOOST_AUTO_TEST_CASE(t_max_element){}

/*
 * min_element: 查找序列中最小元素，返回找元素的迭代器
 	 The versions of min_element that return an iterator, return the iterator to the minimum value as determined by using operator< if a predicate is not supplied.
 	 Otherwise the predicate pred is used to determine the minimum value.
 	 The versions of min_element that return a range_return, defines found in the same manner as the returned iterator described above.
 * Prototype

	template<class ForwardRange>
	typename range_iterator<ForwardRange>::type
	min_element(ForwardRange& rng);

	template<class ForwardRange>
	typename range_iterator<const ForwardRange>::type
	min_element(const ForwardRange& rng);

	template<class ForwardRange, class BinaryPredicate>
	typename range_iterator<ForwardRange>::type
	min_element(ForwardRange& rng, BinaryPredicate pred);

	template<class ForwardRange, class BinaryPredicate>
	typename range_iterator<const ForwardRange>::type
	min_element(const ForwardRange& rng, BinaryPredicate pred);


	template<
		range_return_value re,
		class ForwardRange
		>
	typename range_return<ForwardRange, re>::type
	min_element(ForwardRange& rng);

	template<
		range_return_value_re,
		class ForwardRange
		>
	typename range_return<const ForwardRange, re>::type
	min_element(const ForwardRange& rng);

	template<
		range_return_value re,
		class ForwardRange,
		class BinaryPredicate
		>
	typename range_return<ForwardRange, re>::type
	min_element(ForwardRange& rng, BinaryPredicate pred);

	template<
		range_return_value re,
		class ForwardRange,
		class BinaryPredicate
		>
	typename range_return<const ForwardRange, re>::type
	min_element(const ForwardRange& rng, BinaryPredicate pred);
 */
BOOST_AUTO_TEST_CASE(t_min_element){}

/*
 * mismatch: 查找rng1和rng2中第1个不相等的元素的位置，以std::pair对返回
 	 mismatch finds the first position where the corresponding elements from the two ranges rng1 and rng2 are not equal.

	 Equality is determined by operator== for non-predicate versions of mismatch, and by satisfying pred in the predicate versions.
 * Precondition:
	distance(rng2) >= distance(rng1)
 * Prototype

	template<class SinglePassRange1, class SinglePassRange2>
	std::pair<
		typename range_iterator<SinglePassRange1>::type,
		typename range_iterator<const SinglePassRange2>::type >
	mismatch(SinglePassRange1& rng1, const SinglePassRange2& rng2);

	template<class SinglePassRange1, class SinglePassRange2>
	std::pair<
		typename range_iterator<const SinglePassRange1>::type,
		typename range_iterator<const SinglePassRange2>::type >
	mismatch(const SinglePassRange1& rng1, const SinglePassRange2& rng2);

	template<class SinglePassRange1, class SinglePassRange2>
	std::pair<
		typename range_iterator<SinglePassRange1>::type,
		typename range_iterator<SinglePassRange2>::type >
	mismatch(SinglePassRange1& rng1, SinglePassRange2& rng2);

	template<class SinglePassRange1, class SinglePassRange2>
	std::pair<
		typename range_iterator<const SinglePassRange1>::type,
		typename range_iterator<SinglePassRange2>::type >
	mismatch(const SinglePassRange1& rng1, SinglePassRange2& rng2);


	template<
		class SinglePassRange1,
		class SinglePassRange2,
		class BinaryPredicate
		>
	std::pair<
		typename range_iterator<SinglePassRange1>::type,
		typename range_iterator<const SinglePassRange2>::type >
	mismatch(SinglePassRange1& rng1, const SinglePassRange2& rng2,
			 BinaryPredicate pred);

	template<
		class SinglePassRange1,
		class SinglePassRange2,
		class BinaryPredicate
		>
	std::pair<
		typename range_iterator<const SinglePassRange1>::type,
		typename range_iterator<const SinglePassRange2>::type >
	mismatch(const SinglePassRange1& rng1, const SinglePassRange2& rng2,
			 BinaryPredicate pred);

	template<
		class SinglePassRange1,
		class SinglePassRange2,
		class BinaryPredicate
		>
	std::pair<
		typename range_iterator<SinglePassRange1>::type,
		typename range_iterator<SinglePassRange2>::type >
	mismatch(SinglePassRange1& rng1, SinglePassRange2& rng2,
			 BinaryPredicate pred);

	template<
		class SinglePassRange1,
		class SinglePassRange2,
		class BinaryPredicate
		>
	std::pair<
		typename range_iterator<const SinglePassRange1>::type,
		typename range_iterator<SinglePassRange2>::type >
	mismatch(const SinglePassRange1& rng1, SinglePassRange2& rng2,
			 BinaryPredicate pred);
 */
BOOST_AUTO_TEST_CASE(t_mismatch){}

/*
 * search： 在rng1中搜索是否存在与rng2完全一致的子序列，如果存在返回rng1子序列的首元素的迭代器，不存在返回end(rng1)
 	 The versions of search that return an iterator, return an iterator to the start of the first subsequence in rng1 that is equal to the subsequence rng2.
 	 The end(rng1) is returned if no such subsequence exists in rng1.
 	 Equality is determined by operator== for non-predicate versions of search, and by satisfying pred in the predicate versions.

	 The versions of search that return a range_return, defines found in the same manner as the returned iterator described above.
 * Prototype

	template<class ForwardRange1, class ForwardRange2>
	typename range_iterator<ForwardRange1>::type
	search(ForwardRange1& rng1, const ForwardRange2& rng2);

	template<class ForwardRange1, class ForwardRange2>
	typename range_iterator<const ForwardRange1>::type
	search(const ForwardRange1& rng1, const ForwardRange2& rng2);

	template<
		class ForwardRange1,
		class ForwardRange2,
		class BinaryPredicate
		>
	typename range_iterator<ForwardRange1>::type,
	search(ForwardRange1& rng1, const ForwardRange2& rng2, BinaryPredicate pred);

	template<
		class ForwardRange1,
		class ForwardRange2,
		class BinaryPredicate
		>
	typename range_iterator<const ForwardRange1>::type
	search(const ForwardRange1& rng1, ForwardRange2& rng2, BinaryPredicate pred);


	template<
		range_return_value re,
		class ForwardRange1,
		class ForwardRange2
		>
	typename range_return<ForwardRange1, re>::type
	search(ForwardRange1& rng1, const ForwardRange2& rng2);

	template<
		range_return_value re,
		class ForwardRange1,
		class ForwardRange2
		>
	typename range_return<const ForwardRange1, re>::type
	search(const ForwardRange1& rng1, const ForwardRange2& rng2);

	template<
		range_return_value re,
		class ForwardRange1,
		class ForwardRange2,
		class BinaryPredicate
		>
	typename range_return<ForwardRange1, re>::type,
	search(ForwardRange1& rng1, const ForwardRange2& rng2, BinaryPredicate pred);

	template<
		range_return_value re,
		class ForwardRange1,
		class ForwardRange2,
		class BinaryPredicate
		>
	typename range_return<const ForwardRange1, re>::type
	search(const ForwardRange1& rng1, const ForwardRange2& rng2, BinaryPredicate pred);
 */
BOOST_AUTO_TEST_CASE(t_search){}

/*
 * search_n： 搜索rng中是否有一系列元素值均为某个给定值的子序列，它有如下两个函数原型，分别在迭代器区间[first, last)上搜索是否有count个连续元素，其值均等于value（或者满足谓词判断binary_pred的条件），返回子序列首元素的迭代器，或last以表示没有重复元素的子序列
 	 search_n searches rng for a sequence of length n equal to value where equality is determined by operator== in the non-predicate case, and by a predicate when one is supplied.
 * Prototype

	template<class ForwardRange, class Integer, class Value>
	typename range_iterator<ForwardRange>::type
	search_n(ForwardRange& rng, Integer n, const Value& value);

	template<class ForwardRange, class Integer, class Value>
	typename range_iterator<const ForwardRange>::type
	search_n(const ForwardRange& rng, Integer n, const Value& value);

	template<class ForwardRange, class Integer, class Value, class BinaryPredicate>
	typename range_iterator<ForwardRange>::type
	search_n(ForwardRange& rng, Integer n, const Value& value,
			 BinaryPredicate binary_pred);

	template<class ForwardRange, class Integer, class Value, class BinaryPredicate>
	typename range_iterator<const ForwardRange>::type
	search_n(const ForwardRange& rng, Integer n, const Value& value,
			 BinaryPredicate binary_pred);
 */
bool twice(g_class*& n, int v){
	return ((n->a * 2) == v)?true:false;
};
BOOST_AUTO_TEST_CASE(t_search_n){

	// g_v_class3: {1, 3, 3, 7, 4, 4}

	foreach_(g_class*& n, g_v_class3){

		// output:
		// n:0x55163d0 -- 1
		// n:0x5516470 -- 3
		// n:0x5516510 -- 3
		// n:0x55165c0 -- 7
		// n:0x5516610 -- 4
		// n:0x55166e0 -- 4

		cout << "n:" << n << " -- " << n->a << endl;
	}
	cout << endl;

	vector<g_class*>::iterator iter = boost::search_n(g_v_class3, 2, 8, twice);

	// output:
	// m:0x5516610 -- 4
	cout << "m:" << (*iter) << " -- " << (*iter)->a << endl;
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
