/*
 * boost_container.cpp
 *
 *  Created on: 2016-1-10
 *      Author: root
 */

#include <boost/test/unit_test.hpp>

#include <iostream>
using namespace std;

#include <boost/foreach.hpp>
#define foreach_         BOOST_FOREACH
#define foreach_r_       BOOST_REVERSE_FOREACH
#include <boost/lexical_cast.hpp>
#include <boost/range/adaptors.hpp>
using namespace boost::adaptors;
#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext.hpp>



// New container
#include <boost/bimap.hpp>				// key <--> value双向映射容器
#include <boost/circular_buffer.hpp>	// 循环缓冲区,支持标准容器操作但大小固定,当到达容器末尾时将自动循环利用容器另一端的空间.
#include <boost/dynamic_bitset.hpp>		// 支持动态增长的bitset
using namespace boost;

BOOST_AUTO_TEST_SUITE(boost_containers)

BOOST_AUTO_TEST_CASE(t_bitmap){

	bimap<int, string> bm;

	bm.left.insert(make_pair(1, "one"));
	bm.left.insert(bimap<int, string>::left_value_type(2, "two"));

	bm.right.insert(make_pair("three", 3));
	bm.right.insert(bimap<int, string>::right_value_type("four", 4));
	//bm.left.insert(make_pair(4, "four"));	//重复插入无效

	// traversal left view.
	string out;
	typedef bimap<int, string>::left_value_type bmp_left_pair;
	foreach_(bmp_left_pair n, bm.left)
	{
		out.append(n.second);	//pair(int, string)
		out.append(",");
	}
	BOOST_CHECK_EQUAL(out, "one,two,three,four,");

	// traversal right view
	out.clear();
	typedef bimap<int, string>::right_value_type bmp_right_pair;
	foreach_(bmp_right_pair n, bm.right)
	{
		out.append(n.first);	//pair(string, int)
		out.append(",");

	}
	BOOST_CHECK_EQUAL(out, "four,one,three,two,");

	// base operator
	BOOST_CHECK(bm.left.find(1) != bm.left.end());					//left.find()
	BOOST_CHECK(bm.left.size() == 4);								//left.size()
	BOOST_CHECK(bm.left.count(1) == 1);								//left.count()
	BOOST_CHECK_EQUAL(bm.left.find(1)->second, "one");				//left.find()->second

	// find and replace
	bimap<int, string>::left_iterator pos = bm.left.find(1);
	bm.left.replace_key(pos, 111);									//replace_key()替换key
	bm.left.replace_data(pos, "change one");						//replace_data()替换value
	BOOST_CHECK(bm.left.find(1) == bm.left.end());
	BOOST_CHECK_EQUAL(bm.left.find(111)->second, "change one");

	// erase
	bm.left.erase(pos);
	BOOST_CHECK(bm.left.find(111) == bm.left.end());


	// 标签访问,左右视图类型变为map_of<left>/map_of<right>,迭代器通过pos->get<left>()/pos->get<right>()访问
//	bimap<tagged<int, struct id>, tagged<string, struct name> > bm1;	//tagged<int, struct id>定义标签,包装int类型,标签名字是id
//
//
//	bm1.by<id>().insert(make_pair(1, "one"));							//by<tag>()访问左视图
//	bm1.by<id>().insert(bimap<tagged<int, struct id>, tagged<string, struct name> >::map_by<id>::value_type(2, "two"));
//
//	bm1.by<name>().insert(make_pair("three", 3));						//by<tag>()访问右视图
//	bm1.by<name>().insert(bimap<tagged<int, struct id>, tagged<string, struct name> >::map_by<name>::value_type("four", 4));
//
//	// assign赋值
//	typedef bimap<int, string> bm_t;
//	bm_t bm2 = assign::list_of<bm_t::relation>(1, "111") (2, "222");
}

BOOST_AUTO_TEST_CASE(t_circular_buffer){

	// 构造
	circular_buffer<int> cb(5);					//声明一个大小为5的循环缓冲区
	circular_buffer<int> cb1(3);
	cb1.push_back(1);
	cb1.push_back(2);
	cb1.push_back(3);
	circular_buffer<int> cb2(3);
	cb2.push_back(3);
	cb2.push_back(4);
	cb2.push_back(5);
	circular_buffer<int> cb3 = cb1;

	// 基本操作
	BOOST_CHECK(cb.empty());					//empty()是否为空

	cb.push_back(1);							//push_back()尾部插入元素
	cb.push_front(2);							//push_front()首部插入元素

	BOOST_CHECK_EQUAL(cb.front(), 2);			//front()返回缓冲区首部元素
	BOOST_CHECK_EQUAL(cb.back(), 1);			//back()返回缓冲区尾部元素

	cb.insert(cb.begin(), 3);					//insert()指定位置插入元素

	cb.pop_front();								//pop_front()弹出首部元素
	cb.pop_back();								//pop_back()弹出尾部元素

	BOOST_CHECK(cb.size() == 1);				//size()获得缓冲区元素数目
	BOOST_CHECK_EQUAL(cb[0], 2);				//operator[]获得指定元素

	cb.push_back(1);
	cb.push_front(3);
	string out;
	// 遍历
	foreach_(int n, cb){
		out.append(lexical_cast<string>(n));
		out.append(",");
	}
	BOOST_CHECK_EQUAL(out, "3,2,1,");

	// 比较
	BOOST_CHECK(cb1 < cb2);
	BOOST_CHECK(cb3 == cb1);

	// 特殊用法
	BOOST_CHECK(!cb.full());					//full()判断容器是否已满
	int* p = cb.linearize();					//linearize()获取线性数组
	BOOST_CHECK_EQUAL(p[0], 3);
	BOOST_CHECK_EQUAL(p[1], 2);
	BOOST_CHECK_EQUAL(p[2], 1);
	cb.rotate(cb.begin() + 1);					//rotate()从指定迭代器位置旋转整个缓冲区

	out.clear();
	foreach_(int n, cb){
		out.append(lexical_cast<string>(n));
		out.append(",");
	}
	BOOST_CHECK_EQUAL(out, "2,1,3,");
}

BOOST_AUTO_TEST_CASE(t_dynamic_bitset){

	// 构造
	{
		dynamic_bitset<> db1;								//空的dynamic_bitset
		dynamic_bitset<> db2(10);							//大小为10的dynamic_bitset
		dynamic_bitset<> db3(0x16, BOOST_BINARY(10101));	//BOOST_BINARY直接创立一个二进制常量
		dynamic_bitset<> db4(string("0100"));				//字符串构造,第0个元素存储的低位,也就是字符串最右边一位.
		dynamic_bitset<> db5(db3);							//拷贝构造
		dynamic_bitset<> db6 = db4;							//赋值构造

		BOOST_CHECK(db5.to_ulong() == 21);
		BOOST_CHECK(!(db4[0] | 0x0));	//0
		BOOST_CHECK(!(db4[1] | 0x0));	//0
		BOOST_CHECK(db4[2] | 0x0);		//1
		BOOST_CHECK(!(db4[3] | 0x0));	//0

		BOOST_CHECK(db3.size() == 22);//db3有22位
	}

	// 容器操作
	{
		dynamic_bitset<> db;								//空dynamic_bitset
		db.resize(5, true);									//resize()扩展原有的二进制位不变,新增的二进制位赋值为1.
		BOOST_CHECK((db.to_ulong() & BOOST_BINARY(11111)) == BOOST_BINARY(11111));

		db.resize(4);										//resize()收缩现有的二进制位保持,多余的二进制位抛弃.
		BOOST_CHECK((db.to_ulong() & BOOST_BINARY(1111)) == BOOST_BINARY(1111));

		BOOST_CHECK(db.size() == 4);						//size()返回dynamic_bitset现有位数
		db.clear();											//clear()删除所有位
		BOOST_CHECK(db.empty());							//empty()是否dynamic_bitset为空

		BOOST_CHECK_EQUAL(dynamic_bitset<>(64).num_blocks(), (uint)1);	//num_blocks()返回dynamic_bitset位占用的块的数目,默认unsigned long为8字节即64位一个block.
		BOOST_CHECK_EQUAL(dynamic_bitset<>(128).num_blocks(), (uint)2);
		BOOST_CHECK_EQUAL(dynamic_bitset<>(129).num_blocks(), (uint)3);

		db.push_back(true);									//push_back()添加新的位到容器末尾(二进制数的最高位)
		db.push_back(false);
		BOOST_CHECK_EQUAL(db.to_ulong(), (ulong)BOOST_BINARY(01));
		BOOST_CHECK_EQUAL(db.size(), uint(2));

		db.append(BOOST_BINARY(0010));						//append()把整数转换为二进制追加到dynamic_bitset末尾(最高位),追加的整数是按8字节unsigned long计算.
		BOOST_CHECK_EQUAL(db.to_ulong(), (ulong)BOOST_BINARY(001001));
		BOOST_CHECK_EQUAL(db.size(), sizeof(unsigned long)*8 + 2);	//0000 0000 0000 0000 0000 0000 0000 0010 01
	}

	// 位运算\比较运算
	{
		dynamic_bitset<> db(4, BOOST_BINARY(1010));

		db[0] &= 1;
		db[1] ^= 1;	//异或

		BOOST_CHECK(db.to_ulong() == BOOST_BINARY(1000));	//db : 1000
	}

	// 访问元素
	{
		dynamic_bitset<> db(4, BOOST_BINARY(1010));

		BOOST_CHECK(!db.test(0));				//test(n)测试n位是否为1
		BOOST_CHECK(db.any());					//any()容器中存在1,返回true
		BOOST_CHECK(!db.none());				//none()容器中不存在1,返回true

		db.set();	// db : 1111				//set(n)设置n位为1
		BOOST_CHECK(db.any());
		db.reset(); // db : 0000				//reset(n)设置n位为0
		BOOST_CHECK(db.none());

		db.flip(0);								//flip(n)反转n位
		db.flip(2);	// db : 0101
		BOOST_CHECK(db.test(0));
		BOOST_CHECK(!db.test(1));
		BOOST_CHECK(db.test(2));
		BOOST_CHECK(!db.test(3));

		BOOST_CHECK_EQUAL(db.count(), (uint)2);		//count()返回容器中1的数目

		// 查找
		db.clear();
		db.append(BOOST_BINARY(11001010));

		string out;
		size_t pos = db.find_first();				//find_first()返回找到第一个二进制位1的索引值
		while(pos < db.size() && pos >=0)
		{
			out.append(lexical_cast<string>(pos));
			out.append(",");

			pos = db.find_next(pos);				//find_next()返回下一个二进制位1的索引值
		}
		BOOST_CHECK_EQUAL(out, "1,3,6,7,");
	}

	// 类型转换
	{
		//如果超过了unsigned long的最大上限,可以使用to_string()进行转换
		dynamic_bitset<> db(4, BOOST_BINARY(1010));

		db.append(10);
		db.push_back(1);	//已经超过unsigned long上限

		string str;
		to_string(db, str);	//to_string()进行转换

		BOOST_CHECK_EQUAL(str, "100000000000000000000000000000000000000000000000000000000000010101010");
		//cout << str << endl;
		//	1 -	0001 0000
		//	2 -	0000 0000
		//	3 -	0000 0000
		//	4 -	0000 0000
		//	5 -	0000 0000
		//	6 -	0000 0000
		//	7 -	0000 0000
		//	8 -	0000 0000
		//	9 -	1010 1010
	}

	// 集合操作
	{
		dynamic_bitset<> db1(5, BOOST_BINARY(10101));
		dynamic_bitset<> db2(5, BOOST_BINARY(10010));

		BOOST_CHECK_EQUAL((db1 | db2), dynamic_bitset<>(5, BOOST_BINARY(10111)));		//| 并集
		BOOST_CHECK_EQUAL((db1 & db2), dynamic_bitset<>(5, BOOST_BINARY(10000)));		//& 交集
		BOOST_CHECK_EQUAL((db1 - db2), dynamic_bitset<>(5, BOOST_BINARY(00101)));		//- 差集

		dynamic_bitset<> db3(5, BOOST_BINARY(101));

		BOOST_CHECK(db3.is_proper_subset_of(db1));										//is_proper_subset_of()真子集,真子集是除去集合自身的子集

		dynamic_bitset<> db4(db2);
		BOOST_CHECK(!db4.is_proper_subset_of(db2));
		BOOST_CHECK(db4.is_subset_of(db2));												//is_subset_of()子集
	}
}

BOOST_AUTO_TEST_SUITE_END()
