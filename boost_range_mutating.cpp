/*
 * boost_range2.cpp
 *
 *  Created on: 2016-1-6
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

// Mutating algorithms
#include <boost/range/algorithm/copy.hpp>					// 拷贝元素
#include <boost/range/algorithm/copy_backward.hpp>
#include <boost/range/algorithm/fill.hpp>					// 值分配给序列元素
#include <boost/range/algorithm/fill_n.hpp>
#include <boost/range/algorithm/generate.hpp>				// 创建器分配给序列元素
#include <boost/range/algorithm/inplace_merge.hpp>			// 将两个已经排序的序列[begin(rng), middle)和[middle, end(rng))，合并进入一个新排序的序列[begin(rng), end(rng))中
#include <boost/range/algorithm/merge.hpp>					// 将两个已经排序的序列rng1和rng2，合并进入一个新排序的序列中
#include <boost/range/algorithm/nth_element.hpp>			// 仅排序第nth个元素（从0开始的索引）
#include <boost/range/algorithm/partial_sort.hpp>			// 排序(begin(rng), middle)范围内元素
#include <boost/range/algorithm/partial_sort_copy.hpp>
#include <boost/range/algorithm/partition.hpp>				// 按分区排序元素，满足pred的元素在不满足pred的元素之前
#include <boost/range/algorithm/random_shuffle.hpp>			// 用来对一个元素序列进行重新排序（随机的）
#include <boost/range/algorithm/remove.hpp>					// 删除序列中与value值相等的元素,但是删除机制是将用value后面的值去覆盖value，未被删除的元素整体前移，相对位置不变，此时容器大小也不变
#include <boost/range/algorithm/remove_copy.hpp>
#include <boost/range/algorithm/remove_copy_if.hpp>
#include <boost/range/algorithm/remove_if.hpp>
#include <boost/range/algorithm/replace.hpp>				// 替换序列中满足条件的所有元素
#include <boost/range/algorithm/replace_copy.hpp>
#include <boost/range/algorithm/replace_copy_if.hpp>
#include <boost/range/algorithm/replace_if.hpp>
#include <boost/range/algorithm/reverse.hpp>				// 反转序列中所有元素
#include <boost/range/algorithm/reverse_copy.hpp>
#include <boost/range/algorithm/rotate.hpp>					// 旋转序列中的元素，交换[begin(rng), middle)和[middle, end(rng))序列
#include <boost/range/algorithm/rotate_copy.hpp>
#include <boost/range/algorithm/sort.hpp>					// 排序序列
#include <boost/range/algorithm/stable_partition.hpp>		// partition的稳定版本，stable_partition与partition的区别在于stable_partition保留了pred(x,y)中x==y时的两元素的相对位置
#include <boost/range/algorithm/stable_sort.hpp>			// sort的稳定版本，stable_sort与sort的区别在于stable_sort保留了pred(x,y)中x==y时的两元素的相对位置
#include <boost/range/algorithm/transform.hpp>				// 改变元素值
#include <boost/range/algorithm/unique.hpp>					// 获得唯一元素的序列(只保留遇到的相邻相同元素的第1个元素)
#include <boost/range/algorithm/unique_copy.hpp>

BOOST_AUTO_TEST_SUITE(boost_range_mutating_algorithms)

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
 * copy: 拷贝元素
 	 copy copies all elements from source_rng to the range [out_it, out_it + distance(source_rng)).
 	 The return value is out_it + distance(source_rng)
 * Prototype
	template<class SinglePassRange, class OutputIterator>
	OutputIterator copy(const SinglePassRange& source_rng, OutputIterator out_it);
 * Precondition:
    out_it is not an iterator within the source_rng.
    [out_it, out_it + distance(source_rng)) is a valid range.
 *
 */
BOOST_AUTO_TEST_CASE(t_copy){

	// g_v : {1, 2, 3, 4, 5}
	// g_v_class : {1, 2, 3, 4, 5}

	vector<int> v1;
	v1.push_back(10);

	boost::copy(g_v, std::back_inserter(v1));

	foreach_(int n, v1){
		// output: {10, 1, 2, 3, 4, 5}
		cout << "n:" << n << endl;
	}
	cout << endl;

	vector<g_class*> v2;
	v2.push_back(new g_class(10, 1));

	// Pointer is deeply copy.
	boost::copy(g_v_class, std::back_inserter(v2));

	foreach_(g_class*& n, v2){
		// output: {10, 1, 2, 3, 4, 5}
		cout << "m:" << n->a << endl;
	}
	cout << endl;

	delete v2[0];
}

/*
 * fill: 值分配给序列元素
 	 fill assigns the value val to every element in the range rng.
 * fill_n: assigns the value val to n elements in the range rng beginning with boost::begin(rng).
 * Prototype

	template<class ForwardRange, class Value>
	ForwardRange& fill( ForwardRange& rng, const Value& val );

	template<class ForwardRange, class Size, class Value>
	ForwardRange& fill( ForwardRange& rng, Size n, const Value& val );
 *
 */
BOOST_AUTO_TEST_CASE(t_fill){

	vector<int> v1(2);
	v1[0] = 10;

	foreach_(int n, v1){
		// output: {10, 0}
		cout << "n:" << n << endl;
	}
	cout << endl;

	boost::fill(v1, 2);				// fill(rng, 2)
	foreach_(int n, v1){
		// output: {2, 2}
		cout << "n:" << n << endl;
	}
	cout << endl;

	boost::fill_n(v1, 1, 5);		// fill(rng, size, value)
	foreach_(int n, v1){
		// output: {5, 2}
		cout << "n:" << n << endl;
	}
	cout << endl;
}

/*
 * generate: 创建器分配给序列元素
 	 generate assigns the result of gen() to each element in range rng. Returns the resultant range.
 * Prototype
	template<class ForwardRange, class Generator>
	ForwardRange& generate( ForwardRange& rng, Generator gen );

	template<class ForwardRange, class Generator>
	const ForwardRange& generate( const ForwardRange& rng, Generator gen );
 * Precondition:
    out_it is not an iterator within rng.
    [out_it, out_it + distance(rng)) is a valid range.
 *
 */
struct g_class_generator{
	g_class* operator()() const {
		g_class* p = new g_class(1,1);

		// output:
		// 0x550bde0
		// 0x550be30
		// 0x550be80
		cout << p << endl;

		return p;}
};
BOOST_AUTO_TEST_CASE(t_generate){

	vector<g_class*> v1(3);

	boost::generate(v1, g_class_generator());

	foreach_(g_class*& n, v1){
		// output:
		// n:0x550bde0
		// n:0x550be30
		// n:0x550be80

		cout << "n:" << n << endl;

		delete n;
	}
}

/*
 * inplace_merge: 将两个已经排序的序列[begin(rng), middle)和[middle, end(rng))，合并进入一个新排序的序列[begin(rng), end(rng))中
 	 inplace_merge combines two consecutive sorted ranges [begin(rng), middle) and [middle, end(rng))
 	 into a single sorted range [begin(rng), end(rng)). That is, it starts with a range [begin(rng), end(rng)) that
 	 consists of two pieces each of which is in ascending order, and rearranges it so that the entire range is in ascending order.
 	 inplace_merge is stable, meaning both that the relative order of elements within each input range is preserved.
 */
BOOST_AUTO_TEST_CASE(t_inplace_merge){}

/*
 * merge: 将两个已经排序的序列rng1和rng2，合并进入一个新排序的序列中
 	 merge combines two sorted ranges rng1 and rng2 into a single sorted range by copying elements. merge is stable.
 	 The return value is out + distance(rng1) + distance(rng2). The two versions of merge differ by how they compare the elements.
 	 The non-predicate version uses the operator<() for the range value type. The predicate version uses the predicate instead of operator<().
 */
BOOST_AUTO_TEST_CASE(t_merge){}

/*
 * nth_element: 仅排序第nth个元素（从0开始的索引）
 	 nth_element partially orders a range of elements. nth_element arranges the range rng such that the element
 	 corresponding with the iterator nth is the same as the element that would be in that position if rng has been sorted.
 */
BOOST_AUTO_TEST_CASE(t_nth_element){}

/*
 * partial_sort: 排序(begin(rng), middle)范围内元素
 	 partial_sort rearranges the elements in rng. It places the smallest distance(begin(rng), middle) elements,
	 sorted in ascending order, into the range [begin(rng), middle). The remaining elements are placed in an unspecified order into [middle, last).
	 The non-predicative versions of this function specify that one element is less than another by using operator<().
	 The predicate versions use the predicate instead.
 */
BOOST_AUTO_TEST_CASE(t_partial_sort){}

/*
 * partition: 按分区排序元素，满足pred的元素在不满足pred的元素之前
 	 partition orders the elements in rng based on pred, such that the elements that satisfy pred precede the elements that do not.
 	 In the versions that return a single iterator, the return value is the middle iterator. In the versions that have a configurable range_return,
 	 found corresponds to the middle iterator.
 */
BOOST_AUTO_TEST_CASE(t_partition){}

/*
 * random_shuffle: 用来对一个元素序列进行重新排序（随机的）
	random_shuffle randomly rearranges the elements in rng. The versions of random_shuffle that do not specify a Generator use an internal random number generator.
 	The versions of random_shuffle that do specify a Generator use this instead. Returns the shuffles range.
 */
BOOST_AUTO_TEST_CASE(t_random_shuffle){}

/*
 * remove: 删除序列中与value 值相等的元素。但是删除机制是将用value后面的值去覆盖value，未被删除的元素整体前移，相对位置不变，此时容器大小也不变
 	 remove removes from rng all of the elements x for which x == val is true.
 	 The versions of remove that return an iterator, return an iterator new_last such that the range [begin(rng), new_last) contains no elements equal to val.
 	 The range_return versions of remove defines found as the new last element. The iterators in the range [new_last, end(rng)) are dereferenceable,
 	 but the elements are unspecified.
 * Prototype

	template<class ForwardRange, class Value>
	typename range_iterator<ForwardRange>::type
	remove(ForwardRange& rng, const Value& val);

	template<class ForwardRange, class Value>
	typename range_iterator<const ForwardRange>::type
	remove(const ForwardRange& rng, const Value& val);

	template<range_return_value re, class ForwardRange, class Value>
	typename range_return<ForwardRange,re>::type
	remove(ForwardRange& rng, const Value& val);

	template<range_return_value re, class ForwardRange, class Value>
	typename range_return<const ForwardRange,re>::type
	remove(const ForwardRange& rng, const Value& val);
 */
BOOST_AUTO_TEST_CASE(t_remove){

	// g_v3 : {1, 3, 4, 3, 6}

	vector<int> v1;

	boost::copy(boost::remove<boost::return_begin_found>(g_v3, 3), std::back_inserter(v1));

	foreach_(int n, g_v3){
		// output: {1, 4, 6, 3, 6}
		cout << "n:" << n << endl;
	}
	cout << endl;

	foreach_(int n, v1){
		// output: {1, 4, 6}
		cout << "m:" << n << endl;
	}
	cout << endl;
}

/*
 * remove_if: remove的条件版本
	remove_if removes from rng all of the elements x for which pred(x) is true.
 	The versions of remove_if that return an iterator, return an iterator new_last such that the range [begin(rng), new_last) contains no elements where pred(x) is true.
 	The iterators in the range [new_last, end(rng)) are dereferenceable, but the elements are unspecified.
 * Prototype

	template<class ForwardRange, class UnaryPredicate>
	typename range_iterator<ForwardRange>::type
	remove(ForwardRange& rng, UnaryPredicate pred);

	template<class ForwardRange, class UnaryPredicate>
	typename range_iterator<const ForwardRange>::type
	remove(const ForwardRange& rng, UnaryPredicate pred);

	template<range_return_value re, class ForwardRange, class UnaryPredicate>
	typename range_return<ForwardRange,re>::type
	remove(ForwardRange& rng, UnaryPredicate pred);

	template<range_return_value re, class ForwardRange, class UnaryPredicate>
	typename range_return<const ForwardRange,re>::type
	remove(const ForwardRange& rng, UnaryPredicate pred);
 */
struct remove_odd{
	bool operator()(int x) const {return (x % 2 == 1)?true:false;}
};
struct remove_odd_class{
	bool operator()(g_class* const & x) const {return (x->a % 2 == 1)?true:false;}
};
BOOST_AUTO_TEST_CASE(t_remove_if){

	// g_v : {1, 2, 3, 4, 5}

	vector<int> v1;

	boost::copy(boost::remove_if<boost::return_begin_found>(g_v, remove_odd()), std::back_inserter(v1));

	foreach_(int n, g_v){
		// output: {2, 4, 3, 4, 5}
		cout << "n:" << n << endl;
	}
	cout << endl;

	foreach_(int n, v1){
		// output: {2, 4}
		cout << "m:" << n << endl;
	}
	cout << endl;
}

/*
 * remove_copy: remove的copy版本，将copy创建一个删除元素后的新序列
	remove_copy copied all of the elements x from rng for which x == val is false.
 * Prototype

	template<class ForwardRange, class Outputiterator, class Value>
	OutputIterator
	remove_copy(ForwardRange& rng, OutputIterator out, const Value& val);

	template<class ForwardRange, class OutputIterator, class Value>
	OutputIterator
	remove_copy(const ForwardRange& rng, OutputIterator out, const Value& val);
 */
BOOST_AUTO_TEST_CASE(t_remove_copy){
	// g_v3 : {1, 3, 4, 3, 6}

	vector<int> v1;

	boost::remove_copy(g_v3, std::back_inserter(v1), 3);

	foreach_(int n, g_v3){
		// output: {1, 3, 4, 3, 6}
		cout << "n:" << n << endl;
	}
	cout << endl;

	foreach_(int n, v1){
		// output: {1, 4, 6}
		cout << "m:" << n << endl;
	}
	cout << endl;
}

/*
 * remove_copy_if: remove的copy+if版本
	remove_copy_if copied all of the elements x from rng for which pred(x) is false.
 * Prototype

	template<class ForwardRange, class Outputiterator, class UnaryPred>
	OutputIterator
	remove_copy_if(ForwardRange& rng, OutputIterator out, UnaryPred pred);

	template<class ForwardRange, class OutputIterator, class UnaryPred>
	OutputIterator
	remove_copy_if(const ForwardRange& rng, OutputIterator out, UnaryPred pred);
 */
BOOST_AUTO_TEST_CASE(t_remove_copy_if){
	// g_v_class : {1, 2, 3, 4, 5}

	vector<g_class*> v1;

	boost::remove_copy_if(g_v_class, std::back_inserter(v1), remove_odd_class());

	foreach_(g_class*& n, g_v_class){
		// output:
		// n:0x550da00 -- 1
		// n:0x550daa0 -- 2
		// n:0x550db40 -- 3
		// n:0x550dbf0 -- 4
		// n:0x550dc40 -- 5
		cout << "n:" << n << " -- " << n->a << endl;
	}
	cout << endl;

	foreach_(g_class*& n, v1){
		// output:
		// m:0x550daa0 -- 2
		// m:0x550dbf0 -- 4
		cout << "m:" << n << " -- " << n->a << endl;
	}
	cout << endl;
}

/*
 * replace: 替换序列中满足条件的所有元素
 	 replace every element in rng equal to what with with_what. Return a reference to rng.
 * Prototype

	template<
		class ForwardRange,
		class Value
		>
	ForwardRange& replace(ForwardRange& rng, const Value& what, const Value& with_what);

	template<
		class ForwardRange,
		class UnaryPredicate
		>
	const ForwardRange& replace(const ForwardRange& rng, const Value& what, const Value& with_what);
 */
BOOST_AUTO_TEST_CASE(t_replace){
	// g_v3 : {1, 3, 4, 3, 6}

	boost::replace(g_v3, 3, 10);

	foreach_(int n, g_v3){
		// output: {1, 10, 4, 10, 6}
		cout << "n:" << n << endl;
	}
	cout << endl;
}

/*
 * replace_copy: replace的copy版本
	replace_copy copy every element x in rng such that the corresponding element in the output range y is x == what ? with_what : x.
 * Prototype

	template<class ForwardRange, class OutputIterator, class Value>
	OutputIterator replace_copy(const ForwardRange& rng, OutputIterator out,
								const Value& what, const Value& with_what);
 */
BOOST_AUTO_TEST_CASE(t_replace_copy){
	// g_v3 : {1, 3, 4, 3, 6}

	vector<int> v1;

	boost::replace_copy(g_v3, std::back_inserter(v1), 3, 10);

	foreach_(int n, g_v3){
		// output: {1, 3, 4, 3, 6}
		cout << "n:" << n << endl;
	}
	cout << endl;

	foreach_(int n, v1){
		// output: {1, 10, 4, 10, 6}
		cout << "n:" << n << endl;
	}
}

/*
 * replace_copy_if: replace的copy+if版本
	replace_copy_if copy every element x in rng such that the corresponding element in the output range y is pred(x) ? with_what : x.
 * Prototype

	template<class ForwardRange, class OutputIterator, class UnaryPredicate, class Value>
	OutputIterator replace_copy_if(const ForwardRange& rng, OutputIterator out,
								   UnaryPredicate pred, const Value& with_what);
 */
BOOST_AUTO_TEST_CASE(t_replace_copy_if){}

/*
 * reverse: 反转序列中所有元素
	reverse reverses a range. Returns a reference to the reversed range.
 * Prototype

	template<class BidirectionalRange>
	BidirectionalRange& reverse(BidirectionalRange& rng);

	template<class BidirectionalRange>
	const BidirectionalRange& reverse(const BidirectionalRange& rng);
 */
BOOST_AUTO_TEST_CASE(t_reverse){}

/*
 * reverse_copy: reverse的copy版本
	reverse_copy copies the elements from rng in reverse order to out. Returns the output iterator one passed the last copied element.
 * Prototype

	template<class BidirectionalRange, class OutputIterator>
	OutputIterator reverse_copy(const BidirectionalRange& rng, OutputIterator out);
 */
BOOST_AUTO_TEST_CASE(t_reverse_copy){}

/*
 * rotate: 旋转序列中的元素，交换[begin(rng), middle)和[middle, end(rng))序列
 	 rotate rotates the elements in a range. It exchanges the two ranges [begin(rng), middle) and [middle, end(rng)). Returns a reference to rng.
 * Prototype

	template<class ForwardRange>
	ForwardRange& rotate(ForwardRange& rng,
						 typename range_iterator<ForwardRange>::type middle);

	template<class ForwardRange>
	const ForwardRange& rotate(const ForwardRange& rng,
							   typename range_iterator<const ForwardRange>::type middle);
 */
BOOST_AUTO_TEST_CASE(t_rotate){}

/*
 * rotate_copy: rotate的copy版本
	rotate_copy rotates the elements in a range. It copies the two ranges [begin(rng), middle) and [middle, end(rng)) to out.
 * Prototype

	template<class ForwardRange, class OutputIterator>
	OutputIterator rotate_copy(
		const ForwardRange& rng,
		typename range_iterator<ForwardRange>::type middle,
		OutputIterator out);
 */
BOOST_AUTO_TEST_CASE(t_rotate_copy){}

/*
 * sort: 排序序列
 	 sort sorts the elements in rng into ascending order. sort is not guaranteed to be stable. Returns the sorted range.
 	 For versions of the sort function without a predicate, ascending order is defined by operator<() such that for all adjacent elements [x,y], y < x == false.
 	 For versions of the sort function with a predicate, ascending order is defined by pred such that for all adjacent elements [x,y], pred(y, x) == false.
 * Prototype

	template<class RandomAccessRange>
	RandomAccessRange& sort(RandomAccessRange& rng);

	template<class RandomAccessRange>
	const RandomAccessRange& sort(const RandomAccessRange& rng);

	template<class RandomAccessRange, class BinaryPredicate>
	RandomAccessRange& sort(RandomAccessRange& rng, BinaryPredicate pred);

	template<class RandomAccessRange, class BinaryPredicate>
	const RandomAccessRange& sort(const RandomAccessRange& rng, BinaryPredicate pred);
 */
struct desc_order{
	bool operator()(int x, int y) const {return (x >= y)?true:false;}
};
BOOST_AUTO_TEST_CASE(t_sort){

	// g_v2: {1, 3, 4, 5, 7}

	boost::sort(g_v2, desc_order());

	foreach_(int n, g_v2){
		// output: {7, 5, 4, 3, 1}
		cout << "n:" << n << endl;
	}
}

/*
 * stable_partition: partition的稳定版本，stable_partition与partition的区别在于stable_partition保留了pred(x,y)中x==y时的两元素的相对位置
 	 stable_partition reorders the elements in the range rng base on the function object pred.
 	 Once this function has completed all of the elements that satisfy pred appear before all of the elements that fail to satisfy it.
 	 stable_partition differs from partition because it preserves relative order. It is stable.
	 For the versions that return an iterator, the return value is the iterator to the first element that fails to satisfy pred.
	 For versions that return a range_return, the found iterator is the iterator to the first element that fails to satisfy pred.
 * Prototype

	template<class ForwardRange, class UnaryPredicate>
	typename range_iterator<ForwardRange>::type
	stable_partition(ForwardRange& rng, UnaryPredicate pred);

	template<class ForwardRange, class UnaryPredicate>
	typename range_iterator<const ForwardRange>::type
	stable_partition(const ForwardRange& rng, UnaryPredicate pred);

	template<
		range_return_value re,
		class ForwardRange,
		class UnaryPredicate
	>
	typename range_return<ForwardRange, re>::type
	stable_partition(ForwardRange& rng, UnaryPredicate pred);

	template<
		range_return_value re,
		class ForwardRange,
		class UnaryPredicate
	>
	typename range_return<const ForwardRange, re>::type
	stable_partition(const ForwardRange& rng, UnaryPredicate pred);
 */
BOOST_AUTO_TEST_CASE(t_stable_partition){}

/*
 * stable_sort: sort的稳定版本，stable_sort与sort的区别在于stable_sort保留了pred(x,y)中x==y时的两元素的相对位置
 	 stable_sort sorts the elements in rng into ascending order. stable_sort is guaranteed to be stable.
 	 The order is preserved for equivalent elements.
	 For versions of the stable_sort function without a predicate ascending order is defined by operator<() such that for all adjacent elements [x,y], y < x == false.
	 For versions of the stable_sort function with a predicate, ascending order is designed by pred such that for all adjacent elements [x,y], pred(y,x) == false.
 * Prototype

	template<class RandomAccessRange>
	RandomAccessRange& stable_sort(RandomAccessRange& rng);

	template<class RandomAccessRange>
	const RandomAccessRange& stable_sort(const RandomAccessRange& rng);

	template<class RandomAccessRange, class BinaryPredicate>
	RandomAccessRange& stable_sort(RandomAccessRange& rng, BinaryPredicate pred);

	template<class RandomAccessRange, class BinaryPredicate>
	const RandomAccessRange& stable_sort(const RandomAccessRange& rng, BinaryPredicate pred);
 */
BOOST_AUTO_TEST_CASE(t_stable_sort){}

/*
 * swap_ranges: 交换rng1中与rng2中位置对应的元素
 	 swap_ranges swaps each element x in rng1 with the corresponding element y in rng2. Returns a reference to rng2.
 * Prototype

	template<class SinglePassRange1, class SinglePassRange2>
	SinglePassRange2& swap_ranges(SinglePassRange1& rng1, SinglePassRange& rng2);
 */
BOOST_AUTO_TEST_CASE(t_swap_ranges){}

/*
 * transform: 改变元素值
 	 UnaryOperation version:
	 transform assigns the value y to each element [out, out + distance(rng)), y = fun(x) where x is the corresponding value to y in rng1. The return value is out + distance(rng).

	 BinaryOperation version:
	 transform assigns the value z to each element [out, out + min(distance(rng1), distance(rng2))), z = fun(x,y) where x is the corresponding value in rng1 and y is the corresponding value in rng2.
	 This version of transform stops upon reaching either the end of rng1, or the end of rng2. Hence there isn't a requirement for distance(rng1) == distance(rng2) since there is a safe guaranteed behaviour,
	 unlike with the iterator counterpart in the standard library.

	 The return value is out + min(distance(rng1), distance(rng2)).
 * Prototype

	template<
		class SinglePassRange1,
		class OutputIterator,
		class UnaryOperation
	>
	OutputIterator transform(const SinglePassRange1& rng,
							 OutputIterator out,
							 UnaryOperation fun);

	template<
		class SinglePassRange1,
		class SinglePassRange2,
		class OutputIterator,
		class BinaryOperation
	>
	OutputIterator transform(const SinglePassRange1& rng1,
							 const SinglePassRange2& rng2,
							 OutputIterator out,
							 BinaryOperation fun);
 *
 */
BOOST_AUTO_TEST_CASE(t_transform){}

/*
 * unique: 获得唯一元素的序列(只保留遇到的相邻相同元素的第1个元素)
 	unique removes all but the first element of each sequence of duplicate encountered in rng.

	Elements in the range [new_last, end(rng)) are dereferenceable but undefined.

	Equality is determined by the predicate if one is supplied, or by operator==() for ForwardRange's value type.
 * Prototype

	template<class ForwardRange>
	typename range_return<ForwardRange, return_begin_found>::type
	unique(ForwardRange& rng);

	template<class ForwardRange>
	typename range_return<const ForwardRange, return_begin_found>::type
	unique(const ForwardRange& rng);

	template<class ForwardRange, class BinaryPredicate>
	typename range_return<ForwardRange, return_begin_found>::type
	unique(ForwardRange& rng, BinaryPredicate pred);

	template<class ForwardRange, class BinaryPredicate>
	typename range_return<const ForwardRange, return_begin_found>::type
	unique(const ForwardRange& rng, BinaryPredicate pred);

	template<range_return_value re, class ForwardRange>
	typename range_return<ForwardRange, re>::type
	unique(ForwardRange& rng);

	template<range_return_value re, class ForwardRange>
	typename range_return<const ForwardRange, re>::type
	unique(const ForwardRange& rng);

	template<range_return_value re, class ForwardRange, class BinaryPredicate>
	typename range_return<ForwardRange, re>::type
	unique(ForwardRange& rng, BinaryPredicate pred);

	template<range_return_value re, class ForwardRange, class BinaryPredicate>
	typename range_return<const ForwardRange, re>::type
	unique(const ForwardRange& rng, BinaryPredicate pred);
 */
bool unique_comparer(const g_class* x, const g_class* y) {
		bool re = (x->a == y->a)?true:false;
//		if(re) delete y;
		return re;
};

BOOST_AUTO_TEST_CASE(t_unique){

	// g_v_class3: {1, 3, 3, 7, 4, 4}

	vector<g_class*> v1;
	vector<g_class*> tmp;

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

	// 为保存源序列，必须先copy再unique
	boost::copy(g_v_class3, std::back_inserter(tmp));
	boost::copy(boost::unique(tmp, unique_comparer), std::back_inserter(v1));

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

	foreach_(g_class*& n, v1){

		// output:
		// m:0x5511050 -- 1
		// m:0x55110f0 -- 3
		// m:0x5511240 -- 7
		// m:0x5511290 -- 4

		cout << "m:" << n << " -- " << n->a << endl;
	}
}

/*
 * unique_copy: unique的copy版本
 	unique_copy copies the first element of each sequence of duplicates encountered in rng to out.
	Equality is determined by the predicate if one is supplied, or by operator==() for SinglePassRange's value type.
 * Prototype

	template<class SinglePassRange, class OutputIterator>
	OutputIterator unique_copy(const SinglePassRange& rng, OutputIterator out);

	template<class SinglePassRange, class OutputIterator, class BinaryPredicate>
	OutputIterator unique_copy(const SinglePassRange& rng, OutputIterator out, BinaryPredicate pred);
 */
BOOST_AUTO_TEST_CASE(t_unique_copy){

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

	boost::unique_copy(g_v_class3, std::back_inserter(v1), unique_comparer);

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

	foreach_(g_class*& n, v1){

		// output:
		// m:0x5511050 -- 1
		// m:0x55110f0 -- 3
		// m:0x5511240 -- 7
		// m:0x5511290 -- 4

		cout << "m:" << n << " -- " << n->a << endl;
	}
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()


