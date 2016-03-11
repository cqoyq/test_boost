/*
 * boost_coroutine.cpp
 *
 *  Created on: 2016-2-22
 *      Author: root
 */

#include <boost/test/unit_test.hpp>

#include <iostream>
using namespace std;

#include "cost_time.h"

#include <boost/coroutine/all.hpp>
//#include <boost/lambda/lambda.hpp>
//using namespace std;
//using namespace boost::lambda;
#include <boost/thread.hpp>

BOOST_AUTO_TEST_SUITE(boost_coroutine)

boost::coroutines::symmetric_coroutine<void>::call_type* term_coro = 0;

void yield_func1(boost::coroutines::symmetric_coroutine<int>::yield_type& yield){
	 for (;;) {
		cout << "thr:" << this_thread::get_id() << ",yield:" << yield.get() << endl; // yield.get() to obtain transfer data.
		yield(); // jump back to starting context
	 }
};

// 测试yeild返回控制给main-context
BOOST_AUTO_TEST_CASE(t_pass_data1){
	boost::coroutines::symmetric_coroutine<int>::call_type coro( // constructor does NOT enter coroutine-function
			yield_func1
						);

	cout << "thr:" << this_thread::get_id() << ",start coro(1)." << endl;
	coro(1); // transfer {1} to coroutine-function
	BOOST_CHECK(coro);

	cout << "thr:" << this_thread::get_id() << ",start coro(2)." << endl;
	coro(2); // transfer {2} to coroutine-function
	BOOST_CHECK(coro);

	cout << "thr:" << this_thread::get_id() << ",start coro(3)." << endl;
	coro(3); // transfer {3} to coroutine-function
	BOOST_CHECK(coro);

	cout << "thr:" << this_thread::get_id() << ",start coro(4)." << endl;
	coro(4); // transfer {4} to coroutine-functio
	BOOST_CHECK(coro);

	cout << "thr:" << this_thread::get_id() << ",start coro(5)." << endl;
	coro(5); // transfer {5} to coroutine-function
	BOOST_CHECK(coro);

	// output
	//	thr:6256e40,start coro(1).
	//	thr:6256e40,yield.get():1

	//	thr:6256e40,start coro(2).
	//	thr:6256e40,yield.get():2

	//	thr:6256e40,start coro(3).
	//	thr:6256e40,yield.get():3

	//	thr:6256e40,start coro(4).
	//	thr:6256e40,yield.get():4

	//	thr:6256e40,start coro(5).
	//	thr:6256e40,yield.get():5

}

void yield_empty2(boost::coroutines::symmetric_coroutine<void>::yield_type& yield){
	for (;;) {
		cout << "call yield_empty!" << endl;
		yield();
	}
};

void yield_func2(boost::coroutines::symmetric_coroutine<int>::yield_type& yield){
	 for (;;) {
		cout << "thr:" << this_thread::get_id() << ",yield:" << yield.get() << endl; // yield.get() to obtain transfer data.
		yield(*term_coro);

		if(yield.get() == 6)
			return;
	 }
};

// 测试yeild传递数据到call_type
BOOST_AUTO_TEST_CASE(t_pass_data2){
	boost::coroutines::symmetric_coroutine<void>::call_type coro_e(yield_empty2);
	term_coro = &coro_e;

	boost::coroutines::symmetric_coroutine<int>::call_type coro( // constructor does NOT enter coroutine-function
			yield_func2
						);
	cout << "thr:" << this_thread::get_id() << ",start coro(1)." << endl;
	coro(1); // transfer {1} to coroutine-function
	BOOST_CHECK(coro_e);
	BOOST_CHECK(coro);

	cout << "thr:" << this_thread::get_id() << ",start coro(2)." << endl;
	coro(2); // transfer {2} to coroutine-function
	BOOST_CHECK(coro_e);
	BOOST_CHECK(coro);

	cout << "thr:" << this_thread::get_id() << ",start coro(3)." << endl;
	coro(3); // transfer {3} to coroutine-function
	BOOST_CHECK(coro_e);
	BOOST_CHECK(coro);

	cout << "thr:" << this_thread::get_id() << ",start coro(4)." << endl;
	coro(4); // transfer {4} to coroutine-functio
	BOOST_CHECK(coro_e);
	BOOST_CHECK(coro);

	cout << "thr:" << this_thread::get_id() << ",start coro(5)." << endl;
	coro(5); // transfer {5} to coroutine-function
	BOOST_CHECK(coro_e);
	BOOST_CHECK(coro);

	cout << "thr:" << this_thread::get_id() << ",start coro(6)." << endl;
	coro(6); // transfer {6} to coroutine-function
	BOOST_CHECK(coro_e);
	BOOST_CHECK(!coro);				// coro's coroutine-function has finished.

	// output:
	//	thr:7f7ef78cf720,start coro(1).
	//	thr:7f7ef78cf720,yield:1
	//	call yield_empty!

	//	thr:7f7ef78cf720,start coro(2).
	//	thr:7f7ef78cf720,yield:2
	//	call yield_empty!

	//	thr:7f7ef78cf720,start coro(3).
	//	thr:7f7ef78cf720,yield:3
	//	call yield_empty!

	//	thr:7f7ef78cf720,start coro(4).
	//	thr:7f7ef78cf720,yield:4
	//	call yield_empty!

	//	thr:7f7ef78cf720,start coro(5).
	//	thr:7f7ef78cf720,yield:5
	//	call yield_empty!

	// thr:7f4556b5b720,start coro(6).
}

void yield_func3(boost::coroutines::symmetric_coroutine<int>::yield_type& yield){
	int  m = 0;
	 for (;;) {
		cout << "thr:" << this_thread::get_id() << ",yield:" << yield.get() << endl; // yield.get() to obtain transfer data.
		m += yield.get() + 10;
		yield();

		cout << "thr:" << this_thread::get_id() << ",m:" << m << endl;
		cout << "thr:" << this_thread::get_id() << ",please input:";
		string str;
		cin >> str;
	 }
};

boost::coroutines::symmetric_coroutine<int>::call_type g_coro( // constructor does NOT enter coroutine-function
		yield_func3
		);

void run(int n){
	if(n == 1){
		this_thread::sleep(seconds(5));
		cout << "thr:" << this_thread::get_id() << " start! n:" << n << endl;
		g_coro(0);
		cout << "thr:" << this_thread::get_id() << " exit! n:" << n << endl;
	}else{
		this_thread::sleep(seconds(10));
		cout << "thr:" << this_thread::get_id() << " start! n:" << n << endl;
		int i = 1;
		while(i < 5){
			cout << "thr:" << this_thread::get_id() << " start to input message" << endl;
			g_coro(i);
			i++;
		}
		cout << "thr:" << this_thread::get_id() << " exit! n:" << n << endl;
	}
}

// 测试coroutine线程操作
BOOST_AUTO_TEST_CASE(t_thread){
	thread t1(run, 1);
	thread t2(run, 2);

	t1.join();
	cout << "thr n: 1 exit!" << endl;
	t2.join();
	cout << "thr n: 2 exit!" << endl;

	// output:
	//	thr:705c700 start! n:1
	//	==12925== Warning: client switching stacks?  SP change: 0x705bd50 --> 0x6272578
	//	==12925==          to suppress, use: --max-stackframe=14587864 or greater
	//	thr:705c700,yield:0
	//	==12925== Warning: client switching stacks?  SP change: 0x6272490 --> 0x705bd50
	//	==12925==          to suppress, use: --max-stackframe=14588096 or greater
	//	thr:705c700 exit! n:1
	//  thr n: 1 exit!
	//
	//	thr:7a5d700 start! n:2
	//	thr:7a5d700 start to input message
	//	==12925== Warning: client switching stacks?  SP change: 0x7a5cd50 --> 0x6272490
	//	==12925==          to suppress, use: --max-stackframe=25077952 or greater
	//	==12925==          further instances of this message will not be shown.
	//	thr:705c700,m:10
	//	thr:705c700,please input:a
	//	thr:705c700,yield:1

	//	thr:7a5d700 start to input message
	//	thr:705c700,m:11
	//	thr:705c700,please input:b
	//	thr:705c700,yield:2

	//	thr:7a5d700 start to input message
	//	thr:705c700,m:12
	//	thr:705c700,please input:c
	//	thr:705c700,yield:3

	//	thr:7a5d700 start to input message
	//	thr:705c700,m:13
	//	thr:705c700,please input:d
	//	thr:705c700,yield:4

	//	thr:7a5d700 exit! n:2
	//  thr n: 2 exit!

}

BOOST_AUTO_TEST_SUITE_END()
