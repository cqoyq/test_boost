/*
 * boost_minmax.cpp
 *
 *  Created on: 2016-1-2
 *      Author: root
 */

#include <boost/test/unit_test.hpp>

#include <iostream>
using namespace std;

#include <boost/algorithm/minmax.hpp>					//minmax使用
#include <boost/algorithm/minmax_element.hpp>			//minmax_element使用
using namespace boost;

BOOST_AUTO_TEST_SUITE(boost_minmax)

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

// 测试夹具，每个测试用例都会调用
struct s_boost_minmax_fixture
{
	//测试用例执行前调用
	s_boost_minmax_fixture()
	{
		g_v_class.push_back(new g_class(1, 1));
		g_v_class.push_back(new g_class(2, 1));
		g_v_class.push_back(new g_class(3, 1));
		g_v_class.push_back(new g_class(4, 1));
		g_v_class.push_back(new g_class(5, 1));
	}

	//测试用例测试后调用
	~s_boost_minmax_fixture()
	{
		vector<g_class*>::iterator iter = g_v_class.begin();
		while(iter != g_v_class.end()){

			delete (*iter);

			iter++;
		}
		g_v_class.clear();
	}
};

BOOST_FIXTURE_TEST_SUITE(sxx,s_boost_minmax_fixture)

bool more_than(g_class*& c1, g_class*& c2){
	return (c1->a >= c2->a)?false:true;
}

BOOST_AUTO_TEST_CASE(t_minmax){

	tuple<int, int> res = minmax(10, 8);				// minmax(v1, v2)构建tuple对象
	BOOST_CHECK_EQUAL(res.get<0>(), 8);					// tuple第1个元素是小值
	BOOST_CHECK_EQUAL(res.get<1>(), 10);				// tuple第2个元素是大值


	// minmax_element函数接受区间作为参数,查找其中第一次出现的最大最小值,并以std::pair形式返回其迭代器.
	typedef vector<g_class*>::const_iterator iterator;
	std::pair<iterator, iterator> my = minmax_element(g_v_class.begin(), g_v_class.end(), more_than);
	// output: "1,5"
	cout << (*(my.first))->a << "," << (*(my.second))->a << endl;

	// xxx_min/max_yyy_max/min_element(),其中xxx和yyy可以是first或second
	// * first_min_element(first, last)									//查找第一个最小值
	// * first_min_element(first, last, comp)

	// * last_min_element(first, last)									//查找最后一个最小值
	// * last_min_element(first, last, comp)

	// * first_max_element(first, last)									//查找第一个最大值
	// * first_max_element(first, last, comp)

	// * last_max_element(first, last)									//查找最后一个最大值
	// * last_max_element(first, last, comp)

	// * first_min_first_max_element(first, last)						//查找第一个最大、最小值
	// * first_min_first_max_element(first, last, comp)

	// * first_min_last_max_element(first, last)						//查找第一个最小值,最后一个最大值
	// * first_min_last_max_element(first, last, comp)

	// * last_min_first_max_element(first, last)						//查找最后一个最小值,第一个最大值
	// * last_min_first_max_element(first, last, comp)

	// * last_min_last_max_element(first, last)							//查找最后一个最小值,最后一个最大值
	// * last_min_last_max_element(first, last, comp)

}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
