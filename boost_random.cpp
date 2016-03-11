/*
 * boost_random.cpp
 *
 *  Created on: 2016-3-8
 *      Author: root
 */

#include <boost/test/unit_test.hpp>

#include <iostream>
using namespace std;

#include <boost/random/random_device.hpp>
#include <boost/random/uniform_int_distribution.hpp>

BOOST_AUTO_TEST_SUITE(boost_random)

// This example demonstrates generating a random 8 character password.
BOOST_AUTO_TEST_CASE(t_make_password){

	string chars(
	        "abcdefghijklmnopqrstuvwxyz"
	        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	        "1234567890"
	        "!@#$%^&*()"
	        "`~-_=+[{]}\\|;:'\",<.>/? ");
	boost::random::random_device rng;
	boost::random::uniform_int_distribution<> index_dist(0, chars.size() - 1);

	int n = 0;
	while(n < 5){
		for(int i = 0; i < 8; ++i) {
			std::cout << chars[index_dist(rng)];
		}
		std::cout << std::endl;

		n++;
	}

	// output:
	//	1G1#{cg?
	//	J`<=6q5:
	//	[4Yi8\^2
	//	1a_+2Y@9
	//	~}$zL+Ph
}

BOOST_AUTO_TEST_SUITE_END()
