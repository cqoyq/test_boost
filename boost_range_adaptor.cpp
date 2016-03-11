/*
 * boost_range.cpp
 *
 *  Created on: 2016-1-4
 *      Author: root
 */

#include <boost/test/unit_test.hpp>

#include <iostream>
using namespace std;

#include <boost/foreach.hpp>
#define foreach_         BOOST_FOREACH
#define foreach_r_       BOOST_REVERSE_FOREACH

#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>

/*
 * Three types of objects are currently supported by the library:
    standard-like containers
    std::pair<iterator,iterator>
    built-in arrays
 */
#include <boost/range/adaptor/adjacent_filtered.hpp>	 	// 相邻两元素比较，满足条件返回true，则获得后一个元素
#include <boost/range/adaptor/copied.hpp>				 	// copy序列rng之间的元素
#include <boost/range/adaptor/filtered.hpp>					// 根据条件过滤元素
#include <boost/range/adaptor/indexed.hpp>					// 索引元素
#include <boost/range/adaptor/indirected.hpp>				// 指针处理
#include <boost/range/adaptor/map.hpp>						// 处理key和value
#include <boost/range/adaptor/replaced.hpp>					// 替换元素
#include <boost/range/adaptor/replaced_if.hpp>
#include <boost/range/adaptor/reversed.hpp>					// 反转元素
#include <boost/range/adaptor/strided.hpp>					// 切片序列
#include <boost/range/adaptor/transformed.hpp>				// 改变元素值
#include <boost/range/adaptor/uniqued.hpp>					// 获得唯一元素的序列(自动削除相邻的相同元素，保留第1个相同元素)
using namespace boost::adaptors;

BOOST_AUTO_TEST_SUITE(boost_range_adaptor)

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

/* ------------------ Map ---------------------- */
map<int, string> g_map;

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

		g_map.insert(make_pair(1, "this is one"));
		g_map.insert(make_pair(2, "this is two"));
		g_map.insert(make_pair(3, "this is three"));
		g_map.insert(make_pair(4, "this is four"));
		g_map.insert(make_pair(5, "this is five"));
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

		g_map.clear();
	}
};

BOOST_FIXTURE_TEST_SUITE(sxx,s_boost_range_fixture)

struct is_equal_2
{
    bool operator()( int a, int b ) const {
    	return (b - a == 2)?true:false; }
};

struct is_equalClass_2
{
    bool operator()( g_class*& x, g_class*& y ) const { return (y->a - x->a == 2)?true:false; }
};

/* adjacent_filtered: 相邻两元素比较，满足条件返回true，则获得后一个元素
 * Throws: Whatever the copy constructor of bi_pred might throw.
 * Range Category: Forward Range
 * Return Type: boost::adjacent_filtered_range<decltype(rng), decltype(bi_pred)>
 * Returned Range Category: The minimum of the range category of rng and Forward Range
*/
BOOST_AUTO_TEST_CASE(t_adjacent_filtered){

	// g_v2 : {1, 3, 4, 5, 7}
	// g_v_class2 : {1, 2, 4, 5, 7}

	foreach_(int n, g_v2 | adjacent_filtered(is_equal_2())){
		// output: {1, 3, 7}
		cout << "n:" << n << endl;
	}
	cout << endl;

	// 查看源值
	foreach_(int n, g_v2){
		// output: {1, 3, 4, 5, 7}
		cout << "k:" << n << endl;
	}
	cout << endl;

	foreach_(g_class*& n, g_v_class2 | adjacent_filtered(is_equalClass_2())){
		// output: {1, 4, 7}
		cout << "m:" << n->a << endl;
	}
}

/*
 *  copied: 返回序列rng之间的元素
 *  use: rng | boost::adaptors::copied(n, m)
 *  Precondition: 0 <= n && n <= m && m < distance(rng)
 *  Returns: A new iterator_range that holds the sliced range [n,m) of the original range.
 *  Range Category: Random Access Range
 *  Returned Range Category: Random Access Range
 */

BOOST_AUTO_TEST_CASE(t_copied){

	// g_v : {1, 2, 3, 4, 5}

	foreach_(int n, g_v | copied(1, 3)){
		// output: {2, 3}
		cout << "n:" << n << endl;
	}
	cout << endl;
}

/*
 *  filtered: 根据条件过滤元素
 *  use: rng | boost::adaptors::filtered(pred)
 * 	Precondition: The value_type of the range is convertible to the argument type of pred.
 * 	Postcondition: For all elements [x] in the returned range, pred(x) is true.
 * 	Throws: Whatever the copy constructor of pred might throw.
 * 	Range Category: __singlepass_range__
 * 	Range Return Type: boost::filtered_range<decltype(rng)>
 * 	Returned Range Category: The minimum of the range category of rng and Bidirectional Range
 */
struct is_odd{
	bool operator()( int x ) const { return (x % 2 == 1)?true:false; }
};

struct is_odd_class{
	bool operator()( int x ) const { return (x % 2 == 1)?true:false; }
};

BOOST_AUTO_TEST_CASE(t_filtered){

	// g_v : {1, 2, 3, 4, 5}

	foreach_(int n, g_v | filtered(is_odd())){
		// output: {1, 3, 5}
		cout << "n:" << n << endl;
	}
	cout << endl;
}

/*
 * indexed: 索引
 * use: rng | boost::adaptors::indexed()
 *      rng | boost::adaptors::indexed(start_index)    // start_index is 0 by default.
 * Purpose: Adapt rng to return elements that have the corresponding value from rng and a numeric index.
 * Returns: A range adapted to return both the element and the associated index. The returned range has elements of type:
 *          boost::range::index_value<
    			typename boost::range_reference<decltype(rng)>::type,
    			typename boost::range_difference<decltype(rng)>::type
			>
 *  Range Category: Single Pass Range
 *  Range Return Type: boost::indexed_range<decltype(rng)>
 *  Returned Range Category: The range category of rng if and only if rng is not a Bidirectional Range.
 	 If rng is a Bidirectional Range then the returned range category is Forward Range.
 	 The rationale for the demotion of Bidirectional Range inputs to Forward Range is to avoid slow calculation of indices for boost::end(rng).
 *
 */
BOOST_AUTO_TEST_CASE(t_indexed){

	// g_v_class : {1, 2, 3, 4, 5}

	typedef boost::range::index_value<g_class*&> iv;
	foreach_(const iv& n, g_v_class | indexed()){
		// output:
		// n:1,index:0
		// n:2,index:1
		// n:3,index:2
		// n:4,index:3
		// n:5,index:4

		cout << "n:" << n.value()->a << ",index:" << n.index() << endl;
	}
	cout << endl;

	foreach_(const iv& n, g_v_class | indexed(1)){
		// output:
		// n:1,index:1
		// n:2,index:2
		// n:3,index:3
		// n:4,index:4
		// n:5,index:5
		cout << "m:" << n.value()->a << ",index:" << n.index() << endl;
	}
	cout << endl;
}

/*
 * indirected: 指针处理
 * use: rng | boost::adaptors::indirected
 * Precondition: The value_type of the range defines unary operator*()
 * Postcondition: For all elements x in the returned range, x is the result of *y where y is the corresponding element in the original range.
 * Range Category: Single Pass Range
 * Range Return Type: boost::indirected_range<decltype(rng)>
 * Returned Range Category: The range category of rng
 */
BOOST_AUTO_TEST_CASE(t_indirected){

	// g_v_class : {1, 2, 3, 4, 5}

	foreach_(g_class& n, g_v_class | indirected){

		cout << "n:" << n.a << endl;
	}
}

/*
 * map_keys: 处理std::map的key
 * use: rng | boost::adaptors::map_keys
 * Precondition: The value_type of the range is an instantiation of std::pair.
 * Postcondition: For all elements x in the returned range, x is the result of y.first where y is the corresponding element in the original range.
 * Range Category: Single Pass Range
 * Range Return Type: boost::select_first_range<decltype(rng)>
 * Returned Range Category: The range category of rng.
 */
BOOST_AUTO_TEST_CASE(t_map_keys){

	// g_map : {[1,this is one], [2,this is two], [3,this is three], [4,this is four], [5,this is five]}

	foreach_(const int key, g_map | map_keys){

		// output: {1, 2, 3, 4, 5}
		cout << "key:" << key << endl;
	}
}

/*
 * map_values: 处理std::map的value
 * use: rng | boost::adaptors::map_values
 * Precondition: The value_type of the range is an instantiation of std::pair.
 * Postcondition: For all elements x in the returned range, x is the result of y.second where y is the corresponding element in the original range.
 * Range Category: Single Pass Range
 * Range Return Type: for constant ranges, boost::select_second_const<decltype(rng)> otherwise boost:select_second_mutable<decltype(rng)>
 * Returned Range Category: The range category of rng.
 */
BOOST_AUTO_TEST_CASE(t_map_values){

	// g_map : {[1,this is one], [2,this is two], [3,this is three], [4,this is four], [5,this is five]}

	foreach_(string& value, g_map | map_values){

		// output:
		// value:this is one
		// value:this is two
		// value:this is three
		// value:this is four
		// value:this is five
		cout << "value:" << value << endl;
		value.append("1");                     // modify value
	}
	cout << endl;

	foreach_(string& value, g_map | map_values){

		// output:
		// value:this is one1
		// value:this is two1
		// value:this is three1
		// value:this is four1
		// value:this is five1
		cout << "value:" << value << endl;

	}
}

/*
 * replaced: 替换元素
 * use: rng | boost::adaptors::replaced(old_value, new_value)
 * Precondition:
    new_value is convertible to the value_type of the range.
    old_value is convertible to the value_type of the range.
 * Postcondition: For all elements x in the returned range,
   the value x is equal to the value of (y == old_value) ? new_value : y where y is the corresponding element in the original range.
 * Range Category: Single Pass Range
 * Range Return Type: boost::replaced_range<decltype(rng)>
 * Returned Range Category: The range category of rng.
 */
BOOST_AUTO_TEST_CASE(t_replaced){

	// g_v3 : {1, 3, 4, 3, 6}

	foreach_(int n, g_v3 | replaced(3, 2)){
		// output: {1, 2, 4, 2, 6}
		cout << "n:" << n << endl;
	}
	cout << endl;
}

/*
 * replaced_if: 替换元素
 * use: rng | boost::adaptors::replaced_if(pred, new_value)
 * Precondition:
    The range value_type is convertible to the argument type of pred.
    new_value is convertible to the value_type of the range.
 * Postconditions: For all elements x in the returned range,
   the value x is equal to the value of pred(y) ? new_value : y where y is the corresponding element in the original range.
 * Range Category: Single Pass Range
 * Range Return Type: boost::replaced_if_range<decltype(rng)>
 * Returned Range Category: The range category of rng.
 */

BOOST_AUTO_TEST_CASE(t_replaced_if){

	// g_v : {1, 2, 3, 4, 5}

	foreach_(int n, g_v | replaced_if(is_odd(), 10)){
		// output: {10, 2, 10, 4, 10}
		cout << "n:" << n << endl;
	}
	cout << endl;

	foreach_(int n, g_v ){
		// output: {1, 2, 3, 4, 5}
		cout << "m:" << n << endl;
	}
}

/*
 * reversed: 反转元素
 * use: rng | boost::adaptors::reversed
 * Returns: A range whose iterators behave as if they were the original iterators wrapped in reverse_iterator.
 * Range Category: Bidirectional Range
 * Range Return Type: boost::reversed_range<decltype(rng)>
 * Returned Range Category: The range category of rng.
 */
BOOST_AUTO_TEST_CASE(t_reversed){

	// g_v : {1, 2, 3, 4, 5}

	foreach_(int n, g_v | reversed){
		// output: {5, 4, 3, 2, 1}
		cout << "n:" << n << endl;
	}
	cout << endl;
}

/*
 * sliced: 获得切片元素序列
 * use: rng | boost::adaptors::sliced(n, m)
 * Precondition: 0 <= n && n <= m && m < distance(rng)
 * Returns: make_range(rng, n, m)
 * Range Category: Random Access Range
 * Range Return Type: boost::sliced_range<decltype(rng)>
 * Returned Range Category: Random Access Range
 */
BOOST_AUTO_TEST_CASE(t_sliced){

	// g_v_class : {1, 2, 3, 4, 5}

	foreach_(g_class*& n, g_v_class | sliced(1,3)){
		// output: {2, 3}
		cout << "n:" << n->a << endl;
	}
	cout << endl;

	vector<g_class*> v1;
	boost::push_back(v1, g_v_class | sliced(1,3));

	foreach_(g_class*& n, v1){
		// output: {2, 3}
		cout << "n:" << n->a << endl;
	}
}

/*
 * strided: 步进元素
 * use: rng | boost::adaptors::strided(n)
 * Precondition: 0 <= n.
 * Returns: A new range based on rng where traversal is performed in steps of n.
 * Range Category: Single Pass Range
 * Returned Range Category: The range category of rng.
 */
BOOST_AUTO_TEST_CASE(t_strided){
	// g_v_class : {1, 2, 3, 4, 5}

	foreach_(g_class*& n, g_v_class | strided(2)){

		// output: {1, 3, 5}
		cout << "n:" << n->a << endl;
	}
}

/*
 * transformed: 改变元素值
 * use: rng | boost::adaptors::transformed(fun)
 * Precondition: The value_type of the range is convertible to the argument type of fun.
 * Postcondition: For all elements x in the returned range, x is the result of fun(y) where y is the corresponding element in the original range.
 * Range Category: Single Pass Range
 * Range Return Type: boost::transformed_range<decltype(rng)>
 * Returned Range Category: The range category of rng.
 */

struct double_int_class
{
    typedef g_class*& result_type;
    g_class*& operator()(g_class*& x) const {
    	x->a = (x->a) * 2;
    	return x;
    }
};

BOOST_AUTO_TEST_CASE(t_transformed){

	// g_v_class : {1, 2, 3, 4, 5}

//	vector<g_class*> v1;
//	boost::push_back(v1, g_v_class | strided(2) | transformed(double_int_class()));

	foreach_(g_class*& n, g_v_class | strided(2) | transformed(double_int_class())){
		// output: {2, 6, 10}
		cout << "n:" << n->a << endl;
	}
	cout << endl;

	foreach_(g_class*& n, g_v_class){
		// output: {2, 2, 6, 4, 10}
		cout << "m:" << n->a << endl;
	}
}

/*
 * uniqued: 获得唯一元素的序列(自动削除相邻的相同元素)
 * use: rng | boost::adaptors::uniqued
 * Precondition: The value_type of the range is comparable with operator==().
 * Postcondition: For all adjacent elements [x,y] in the returned range, x==y is false.
 * Range Category: Forward Range
 * Range Return Type: boost::uniqued_range<decltype(rng)>
 * Returned Range Category: The minimum of the range concept of rng and Forward Range.
 */
BOOST_AUTO_TEST_CASE(t_uniqued){

	// g_v4 : {1, 3, 3, 4, 6}

	foreach_(int n, g_v4 | uniqued){
		// output: {1, 3, 4, 6}
		cout << "n:" << n << endl;
	}
	cout << endl;
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
