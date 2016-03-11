/*
 * boost_asio_signal_set.cpp
 *
 *  Created on: 2016-2-5
 *      Author: root
 */


#include <boost/test/unit_test.hpp>

#include <csignal>
#include <iostream>
using namespace std;

#include "cost_time.h"

#include <boost/foreach.hpp>
#define foreach_         BOOST_FOREACH
#define foreach_r_       BOOST_REVERSE_FOREACH

//#define BOOST_ASIO_ENABLE_HANDLER_TRACKING			// Open handler tracking
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>

BOOST_AUTO_TEST_SUITE(boost_asio_signal_set)

class my_signal{
public:
	my_signal(boost::asio::io_service& ios) : signals_(ios), str_("this is a test"){
		signals_.add(SIGUSR1);
		signals_.add(SIGUSR2);
		signals_.async_wait(boost::bind(&my_signal::signal_handler, this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::signal_number,
						str_.c_str()));
	}

	void signal_handler(
	    const boost::system::error_code& ec,
	    int signal_number,
	    const char* p)
	{


		cout << "SIGRTMAX:" << SIGRTMAX << ",SIGRTMIN:" << SIGRTMIN << ",signal_number:" << signal_number <<
				",str:" << p << ",thr:" << this_thread::get_id() << endl;

		if(signal_number == SIGUSR2)
			return;

		str_ = "SIGUSR2";
		signals_.async_wait(boost::bind(&my_signal::signal_handler, this,
							boost::asio::placeholders::error,
							boost::asio::placeholders::signal_number,
							str_.c_str()));
	}

public:
	string str_;
	boost::asio::signal_set signals_;
};



void run(){

	this_thread::sleep(seconds(5));
	raise(SIGUSR1);

	this_thread::sleep(seconds(5));
	raise(SIGUSR2);
}

void run2(){

	boost::asio::io_service ios;

	my_signal sig(ios);

	ios.run();
}

BOOST_AUTO_TEST_CASE(t){

	boost::asio::io_service ios;

	my_signal sig(ios);

	thread t1(run);
	thread t2(run2);

	ios.run();

	t1.join();
	t2.join();

	// output:
	//	@asio|1454773336.451790|0*1|signal_set@0x7feffe200.async_wait
	//	@asio|1454773336.570213|0*2|signal_set@0x6f0fe20.async_wait
	//
	//
	//	@asio|1454773341.573826|>1|ec=system:0,signal_number=10
	//	SIGRTMAX:64,signal_number:10,str:this is a test,thr:570a300
	//	@asio|1454773341.582433|1*3|signal_set@0x7feffe200.async_wait
	//	@asio|1454773341.584206|>2|ec=system:0,signal_number=10
	//	SIGRTMAX:64,signal_number:10,str:this is a test,thr:6f10700
	//	@asio|1454773341.584377|2*4|signal_set@0x6f0fe20.async_wait
	//	@asio|1454773341.584443|<2|
	//	@asio|1454773341.583440|<1|
	//
	//
	//	@asio|1454773346.569639|>3|ec=system:0,signal_number=12
	//	SIGRTMAX:64,signal_number:12,str:SIGUSR2,thr:570a300
	//	@asio|1454773346.569862|<3|
	//	@asio|1454773346.588602|>4|ec=system:0,signal_number=12
	//	SIGRTMAX:64,signal_number:12,str:SIGUSR2,thr:6f10700
	//	@asio|1454773346.588777|<4|
	//	@asio|1454773346.599152|0|signal_set@0x6f0fe20.cancel
	//	@asio|1454773346.622721|0|signal_set@0x7feffe200.cancel


}

BOOST_AUTO_TEST_SUITE_END()

