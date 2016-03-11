/*
 * stl_standard.cpp
 *
 *  Created on: 2016-1-1
 *      Author: root
 */

#include <boost/test/unit_test.hpp>

#include <iostream>
#include <vector>				// 数组
#include <list>					// list列表(双端链表),可以高效的前后新增元素
#include <deque>				// deque双端队列(支持开始端高效插入的向量),基本操作同vector一样
#include <map>					// 映射
#include <set>					// 集合key = value, 无重复元素，有比较器
#include <bitset>				// 位集合
using namespace std;

#include <boost/foreach.hpp>
#define foreach_         BOOST_FOREACH
#define foreach_r_       BOOST_REVERSE_FOREACH

#include <boost/assign.hpp>
using namespace boost::assign;
using namespace boost;

BOOST_AUTO_TEST_SUITE(stl_standard)

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
struct s_stl_standard_fixture
{
	//测试用例执行前调用
	s_stl_standard_fixture()
	{
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
	~s_stl_standard_fixture()
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

BOOST_FIXTURE_TEST_SUITE(sxx,s_stl_standard_fixture)

struct is_find_3
{
    bool operator()( int i ) const {
    	return (i == 3)?true:false; }
};

bool find1(int i){
	return (i == 3)?true:false;
}

bool find1Class(g_class*& v){
	return (v->a == 3)?true:false;
}

/* vector 数组 */
BOOST_AUTO_TEST_CASE(t_vector){

	// 新增数据
	g_v.push_back(6);

	// 获得数据
	BOOST_CHECK_EQUAL(g_v[5], 6);

	// 查询数据
	BOOST_CHECK_EQUAL(*(std::find(g_v.begin(), g_v.end(), 3)), 3);
	BOOST_CHECK_EQUAL(*(std::find_if(g_v.begin(), g_v.end(), is_find_3())), 3);
	BOOST_CHECK_EQUAL(*(std::find_if(g_v.begin(), g_v.end(), find1)), 3);
	BOOST_CHECK(std::find(g_v.begin(), g_v.end(), 10) == g_v.end());    // no find then return end()
	BOOST_CHECK_EQUAL((*(std::find_if(g_v_class.begin(), g_v_class.end(), find1Class)))->a, 3);

	// 删除数据
	typedef vector<int>::iterator ITER;
	ITER iter = std::find(g_v.begin(), g_v.end(), 3);
	if(iter != g_v.end()){
		g_v.erase(iter);
	}

	// 正向遍历
	foreach_(int& i, g_v){

		// output:{1, 2, 4, 5, 6}
		cout << "i=" << i << endl;
	}
	foreach_(g_class*& c, g_v_class){

		// output:{1, 2, 3, 4, 5, 6}
		cout << "c=" << c->a << endl;
	}

	// 反向遍历
	foreach_r_(g_class*& c, g_v_class){

		// output:{1, 2, 3, 4, 5, 6}
		cout << "reverse c=" << c->a << endl;
	}
}

/* list列表(双端链表),可以高效的前后新增元素 */
BOOST_AUTO_TEST_CASE(t_list){

	list<int> ls(4, 1);				//构造含有4个元素都等于1的列表

	ls.push_back(2);				//push_back()新增元素到最后
	ls.push_front(0);				//push_front()新增元素到开始

	// 遍历
	foreach_(int n, ls)
	{
		// output: {0, 1, 1, 1, 1, 2}
		cout << "n=" << n << endl;
	}
	cout << endl;

	ls.pop_front();					//pop_front()删除开始的一个元素
	ls.pop_back();					//pop_back()删除最后的一个元素

	ls.erase(ls.begin());			//erase()删除一个元素

	// 遍历
	foreach_(int n, ls)
	{
		// output: {1, 1, 1}
		cout << "m=" << n << endl;
	}
	cout << endl;

	list<int> ls2 = list_of(2) (3);
	ls.splice(ls.begin(), ls2);			//splice(pos, x)将list剪切到pos之前

	list<int>::iterator iter = find(ls.begin(), ls.end(), 1);
	ls.insert(iter, 5);					//insert(pos, v)将元素v插入到pos之前

	// 遍历
	foreach_(int n, ls)
	{
		// output: {2, 3, 5, 1, 1, 1}
		cout << "k=" << n << endl;
	}
	cout << endl;

	// 查找
	BOOST_CHECK_EQUAL(*(find(ls.begin(), ls.end(), 3)), 3);
	BOOST_CHECK(find(ls.begin(), ls.end(), 11) == ls.end());
}

/* deque双端队列(支持开始端高效插入的向量),基本操作同vector一样 */
BOOST_AUTO_TEST_CASE(t_deque){

	deque<int> dq = list_of(1) (2) (3);

	dq.push_back(4);
	dq.push_front(0);

	// 遍历
	foreach_(int n, dq) {
		// output: {0, 1, 2, 3, 4}
		cout << "n=" << n << endl;
	}
	cout << endl;

	dq.pop_back();
	// 遍历
	foreach_(int n, dq) {
		// output: {0, 1, 2, 3}
		cout << "m=" << n << endl;
	}
	cout << endl;

	dq.pop_front();
	// 遍历
	foreach_(int n, dq) {
		// output: {1, 2, 3}
		cout << "k=" << n << endl;
	}
	cout << endl;

}

/* stack堆栈,先进后出 */
BOOST_AUTO_TEST_CASE(t_stack){

	stack<int> stk = list_of(1) (2) (3) (4) (5).to_adapter();

	stk.push(6);							// push(x)入栈

	while(!stk.empty())						// empty()判断栈是否为空，为空返回true
	{
		// output: {6, 5, 4, 3, 2, 1}
		cout << "n=" << stk.top() << endl;  // top()获得栈中第1个元素
		stk.pop();							// pop()第1个元素出栈
	}

}

/* queue队列,先进先出 */
BOOST_AUTO_TEST_CASE(t_queue){
	queue<int> qu = list_of(1) (2) (3) (4) (5).to_adapter();

	qu.push(6);								// push(x)入队列

	while(!qu.empty())						// empty()判断队列是否为空，为空返回true
	{
		// output: {1, 2, 3, 4, 5, 6}
		cout << "n=" << qu.front() << endl;	// front()获得队列中第1个元素
		qu.pop();							// pop()第1个元素出队列
	}
}

/* priority_queue优先级队列(按照一定算法,自动对队列中的元素进行排序) */

// 自定义比较器
template<typename T>
struct more_than_compare
{
public:
	bool operator()(const T& a,const T& b)
	{
		return a > b;
	}
};

BOOST_AUTO_TEST_CASE(t_priority_queue){

	priority_queue<int> pq = list_of(1) (4) (2) (0) (3) (6) (5).to_adapter();

	pq.push(10);							// push(x)入队列

	// 遍历
	while(!pq.empty())						// empty()判断队列是否为空，为空返回true
	{
		// output: {10, 6, 5, 4, 3, 2, 1, 0}
		cout << "n=" << pq.top() << endl;	// top()获得队列中第1个元素,元素只读
		pq.pop();							// pop()第1个元素出队列
	}
	cout << endl;

	// 使用比较器
	priority_queue<int, vector<int>, more_than_compare<int> > pq1 = list_of(1) (4) (2) (0) (3) (6) (5).to_adapter();

	//遍历
	while(!pq1.empty())
	{
		// output: {0, 1, 2, 3, 4, 5, 6}
		cout << "m=" << pq1.top() << endl;
		pq1.pop();
	}
}

/* map映射,1个key -> 1个value */
BOOST_AUTO_TEST_CASE(t_map){

	typedef map<int, string>::value_type pair;
	typedef map<int, string>::iterator map_iterator;

	map<int, string> m = map_list_of(1, "this is 1") (2, "this is 2") (3, "this is 3");

	m[4] = "this is 4";
	m.insert(make_pair(5, "this is 5"));				// insert(pair(k, v))插入pair到map

	BOOST_CHECK(m.find(8) == m.end());					// find(k)查找元素
	BOOST_CHECK(m.find(4) != m.end());

	// 遍历
	foreach_(pair& n, m)
	{
		// output: {this is 1,this is 2,this is 3,this is 4,this is 5}
		cout << "n:" << n.second << endl;
	}
	cout << endl;

	map_iterator iter = m.find(2);
	if(iter != m.end())
		m.erase(iter);									// erase(iter)删除元素

	// 遍历
	foreach_(pair& n, m)
	{
		// output: {this is 1,this is 3,this is 4,this is 5}
		cout << "k:" << n.second << endl;
	}
	cout << endl;

	// 定义比较器
	map<int, string,more_than_compare<int> > m1 = map_list_of(1, "this is 1") (2, "this is 2") (3, "this is 3");

	//遍历
	foreach_(pair& n, m1)
	{
		// output: {this is 3,this is 2,this is 1}
		cout << "m:" << n.second << endl;
	}
	cout << endl;
}

/* multimap多映射,1个key -> n个value, 有比较器 */
BOOST_AUTO_TEST_CASE(t_multimap){

	typedef multimap<int, string>::value_type multimap_pair;
	typedef multimap<int, string>::iterator multimap_iterator;

	multimap<int, string> ma = map_list_of(1, "this is 1 first") (2, "this is 2") (3, "this is 3") (1, "this is 1 second");

	ma.insert(make_pair(1, "this is 1 third"));											// insert(pair(key, value))插入元素

	std::pair<multimap_iterator, multimap_iterator> rge = ma.equal_range(1);			// equal_range(key)查找key对应的多个值

	// 遍历
	foreach_(multimap_pair& pos, rge){
		// output : {this is 1 first, this is 1 second, this is 1 third}
		cout << "pos:" << pos.second << endl;
	}
	cout << endl;

	ma.erase(1);																		// erase(key)删除元素
	foreach_(multimap_pair& pos, ma){
		// output : {this is 2, this is 3}
		cout << "pos2:" << pos.second << endl;
	}
	cout << endl;

	ma.erase(2);
	foreach_(multimap_pair& pos, ma){
		// output : {this is 3}
		cout << "pos3:" << pos.second << endl;
	}
	cout << endl;
}

/*
 * set集合, key = value, 无重复元素，有比较器
 * set集合容器：实现了红黑树的平衡二叉检索树的数据结构，插入元素时，它会自动调整二叉树的排列，
 * 把元素放到适当的位置，以保证每个子树根节点键值大于左子树所有节点的键值，小于右子树所有节点的键值；
 * 另外，还得保证根节点左子树的高度与右子树高度相等。平衡二叉检索树使用中序遍历算法，检索效率高于
 * vector、deque和list等容器，另外使用中序遍历可将键值按照从小到大遍历出来。
 * 构造set集合主要目的是为了快速检索，不可直接去修改键值
 */
BOOST_AUTO_TEST_CASE(t_set){

	set<int> s = list_of(1) (2) (3);

	s.insert(1);								// insert(v)插入元素,重复插入无效
	s.insert(1);

	//遍历
	foreach_(int n, s)
	{
		// output:{1, 2, 3}
		cout << "n:" << n << endl;
	}
	cout << endl;

	set<int, more_than_compare<int> > s2 = list_of(5) (1) (4) (2) (10);
	s2.insert(9);

	//遍历
	foreach_(int n, s2)
	{
		// output:{10, 9, 5, 4, 2, 1}
		cout << "m:" << n << endl;
	}
	cout << endl;


	BOOST_CHECK(s2.find(20) == s2.end());		// find(key)查找元素
	BOOST_CHECK_EQUAL(*(s2.find(2)), 2);

	s2.erase(2);								// erase(key)删除元素
	//遍历
	foreach_(int n, s2)
	{
		// output:{10, 9, 5, 4, 1}
		cout << "k:" << n << endl;
	}
	cout << endl;
}

/* multiset多重集合, 允许重复元素, 有比较器 */
BOOST_AUTO_TEST_CASE(t_multiset){

	multiset<int> ms = list_of(1) (2) (3) (1);

	//遍历
	foreach_(int n, ms)
	{
		// output:{1, 1, 2, 3}
		cout << "n:" << n << endl;
	}
	cout << endl;
}

/* bitset位集合,集合中每一个元素对应数值的每一位,它的值是0或1,不支持动态增长 */
BOOST_AUTO_TEST_CASE(t_bitset){

	short v = 5;
	bitset<32> b(v);

	// output: 0000 0000 0000 0000 0000 0000 0000 0101
	cout << b << endl;

	BOOST_CHECK(b.test(0));			//test(pos)测试n位是否为真(1)
	BOOST_CHECK(!b.test(1));
	BOOST_CHECK(b.test(2));

	b.set(3);						//set(pos)设置n位为1
	b.set(1);
	b.reset(0);						//reset(pos)设置n位为0
	b.reset(2);

	// output: 0000 0000 0000 0000 0000 0000 0000 1010
	cout << b << endl;
	BOOST_CHECK(b.to_ulong() == 10);	//to_ulong()转换为long

	b.set();
	BOOST_CHECK(b.any());			//any()判断集合中是否包含1
	BOOST_CHECK(!b.none());			//none()判断集合中是否包含0

	// output: 1111 1111 1111 1111 1111 1111 1111 1111
	cout << b << endl;

	b.flip();						//flip()反转所有位
	// output: 0000 0000 0000 0000 0000 0000 0000 0000
	cout << b << endl;
	BOOST_CHECK(!b.any());

	// 支持位操作
	bitset<8> b1(5);				//0000 0101
	bitset<8> b2(10);				//0000 1010

	BOOST_CHECK((b1 | b2).to_ulong() == 15);
	BOOST_CHECK((b1 & b2).to_ulong() == 0);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
