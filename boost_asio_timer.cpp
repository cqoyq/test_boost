/*
 * boost_asio.cpp
 *
 *  Created on: 2016-1-15
 *      Author: root
 */

#include <boost/test/unit_test.hpp>

#include <iostream>
using namespace std;

#include "cost_time.h"

#include <boost/foreach.hpp>
#define foreach_         BOOST_FOREACH
#define foreach_r_       BOOST_REVERSE_FOREACH

//#define BOOST_ASIO_ENABLE_HANDLER_TRACKING			// Open handler tracking

#include <boost/asio.hpp>
using boost::asio::ip::tcp;
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>



BOOST_AUTO_TEST_SUITE(boost_asio_timer)

void print_use_by_base(const boost::system::error_code& /*e*/)
{
	cout << "Hello, world!" << endl;

	SHOW_CURRENT_TIME("-- asyc timer end");
};

void print_use_by_func(const boost::system::error_code& /*e*/,
    boost::asio::deadline_timer* t, int* count)
{
	if (*count < 5)
	{
	    std::cout << *count << std::endl;
	    SHOW_CURRENT_TIME("-- asyc timer bind");
	    ++(*count);
//	    t->expires_at(t->expires_at() + boost::posix_time::seconds(1));
	    t->expires_from_now(boost::posix_time::seconds(1));
	    t->async_wait(boost::bind(print_use_by_func,
	              boost::asio::placeholders::error, t, count));
	}
};

class printer_use_by_member
{
public:
	printer_use_by_member(boost::asio::io_service& io)
	    : timer_(io, boost::posix_time::seconds(1)),
	      count_(0){
		timer_.async_wait(boost::bind(&printer_use_by_member::print, this, boost::asio::placeholders::error)); 		// bind class member
	}
	 ~printer_use_by_member(){
		 cout << "Final count is " << count_ << endl;
	}
	void print(const boost::system::error_code& err){
		if(!err){
			if (count_ < 5){
				cout << count_ << endl;
				++count_;

				timer_.expires_at(timer_.expires_at() + boost::posix_time::seconds(1));
				timer_.async_wait(boost::bind(&printer_use_by_member::print, this, boost::asio::placeholders::error));	// bind class member
			}
		}
	}
private:
	boost::asio::deadline_timer timer_;
	int count_;
};

BOOST_AUTO_TEST_CASE(t_asyc_timer_base){

	SHOW_CURRENT_TIME("-- asyc timer begin");

	boost::asio::io_service io;

	boost::asio::deadline_timer t(io, boost::posix_time::seconds(5));
	t.async_wait(&print_use_by_base);

	io.run();

	// output:
	// 0
	// 1
	// 2
	// 3
	// 4
	// Final count is 5
}

BOOST_AUTO_TEST_CASE(t_asyc_timer_bind_func){

	boost::asio::io_service io;

	int count = 0;
	boost::asio::deadline_timer t(io, boost::posix_time::seconds(1));

	t.async_wait(boost::bind(print_use_by_func,							// bind function argument
	        boost::asio::placeholders::error, &t, &count));

	io.run();

	cout << "Final count is " << count << endl;
	SHOW_CURRENT_TIME("-- asyc timer bind");

	// output:
	// 0
	// 1
	// 2
	// 3
	// 4
	// Final count is 5
}

BOOST_AUTO_TEST_CASE(t_asyc_timer_bind_member){

	boost::asio::io_service io;

	printer_use_by_member p(io);

	io.run();

	// output:
	// 0
	// 1
	// 2
	// 3
	// 4
	// Final count is 5

}

class printer_use_by_strand
{
public:
	printer_use_by_strand(boost::asio::io_service& io)
	    : strand_(io),	// An boost::asio::strand guarantees that, for those handlers that are dispatched through it,
						// an executing handler will be allowed to complete before the next one is started.
	      timer1_(io, boost::posix_time::seconds(1)),
	      timer2_(io, boost::posix_time::seconds(1)),
	      count_(0){

		// each callback handler is "wrapped" using the boost::asio::strand object.
		// The strand::wrap() function returns a new handler that automatically dispatches its contained handler
		// through the boost::asio::strand object. By wrapping the handlers using the same boost::asio::strand,
		// we are ensuring that they cannot execute concurrently.
		timer1_.async_wait(strand_.wrap(boost::bind(&printer_use_by_strand::print1, this)));
		timer2_.async_wait(strand_.wrap(boost::bind(&printer_use_by_strand::print2, this)));
	}
	 ~printer_use_by_strand(){
		 cout << "Final count is " << count_ << endl;
	}

	// In a multithreaded program, the handlers for asynchronous operations should be synchronised if they access shared resources.
	// In this tutorial, the shared resources used by the handlers (print1 and print2) are std::cout and the count_ data member.
	void print1(){
		if (count_ < 10){
			std::cout << "Timer 1: " << count_ << std::endl;
			++count_;

			timer1_.expires_at(timer1_.expires_at() + boost::posix_time::seconds(1));
			timer1_.async_wait(strand_.wrap(boost::bind(&printer_use_by_strand::print1, this)));			// wrap handler
		}
	}

	void print2(){
		if (count_ < 10){
			std::cout << "Timer 2: " << count_ << std::endl;
			++count_;

			timer2_.expires_at(timer2_.expires_at() + boost::posix_time::seconds(1));
			timer2_.async_wait(strand_.wrap(boost::bind(&printer_use_by_strand::print2, this)));			// wrap handler
		}
	}

private:
	boost::asio::io_service::strand strand_;
	boost::asio::deadline_timer timer1_;
	boost::asio::deadline_timer timer2_;
	int count_;
};

BOOST_AUTO_TEST_CASE(t_asyc_timer_strand){

	// The main function now causes io_service::run() to be called from two threads: the main thread and one additional thread.
	// This is accomplished using an boost::thread object.

	// Just as it would with a call from a single thread, concurrent calls to io_service::run() will continue to execute while there is "work" left to do.
	// The background thread will not exit until all asynchronous operations have completed.

	boost::asio::io_service io;

	printer_use_by_strand p(io);

	boost::thread t(boost::bind(&boost::asio::io_service::run, &io));

	io.run();
	t.join();

	// output:
	// Timer 1: 0
	// Timer 2: 1
	// Timer 1: 2
	// Timer 2: 3
	// Timer 1: 4
	// Timer 2: 5
	// Timer 1: 6
	// Timer 2: 7
	// Timer 1: 8
	// Timer 2: 9
	// Final count is 10

}

BOOST_AUTO_TEST_SUITE_END()



