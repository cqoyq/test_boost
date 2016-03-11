/*
 * boost_signals2.cpp
 *
 *  Created on: 2016-2-6
 *      Author: root
 */

#include <boost/test/unit_test.hpp>

#include <iostream>
using namespace std;

#include <boost/signals2.hpp>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
using namespace boost;
using namespace boost::posix_time;

BOOST_AUTO_TEST_SUITE(boost_signals2)

struct Hello
{
  void operator()() const
  {
    std::cout << "Hello";
  }
};

struct World
{
  void operator()() const
  {
    std::cout << ", World!" << std::endl;
  }
};

struct GoodMorning
{
  void operator()() const
  {
    std::cout << "... and good morning!" << std::endl;
  }
};

struct MyTest{
	void operator()() const
	{
		std::cout << "start test!" << std::endl;
	}
};

BOOST_AUTO_TEST_CASE(t_slots_order){

	boost::signals2::signal<void ()> sig;

	sig.connect(1, World());  // connect with group 1
	sig.connect(0, Hello());  // connect with group 0

	// by default slots are connected at the end of the slot list
	sig.connect(GoodMorning());

	sig.connect(MyTest(), boost::signals2::at_front);

	// slots are invoked this order:
	// 1) ungrouped slots connected with boost::signals2::at_front
	// 2) grouped slots according to ordering of their groups
	// 3) ungrouped slots connected with boost::signals2::at_back
	sig();

	// output:
	//	start test!
	//	Hello, World!
	//	... and good morning!

}
struct valueA{
	int m;
};
int pass_value_1(int a, void* p, valueA& v){
	cout << "pass_value_1, a:" << a << ",p:" << p << ", valueA->m:" << v.m << endl;
	v.m += 1;
	return v.m;
}
int pass_value_2(int a, void* p, valueA& v){
	cout << "pass_value_2, a:" << a << ",p:" << p << ", valueA->m:" << v.m << endl;
	v.m += 2;
	return v.m;
}
int pass_value_3(int a, void* p, valueA& v){
	cout << "pass_value_3, a:" << a << ",p:" << p << ", valueA->m:" << v.m << endl;
	v.m += 3;
	return v.m;
}

BOOST_AUTO_TEST_CASE(t_pass_value){

	valueA A;
	A.m = 1;
	void* p = &A;

	boost::signals2::signal<int (int, void*, valueA&)> sig;
	sig.connect(&pass_value_1);
	sig.connect(&pass_value_2);
	sig.connect(&pass_value_3);

	// The default combiner returns a boost::optional containing the return
	// value of the last slot in the slot list, in this case the
	// pass_value_3 function.
	cout << "combiner return:" << *sig(100, p, A) << endl;

	// output:
	//	pass_value_1, a:100,p:0x7feffe290, valueA->m:1
	//	pass_value_2, a:100,p:0x7feffe290, valueA->m:2
	//	pass_value_3, a:100,p:0x7feffe290, valueA->m:4
	//	combiner return: 7

}

struct DistributeRequest {
  typedef int result_type;

  template<typename InputIterator>
  result_type operator()(InputIterator first, InputIterator last) const
  {

	  cout << "operator(),first:" << *first << endl;

	  // If there are no slots to call, just return the
	  // default-constructed value
	  if(first == last ) return -1;

	  while (first != last) {
		  cout << "begin" << endl;
		  cout << "first:" << *first  << endl;
		  cout << "end" << endl;

		  if (*first == 4)
			return 4;

		  ++first;

	  }
	  return 0;
  }
};

BOOST_AUTO_TEST_CASE(t_combiner){

	valueA A;
	A.m = 1;
	void* p = &A;

	boost::signals2::signal<int (int, void*, valueA&), DistributeRequest> sig;
	sig.connect(&pass_value_1);
	sig.connect(&pass_value_2);
	sig.connect(&pass_value_3);

	cout << "combiner return:" << sig(100, p, A) << endl;

	// output
	//	pass_value_1, a:100,p:0x7feffe2b0, valueA->m:1			// call first solt
	//	operator(),first:2
	//	begin
	//	first:2
	//	end
	//	begin													// loop call succedent solts
	//	pass_value_2, a:100,p:0x7feffe2b0, valueA->m:2
	//	first:4
	//	end
	//	combiner return:4

}

BOOST_AUTO_TEST_CASE(t_connection){

	cout << "----------- Disconnect Slots -------------" << endl;
	{
		boost::signals2::signal<void ()> sig;

		boost::signals2::connection c = sig.connect(GoodMorning());
		std::cout << "c is connected\n";
		sig(); // Prints "Hello, World!"

		c.disconnect(); // Disconnect the HelloWorld object
		std::cout << "c is disconnected\n";
		sig(); // Does nothing: there are no connected slots
	}

	cout << "----------- Blocking Slots-------------" << endl;
	{
		boost::signals2::signal<void ()> sig;

		boost::signals2::connection c = sig.connect(GoodMorning());
		std::cout << "c is not blocked.\n";
		sig(); // Prints "Hello, World!"

		{
			boost::signals2::shared_connection_block block(c); // block the slot
			std::cout << "c is blocked.\n";
			sig(); // No output: the slot is blocked
		} // shared_connection_block going out of scope unblocks the slot

		std::cout << "c is not blocked.\n";
		sig(); // Prints "Hello, World!"}
	}

	cout << "----------- Scoped Connections-------------" << endl;
	{
		boost::signals2::signal<void ()> sig;

		{
			boost::signals2::scoped_connection c(sig.connect(GoodMorning()));
			sig(); // will call Hello function object
		} // scoped_connection goes out of scope and disconnects

		sig(); // Hello function object no longer connected to sig
	}

	cout << "----------- Disconnecting Equivalent Slots-------------" << endl;
	{
		valueA A;
		A.m = 1;
		void* p = &A;

		boost::signals2::signal<int (int, void*, valueA&)> sig;

		sig.connect(&pass_value_1);
		sig.connect(&pass_value_2);
		sig(100, p, A);
		cout << "after disconnects pass_value_1" << endl;
		// disconnects Hello, but not GoodMorning
		sig.disconnect(&pass_value_1);
		sig(100, p, A);

		// output
		//	pass_value_1, a:100,p:0x7feffe2a0, valueA->m:1
		//	pass_value_2, a:100,p:0x7feffe2a0, valueA->m:2
		//	after disconnects pass_value_1
		//	pass_value_2, a:100,p:0x7feffe2a0, valueA->m:4

	}

}

boost::signals2::signal<void (int)> thr_sig;

void thr_notify(int taskid){
	cout << "notify(),thr:" << this_thread::get_id() << ",taskid:" << taskid << endl;
}

void thr_run(int taskid){

	this_thread::sleep(seconds(5));

	cout << "run(),thr:" << this_thread::get_id() << ",taskid:" << taskid << endl;
	thr_sig(taskid);

	this_thread::sleep(seconds(10));
}

BOOST_AUTO_TEST_CASE(t_thread_notify){

	thr_sig.connect(&thr_notify);
	cout << "main(),thr:" << this_thread::get_id() << endl;
	boost::thread t(thr_run, 110);
	t.join();

	thr_run(120);

	// output:
	//	main(),thr:570a300
	//
	//	run(),thr:650f700,taskid:110			// call sig in thread
	//	notify(),thr:650f700,taskid:110
	//
	//	run(),thr:570a300,taskid:120
	//	notify(),thr:570a300,taskid:120

}

BOOST_AUTO_TEST_SUITE_END()
