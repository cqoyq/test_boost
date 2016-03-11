/*
 * boost_algorithms.cpp
 *
 *  Created on: 2015-12-31
 *      Author: root
 */

#include <boost/test/unit_test.hpp>

#include <iostream>
#include <vector>
using namespace std;

#include <boost/algorithm/cxx11/all_of.hpp>				// 测试rng所有元素满足条件，返回true
#include <boost/algorithm/cxx11/any_of.hpp>				// 测试rng任一元素满足条件，返回true
#include <boost/algorithm/cxx11/none_of.hpp>			// 测试rng所有元素不满足条件，返回true
#include <boost/algorithm/cxx11/one_of.hpp>				// 测试rng只有一个元素满足条件，返回true
#include <boost/algorithm/cxx11/is_permutation.hpp>		// rng1与rng2比较，rng1和rng2元素数量必须相同，元素值相同，只是排列顺序不同，则函数返回为true
#include <boost/algorithm/cxx14/equal.hpp>				// rng1与rng2比较，rng1和rng2元素数量相同，元素值和排列顺序一样，则函数返回为true
#include <boost/algorithm/cxx14/mismatch.hpp>			// rng1与rng2比较，返回rng1和rng2元素第一次不匹配的pair位置对
using namespace boost::algorithm;

BOOST_AUTO_TEST_SUITE(boost_algorithms)

/* ------------------ int变量 ---------------------- */
int nCount;

/* ------------------ int数组 ---------------------- */
vector<int> g_v;
vector<int> g_v2;
vector<int> g_v3;

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
struct s_boost_algorithms_fixture
{
	//测试用例执行前调用
	s_boost_algorithms_fixture()
	{
		nCount = 4;

		g_v.push_back(1);
		g_v.push_back(2);
		g_v.push_back(3);
		g_v.push_back(4);
		g_v.push_back(5);

		g_v2.push_back(5);
		g_v2.push_back(1);
		g_v2.push_back(4);
		g_v2.push_back(3);
		g_v2.push_back(2);

		g_v3.push_back(2);
		g_v3.push_back(3);
		g_v3.push_back(4);

		g_v_class.push_back(new g_class(1, 1));
		g_v_class.push_back(new g_class(2, 1));
		g_v_class.push_back(new g_class(3, 1));
		g_v_class.push_back(new g_class(4, 1));
		g_v_class.push_back(new g_class(5, 1));

		g_v_class2.push_back(new g_class(5, 2));
		g_v_class2.push_back(new g_class(4, 2));
		g_v_class2.push_back(new g_class(2, 2));
		g_v_class2.push_back(new g_class(3, 2));

		g_v_class3.push_back(new g_class(2, 3));
		g_v_class3.push_back(new g_class(3, 3));
		g_v_class3.push_back(new g_class(4, 3));
	};
	//测试用例测试后调用
	~s_boost_algorithms_fixture()
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

	};
};

BOOST_FIXTURE_TEST_SUITE(sxx,s_boost_algorithms_fixture)

bool isOdd ( int i ) { return i % 2 == 1; } //是否奇数
bool lessThan10 ( int i ) { return i < 10; } //是否小于10

bool isOddClass ( g_class*& i ) { return (i->a) % 2 == 1; } //是否奇数
bool lessThan10Class ( g_class*& i ) { return (i->a) < 10; } //是否小于10

BOOST_AUTO_TEST_CASE(t_all_of){

	// g_v value:{1, 2, 3, 4, 5}

	BOOST_CHECK_EQUAL(all_of(g_v.begin(), g_v.end(), isOdd), false);
	BOOST_CHECK_EQUAL(all_of(g_v.begin(), g_v.end(), lessThan10), true);
	BOOST_CHECK_EQUAL(all_of(g_v.begin(), g_v.begin(), lessThan10), true); //empty range, return true

	BOOST_CHECK_EQUAL(all_of(g_v_class.begin(), g_v_class.end(), isOddClass), false);
	BOOST_CHECK_EQUAL(all_of(g_v_class.begin(), g_v_class.end(), lessThan10Class), true);

	BOOST_CHECK_EQUAL(all_of_equal(g_v.begin(), g_v.end(), 11), false);
	BOOST_CHECK_EQUAL(all_of_equal(g_v.begin() + 2, g_v.begin() + 2, 3), true);
	BOOST_CHECK_EQUAL(all_of_equal(g_v.begin(), g_v.begin(), 3), true);//empty range, return true

}

BOOST_AUTO_TEST_CASE(t_any_of){

	// g_v value:{1, 2, 3, 4, 5}

	BOOST_CHECK_EQUAL(any_of(g_v.begin(), g_v.end(), isOdd), true);
	BOOST_CHECK_EQUAL(any_of(g_v.begin(), g_v.end(), lessThan10), true);
	BOOST_CHECK_EQUAL(any_of(g_v.begin(), g_v.begin(), lessThan10), false); //empty range, return false

	BOOST_CHECK_EQUAL(any_of(g_v_class.begin(), g_v_class.end(), isOddClass), true);
	BOOST_CHECK_EQUAL(any_of(g_v_class.begin(), g_v_class.end(), lessThan10Class), true);

	BOOST_CHECK_EQUAL(any_of_equal(g_v.begin(), g_v.end(), nCount), true);

	BOOST_CHECK_EQUAL(any_of_equal(g_v.begin(), g_v.end(), 11), false);
	BOOST_CHECK_EQUAL(any_of_equal(g_v.begin() + 2, g_v.begin() + 2, 3), false);
	BOOST_CHECK_EQUAL(any_of_equal(g_v.begin(), g_v.begin(), 3), false);//empty range, return false

}

BOOST_AUTO_TEST_CASE(t_none_of){

	// g_v value:{1, 2, 3, 4, 5}

	BOOST_CHECK_EQUAL(none_of(g_v.begin(), g_v.end(), isOdd), false);
	BOOST_CHECK_EQUAL(none_of(g_v.begin(), g_v.end(), lessThan10), false);
	BOOST_CHECK_EQUAL(none_of(g_v.begin(), g_v.begin(), lessThan10), true); //empty range, return true

	BOOST_CHECK_EQUAL(none_of(g_v_class.begin(), g_v_class.end(), isOddClass), false);
	BOOST_CHECK_EQUAL(none_of(g_v_class.begin(), g_v_class.end(), lessThan10Class), false);

	BOOST_CHECK_EQUAL(none_of_equal(g_v.begin(), g_v.end(), 11), true);
	BOOST_CHECK_EQUAL(none_of_equal(g_v.begin(), g_v.begin() + 3, 3), false);
	BOOST_CHECK_EQUAL(none_of_equal(g_v.begin(), g_v.begin(), 3), true);//empty range, return true

}

BOOST_AUTO_TEST_CASE(t_one_of){

	// g_v value:{1, 2, 3, 4, 5}

	BOOST_CHECK_EQUAL(one_of(g_v.begin(), g_v.end(), isOdd), false);
	BOOST_CHECK_EQUAL(one_of(g_v.begin(), g_v.end(), lessThan10), false);
	BOOST_CHECK_EQUAL(one_of(g_v.begin(), g_v.begin(), lessThan10), false); //empty range, return false

	BOOST_CHECK_EQUAL(one_of(g_v_class.begin(), g_v_class.end(), isOddClass), false);
	BOOST_CHECK_EQUAL(one_of(g_v_class.begin(), g_v_class.end(), lessThan10Class), false);

	BOOST_CHECK_EQUAL(one_of_equal(g_v.begin(), g_v.end(), 11), false);
	BOOST_CHECK_EQUAL(one_of_equal(g_v.begin(), g_v.begin() + 3, 3), true);
	BOOST_CHECK_EQUAL(one_of_equal(g_v.begin(), g_v.begin(), 3), false);//empty range, return false

}

bool p1(int first, int second){
	return (first == second)?true:false;
}

bool p1Class(g_class* first,  g_class* second){

	return (first->a == second->a)?true:false;
}

BOOST_AUTO_TEST_CASE(t_is_permutation){

	/* is_permutation函数说明：序列1与序列2比较，如果序列1和序列2元素个数必须相同，只是排列顺序不同，则函数返回为true */

	// g_v  value:{1, 2, 3, 4, 5}
	// g_v2 value:{2, 3, 4}
	// g_v_class2 value:{5, 4, 2, 3}

	BOOST_CHECK_EQUAL(is_permutation(g_v.begin(), g_v.end(), g_v2.begin(), p1), true);

	BOOST_CHECK_EQUAL(is_permutation(g_v_class.begin() + 1, g_v_class.end(), g_v_class2.begin(), p1Class), true);
	BOOST_CHECK_EQUAL(is_permutation(g_v_class2.begin(), g_v_class2.end(), g_v_class.begin(), p1Class), false);

	// all empty ranges are permutations of each other
	BOOST_CHECK_EQUAL(is_permutation(g_v_class2.end(), g_v_class2.end(), g_v_class.end(), p1Class), true);
	BOOST_CHECK_EQUAL(is_permutation(g_v_class2.end(), g_v_class2.end(), g_v_class.begin(), p1Class), true);
}

BOOST_AUTO_TEST_CASE(t_equal){

	/* equal函数说明：序列1与序列2比较，如果序列1和序列2元素个数相同，元素值和排列顺序一样，则函数返回为true */

	// g_v  value:{1, 2, 3, 4, 5}
	// g_v3 value:{2, 3, 4}
	// g_v_class3 value:{2, 3, 4}

	using namespace boost::algorithm;
	BOOST_CHECK_EQUAL(equal(g_v.begin(), g_v.end(), g_v3.begin(), g_v3.end(), p1), false); 		// 序列元素个数不同
	BOOST_CHECK_EQUAL(equal(g_v.begin() + 1, g_v.end(), g_v3.begin(), g_v3.end(), p1), false);  // 序列元素个数不同
	BOOST_CHECK_EQUAL(equal(g_v.begin(), g_v.begin() + 3, g_v3.begin(), g_v3.end(), p1), false);  // 序列元素个数相同，元素值和排列顺序不同
	BOOST_CHECK_EQUAL(equal(g_v.begin() + 1, g_v.begin() + 4, g_v3.begin(), g_v3.end(), p1), true);

	BOOST_CHECK_EQUAL(equal(g_v_class.begin() + 1, g_v_class.end(), g_v_class3.begin(), g_v_class3.end(), p1Class), false); // 序列元素个数不同
	BOOST_CHECK_EQUAL(equal(g_v_class.begin() + 1, g_v_class.begin() + 4, g_v_class3.begin(), g_v_class3.end(), p1Class), true);

	// empty sequences are alway equal to each other,
	// equal returns true for two empty ranges, no matter what predicate is passed to test against.
	BOOST_CHECK_EQUAL(equal(g_v_class.end(), g_v_class.end(), g_v_class3.begin(),g_v_class3.begin(), p1Class), true);
	BOOST_CHECK_EQUAL(equal(g_v_class.end(), g_v_class.end(), g_v_class3.end(),g_v_class3.end(), p1Class), true);
}

BOOST_AUTO_TEST_CASE(t_mismatch){

	/* mismatch函数说明：序列1与序列2比较，返回序列1和序列2元素第一次不匹配的pair位置对 */

	// g_v  value:{1, 2, 3, 4, 5}
	// g_v3 value:{2, 3, 4}
	// g_v_class3 value:{2, 3, 4}

	typedef vector<g_class*>::iterator ITER;
	using namespace boost::algorithm;
	{
		std::pair<ITER, ITER> pa = mismatch(g_v_class.begin(), g_v_class.end(), g_v_class3.begin(), g_v_class3.end(), p1Class);
		BOOST_CHECK_EQUAL((*pa.first)->count, 1);
		BOOST_CHECK_EQUAL((*pa.first)->a, 1);
		BOOST_CHECK_EQUAL((*pa.second)->count, 3);
		BOOST_CHECK_EQUAL((*pa.second)->a, 2);
	}

	{
		std::pair<ITER, ITER> pa = mismatch(g_v_class.begin() + 1, g_v_class.end(), g_v_class3.begin(), g_v_class3.end(), p1Class);
		BOOST_CHECK_EQUAL((*pa.first)->count, 1);
		BOOST_CHECK_EQUAL((*pa.first)->a, 5);
		BOOST_CHECK(pa.second == g_v_class3.end());
	}

	// If the sequences are equal (or both are empty), then mismatch returns the end iterators of both sequences.
	{
		std::pair<ITER, ITER> pa = mismatch(g_v_class.begin(), g_v_class.begin(), g_v_class3.begin(), g_v_class3.end(), p1Class);
		BOOST_CHECK(pa.first == g_v_class.begin());
		BOOST_CHECK(pa.second == g_v_class3.begin());
	}

	{
		std::pair<ITER, ITER> pa = mismatch(g_v_class.begin(), g_v_class.begin(), g_v_class3.end(), g_v_class3.end(), p1Class);
		BOOST_CHECK(pa.first == g_v_class.begin());
		BOOST_CHECK(pa.second == g_v_class3.end());
	}

	{
		std::pair<ITER, ITER> pa = mismatch(g_v_class.begin(), g_v_class.begin(), g_v_class3.begin(), g_v_class3.end(), p1Class);
		BOOST_CHECK(pa.first == g_v_class.begin());
		BOOST_CHECK_EQUAL((*pa.second)->count, 3);
		BOOST_CHECK_EQUAL((*pa.second)->a, 2);
	}
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
