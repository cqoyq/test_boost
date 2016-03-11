/*
 * boost_thread.cpp
 *
 *  Created on: 2016-1-27
 *      Author: root
 */

#include <boost/test/unit_test.hpp>

#include <iostream>
using namespace std;

#include <boost/thread.hpp>
using namespace boost;

BOOST_AUTO_TEST_SUITE(boost_thread)

struct mystudy{
	long m;
	mystudy():m(0){}
	~mystudy(){
		cout << "thr:" << this_thread::get_id() << ",m:" << m << ",this pointer:" << this << ",mystudy destory!" << endl;
		cout << endl;
	}
};

struct mydata{
	int a;
	mystudy my;
	mydata():a(0){}
	~mydata(){
		cout << "thr:" << this_thread::get_id() << ",a:" << a << ",mystudy pointer:" << &my <<",mydata destory!" << endl;
	}
};

boost::thread_specific_ptr<mydata> value;

void increment()
{
	mydata* p = value.get();
    p->a++;
}

void thread_proc()
{
    value.reset(new mydata()); // initialize the thread's storage

    cout << "tss pointer:" << value.get() << endl;

    for (int i=0; i<100; ++i)
    {
        increment();
        mydata* p = value.get();
//        assert(*p == i+1);
        p->my.m +=2;
    }
}

BOOST_AUTO_TEST_CASE(t_tss){

	boost::thread_group threads;
	for (int i=0; i<5; ++i)
		threads.create_thread(&thread_proc);
	threads.join_all();

	// output:
	//	tss pointer:0x574db20
	//	thr:6f10700,a:100,mystudy pointer:0x574db28,mydata destory!
	//	thr:6f10700,m:200,this pointer:0x574db28,mystudy destory!
	//
	//	tss pointer:0x574eaa0
	//	thr:8d13700,a:100,mystudy pointer:0x574eaa8,mydata destory!

	//	tss pointer:0x574e070
	//	thr:7911700,a:100,mystudy pointer:0x574e078,mydata destory!
	//	thr:7911700,m:200,this pointer:0x574e078,mystudy destory!
	//
	//	tss pointer:0x574ec40
	//	thr:8312700,a:100,mystudy pointer:0x574ec48,mydata destory!
	//	thr:8312700,m:200,this pointer:0x574ec48,mystudy destory!
	//
	//	thr:8d13700,m:200,this pointer:0x574eaa8,mystudy destory!
	//
	//	tss pointer:0x574eb70
	//	thr:650f700,a:100,mystudy pointer:0x574eb78,mydata destory!
	//	thr:650f700,m:200,this pointer:0x574eb78,mystudy destory!

}


BOOST_AUTO_TEST_SUITE_END()
