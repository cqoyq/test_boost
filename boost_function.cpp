/*
 * boost_function.cpp
 *
 *  Created on: 2016-3-5
 *      Author: root
 */

#include <boost/test/unit_test.hpp>

#include <boost/bind.hpp>
#include <boost/function.hpp>
using namespace boost;

BOOST_AUTO_TEST_SUITE(boost_function)

int f(int a, int b)							//二元函数
{
	return a + b;
};

struct demo_class
{
	int add(int a, int b)					//成员函数
	{
		return a + b;
	}

	int operator()(int x) const				//重载operator(),实现函数对象
	{
		return x * x;
	}
};

class demo_callback_class
{
private:
	typedef function<int(int)> func_t;		//function类型定义
	func_t func;							//function对象
	int n;									//内部成员变量
public:
	demo_callback_class(int i):n(i){}

	template<typename CallBack>
	void accept(CallBack f)					//模板函数accept()接受回调函数
	{
		func = f;
	};

	int run()								//run()执行回调函数
	{
		return func(n);
	};
};

int call_back_func(int i)
{
	return i * 2;
};

class call_back_obj
{
private:
	int x;
public:
	call_back_obj(int i) : x(i){}

	int operator()(int i)
	{
		return i * x++;				//x先乘法,再递增
	};
};

//回调函数工厂
class call_back_factory
{
public:
	int call_back_func1(int i)				//1个参数
	{
		return i * 2;
	};

	int call_back_func2(int i, int j)		//2个参数
	{
		return i * j * 2;
	};

};

BOOST_AUTO_TEST_CASE(t_base)
{
	// Base operator
	{
		function<int(int, int)> func;						//无参构造一个function对象,函数返回类型为int,函数参数是两个int类型
		BOOST_CHECK(!func);									//隐式转换为bool,此时function不持有任何对象

		func = f;											//func存储函数f
		if(func)
		{
			BOOST_CHECK_EQUAL(func(10, 20), 30);			//调用function的operator()
		}

		func = 0;											//function清空,相当于clear()
		BOOST_CHECK(func.empty());							//empty()判断function是否为空

		demo_class c;
		func = bind(&demo_class::add, c, _1, _2);			//func存储成员函数
		BOOST_CHECK_EQUAL(func(20, 20), 40);

		function<int(int)> func1;							//构造一个新的function对象
		func1 = bind<int>(demo_class(), _1);				//func存储函数对象
		BOOST_CHECK_EQUAL(func1(10), 100);
	}

	// 使用ref库
	{
		demo_class c;

		function<int(int)> func1;
		func1 = cref(c);									//使用cref()函数包装常对象的引用
		BOOST_CHECK_EQUAL(func1(10), 100);
	}

	// 回调运用,无需改变回调的接口就能实现解耦客户代码,使客户代码不必绑死在一种回调形式上,进而可以持续演化.
	{
		demo_callback_class c(10);
		c.accept(call_back_func);							//接受回调函数
		BOOST_CHECK_EQUAL(c.run(), 20);						//调用回调函数

		call_back_obj cbo(2);
		c.accept(ref(cbo));									//接受函数对象引用
		BOOST_CHECK_EQUAL(c.run(), 20);
		BOOST_CHECK_EQUAL(c.run(), 30);

		// 回调工厂实现
		call_back_factory cbf;
		c.accept(bind(&call_back_factory::call_back_func1, cbf, _1));	//接受bind表达式
		BOOST_CHECK_EQUAL(c.run(), 20);

		c.accept(bind(&call_back_factory::call_back_func2, cbf, _1, 3));
		BOOST_CHECK_EQUAL(c.run(), 60);

	}
}

BOOST_AUTO_TEST_SUITE_END()
