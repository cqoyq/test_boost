/*
 * boost_atomic.cpp
 *
 *  Created on: 2016-1-24
 *      Author: root
 */

#include <boost/test/unit_test.hpp>

#include <iostream>
using namespace std;

#include <boost/atomic.hpp>

BOOST_AUTO_TEST_SUITE(boost_atomic)

BOOST_AUTO_TEST_CASE(t_lock_free){

	BOOST_CHECK_EQUAL(BOOST_ATOMIC_BOOL_LOCK_FREE, 2);
}


BOOST_AUTO_TEST_SUITE_END()

