/*
 * boost_range_new.cpp
 *
 *  Created on: 2016-1-8
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

// New algorithms
#include <boost/range/algorithm_ext/copy_n.hpp>			// 复制rng的[begin, begin+n)区间到输出流
#include <boost/range/algorithm_ext/for_each.hpp>		// 同时遍历rng1和rng2
#include <boost/range/algorithm_ext/is_sorted.hpp>		// 判断rng是否是有序序列,是返回true
#include <boost/range/algorithm_ext/iota.hpp>			// 生成有序序列，比如我们需要一个定长数组，这个数组中的元素都是在某一个数值的基础之上递增的，那么用iota可以很方便的生成这个数组了
#include <boost/range/algorithm_ext/overwrite.hpp>		// 重新分配值从from进入to序列
#include <boost/range/algorithm_ext/push_back.hpp>		// 添加来自from序列的元素到target序列末尾
#include <boost/range/algorithm_ext/push_front.hpp>		// 添加来自from序列的元素到target序列首部
#include <boost/range/algorithm_ext/insert.hpp>			// 将序列from的元素插入到序列target的before位置之前，返回新序列Container
#include <boost/range/algorithm_ext/erase.hpp>			// 从rng中删除元素，算法有erase、remove_erase、remove_erase_if

BOOST_AUTO_TEST_SUITE(boost_range_new_algorithms)

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
 * copy_n: 复制rng的[begin, begin+n)区间到输出流
 	 copy_n is provided to completely replicate the standard algorithm header, it is preferable to use Range Adaptors and the extension functions to achieve the same result with greater safety.

	 copy_n copies elements from [boost::begin(rng), boost::begin(rng) + n) to the range [out, out + n)
 * Prototype

	template<class SinglePassRange, class Size, class OutputIterator>
	OutputIterator copy_n(const SinglePassRange& rng, Size n, OutputIterator out);
 */
BOOST_AUTO_TEST_CASE(t_copy_n){}

/*
 * erase: 删除元素
 	 erase the iterator range to_erase from the container target.

	 remove_erase performs the frequently used combination equivalent to target.erase(std::remove(target.begin(), target.end(), value), target.end());

	 remove_erase_if performs the frequently used combination equivalent to target.erase(std::remove_if(target.begin(), target.end(), pred), target.end());
 * Prototype

	template<class Container>
	Container& erase(
		Container& target,
		iterator_range<typename Container::iterator> to_erase);
 */
BOOST_AUTO_TEST_CASE(t_erase){

	// g_v : {1, 2, 3, 4, 5}

	foreach_(int n, g_v){
		// output: {1, 2, 3, 4, 5}
		cout << "n:" << n << endl;
	}
	cout << endl;

	boost::erase(g_v, g_v | sliced(1,3));
	foreach_(int n, g_v){

		// output: {1, 4, 5}
		cout << "m:" << n << endl;
	}
}

/*
 * for_each: 同时遍历rng1和rng2
 	 for_each traverses forward through rng1 and rng2 simultaneously. For each iteration, the element x is used from rng1 and the corresponding element y is used from rng2 to invoke fn(x,y).

	 Iteration is stopped upon reaching the end of the shorter of rng1, or rng2. It is safe to call this function with unequal length ranges.
 * Prototype

	template<
		class SinglePassRange1,
		class SinglePassRange2,
		class BinaryFunction
		>
	BinaryFunction for_each(const SinglePassRange1& rng1,
							const SinglePassRange2& rng2,
							BinaryFunction fn);

	template<
		class SinglePassRange1,
		class SinglePassRange2,
		class BinaryFunction
		>
	BinaryFunction for_each(const SinglePassRange1& rng1,
							SinglePassRange2& rng2,
							BinaryFunction fn);

	template<
		class SinglePassRange1,
		class SinglePassRange2,
		class BinaryFunction
		>
	BinaryFunction for_each(SinglePassRange1& rng1,
							const SinglePassRange2& rng2,
							BinaryFunction fn);

	template<
		class SinglePassRange1,
		class SinglePassRange2,
		class BinaryFunction
		>
	BinaryFunction for_each(SinglePassRange1& rng1,
							SinglePassRange2& rng2,
							BinaryFunction fn);
 */
struct twice{
	void operator()(int& x, g_class*& y){
		x +=2;
		y->a += 2;
	}
};
BOOST_AUTO_TEST_CASE(t_for_each){

	// g_v : {1, 2, 3, 4, 5}
	// g_v_class: {1, 2, 3, 4, 5}

	boost::for_each(g_v, g_v_class, twice());

	foreach_(int n, g_v){
		// output: {3, 4, 5, 6, 7}
		cout << "n:" << n << endl;
	}
	cout << endl;

	foreach_(g_class*& n, g_v_class){
		// output: {3, 4, 5, 6, 7}
		cout << "m:" << n->a << endl;
	}
	cout << endl;
}

/*
 * insert: 将序列from的元素插入到序列target的before位置之前
 	 insert all of the elements in the range from before the before iterator into target.
 * Prototype

	template<
		class Container,
		class SinglePassRange
	>
	Container& insert(Container& target,
					  typename Container::iterator before,
					  const SinglePassRange& from);

	// This overload is for target containers that do not require an insertion
	// position e.g. set/map
	template<
		class Container,
		class SinglePassRange
	>
	Container& insert(Container& target, const SinglePassRange& from);
 */
BOOST_AUTO_TEST_CASE(t_insert){

	// g_v: {1, 2, 3, 4, 5}

	vector<int> v1;
	v1.push_back(9);
	v1.push_back(10);

	boost::insert(g_v, g_v.begin() + 1,v1);

	foreach_(int n, g_v){
		// output: {1, 9, 10, 2, 3, 4, 5}
		cout << "n:" << n << endl;
	}
	cout << endl;
}

/*
 * iota: 生成有序序列，比如我们需要一个定长数组，这个数组中的元素都是在某一个数值的基础之上递增的，那么用iota可以很方便的生成这个数组了
 	 iota traverses forward through rng, each element y in rng is assigned a value equivalent to x + boost::distance(boost::begin(rng), it)
 * Prototype

	template<class ForwardRange, class Value>
	ForwardRange& iota(ForwardRange& rng, Value x);
 */
BOOST_AUTO_TEST_CASE(t_iota){

	vector<int> v1(4);
	boost::iota(v1, 4);

	foreach_(int n, v1){
		// output: {4, 5, 6, 7}
		cout << "n:" << n << endl;
	}
}

/*
 * is_sorted: 判断rng是否是有序序列,是返回true
 	 is_sorted determines if a range is sorted. For the non-predicate version the return value is true if and only if for each adjacent elements [x,y] the expression x < y is true.
 	 For the predicate version the return value is true is and only if for each adjacent elements [x,y] the expression pred(x,y) is true.
 * Prototype

	template<class SinglePassRange>
	bool is_sorted(const SinglePassRange& rng);

	template<class SinglePassRange, class BinaryPredicate>
	bool is_sorted(const SinglePassRange& rng, BinaryPredicate pred);
 */
BOOST_AUTO_TEST_CASE(t_is_sorted){}

/*
 * overwrite: 重新分配值从from进入to序列
 	 overwrite assigns the values from the range from into the range to.
 * Prototype

	template<
		class SinglePassRange1,
		class SinglePassRange2
		>
	void overwrite(const SinglePassRange1& from,
				   SinglePassRange2& to);
 */
BOOST_AUTO_TEST_CASE(t_overwrite){}

/*
 * push_back: 添加来自from序列的元素到target序列末尾
 	 push_back all of the elements in the range from to the back of the container target.
 * Prototype

	template<
		class Container,
		class SinglePassRange
		>
	Container& push_back(Container& target,
						 const SinglePassRange& from);
 */
BOOST_AUTO_TEST_CASE(t_push_back){}

/*
 * push_front: 添加来自from序列的元素到target序列首部
 	 push_front all of the elements in the range from to the front of the container target.
 * Prototype

	template<
		class Container,
		class SinglePassRange
		>
	Container& push_front(Container& target,
						  const SinglePassRange& from);
 */
BOOST_AUTO_TEST_CASE(t_push_front){}

/*
 * remove_erase： 等同与erase算法
 	 emove_erase actually eliminates the elements equal to value from the container. This is in contrast to the remove algorithm which merely rearranges elements.
 * Prototype

	template<class Container, class Value>
	Container& remove_erase(Container& target,
							const Value& value);
 */
BOOST_AUTO_TEST_CASE(t_remove_erase){

	// g_v4: {1, 3, 3, 4, 6}

	boost::remove_erase(g_v4, 3);

	foreach_(int n, g_v4){
		// output: {1, 4, 6}
		cout << "n:" << n << endl;
	}
}

/*
 * remove_erase_if：remove_erase的条件版本
 	 remove_erase_if removes the elements x that satisfy pred(x) from the container. This is in contrast to the erase algorithm which merely rearranges elements.
 * Prototype

	template<class Container, class Pred>
	Container& remove_erase_if(Container& target,
							   Pred pred);
 */
BOOST_AUTO_TEST_CASE(t_remove_erase_if){}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
