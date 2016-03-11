/*
 * boost_multiindex.cpp
 *
 *  Created on: 2016-1-13
 *      Author: root
 */

#include <boost/test/unit_test.hpp>

#include <iostream>
using namespace std;

#include "cost_time.h"

#include <boost/foreach.hpp>
#define foreach_         BOOST_FOREACH
#define foreach_r_       BOOST_REVERSE_FOREACH
#include <boost/lexical_cast.hpp>
#include <boost/range/adaptors.hpp>
using namespace boost::adaptors;
#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext.hpp>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/composite_key.hpp>
using namespace boost;
using namespace boost::multi_index;

BOOST_AUTO_TEST_SUITE(boost_multiindex)

#pragma pack(1)			// set byte align

struct sock_data{
	// client
	ushort 	clt_familyid;		// 2 bytes
	uint    clt_ip;				// 4 bytes
	ushort	clt_port;			// 2 bytes
	void*	clt_sockid;			// 8 bytes
	// server
	void* 	srv_sock;			// 8 bytes
	char	srv_ch;				// 1 bytes

	sock_data(ushort familyid, uint ip, ushort port, void* csock, void* ssock, char ch = 0x00):
		clt_familyid(familyid),clt_ip(ip), clt_port(port), clt_sockid(csock), srv_sock(ssock), srv_ch(ch){}
};
#pragma pack()

/* tags for accessing the corresponding indices of sock_data_set */
struct srv_sock_index{};
struct clt_sock_index{};

// define a multiply indexed set with indices
typedef multi_index_container<
		sock_data,
		indexed_by<
		// sort by clt_familyid,clt_ip,clt_port,clt_sockid
		ordered_unique<
			tag<clt_sock_index>,
			composite_key<
				sock_data,
				member<sock_data,ushort, &sock_data::clt_familyid>,
				member<sock_data,uint,   &sock_data::clt_ip>,
				member<sock_data,ushort, &sock_data::clt_port>,
				member<sock_data,void*,  &sock_data::clt_sockid>
			>
		>,

		// sort by srv_sock
		ordered_non_unique<tag<srv_sock_index>, member<sock_data,void*,&sock_data::srv_sock> >
  >
> sock_data_set;

typedef sock_data_set::index<srv_sock_index>::type			 SRV_INDEX_TYPE;
typedef sock_data_set::index<srv_sock_index>::type::iterator SRV_INDEX_ITERATOR;
typedef sock_data_set::index<clt_sock_index>::type  		 CLT_INDEX_TYPE;;
typedef sock_data_set::index<clt_sock_index>::type::iterator CLT_INDEX_ITERATOR;

// Modify element predicate
struct change_ch
{
	change_ch(const char ch):ch_(ch){}

  void operator()(sock_data& e)
  {
    e.srv_ch = ch_;
  }

private:
  char ch_;
};

// Filter predicate
struct is_ip_odd{
	bool operator()(const sock_data& x ) const { return (x.clt_ip % 2 == 1)?true:false; }
};

BOOST_AUTO_TEST_CASE(t_ordered_index){

	// output: 25
	cout << sizeof(sock_data) << endl;

	// Define clt_sockid value
	int csock1 = 1, csock2 = 2, csock3 = 3, csock4 = 4, csock5 = 5, csock6 = 6;

	// Define srv_sock value
	int ssock1 = 10, ssock2 = 11;

	sock_data_set sds;

	sds.insert(sock_data(1, 100, 18000, &csock1, &ssock1));
	sds.insert(sock_data(2, 101, 18000, &csock2, &ssock1));
	sds.insert(sock_data(3, 102, 18000, &csock3, &ssock1));
	sds.insert(sock_data(1, 200, 19000, &csock4, &ssock2));
	sds.insert(sock_data(2, 201, 19000, &csock5, &ssock2));
	sds.insert(sock_data(1, 100, 18000, &csock6, &ssock1));

	// find elements by column-key index 'srv_sock_index'
	SRV_INDEX_TYPE& srv_index = sds.get<srv_sock_index>();
	foreach_(const sock_data& n, srv_index.equal_range(&ssock1)){

		int csk = 0, ssk = 0;
		memcpy(&csk, n.clt_sockid, 4);
		memcpy(&ssk, n.srv_sock, 4);

		// output:
		// familyid:1,ip:100,port:18000,sockid:1,sock:10
		// familyid:2,ip:101,port:18000,sockid:2,sock:10
		// familyid:3,ip:102,port:18000,sockid:3,sock:10
		// familyid:1,ip:100,port:18000,sockid:6,sock:10

		cout << "familyid:" << n.clt_familyid
			<< ",ip:" << n.clt_ip
			<< ",port:" << n.clt_port
			<< ",sockid:" << csk
			<< ",sock:" << ssk
			<< endl;
	}
	cout << endl;

	// find elements by column-key index 'srv_sock_index' with adaptor.
	foreach_(const sock_data& n, srv_index.equal_range(&ssock1) | filtered(is_ip_odd())){

		int csk = 0, ssk = 0;
		memcpy(&csk, n.clt_sockid, 4);
		memcpy(&ssk, n.srv_sock, 4);

		// output:
		// familyid:2,ip:101,port:18000,sockid:2,sock:10

		cout << "familyid:" << n.clt_familyid
			<< ",ip:" << n.clt_ip
			<< ",port:" << n.clt_port
			<< ",sockid:" << csk
			<< ",sock:" << ssk
			<< endl;
	}
	cout << endl;

	// find elements by composite_key index 'clt_sock_index'
	// note:
	// * Lookup operations on a composite key are accomplished by passing tuples with the values searched.
	// * Composite keys are sorted by lexicographical order, i.e. sorting is performed by the first key,
	//   then the second key if the first one is equal, etc. This order allows for partial searches where only the first keys are specified.
	//
	CLT_INDEX_TYPE& clt_index = sds.get<clt_sock_index>();
	foreach_(const sock_data& n, clt_index.equal_range(make_tuple(1, 100, 18000))){			// This is partial search.

		int csk = 0, ssk = 0;
		memcpy(&csk, n.clt_sockid, 4);
		memcpy(&ssk, n.srv_sock, 4);

		// output:
		// familyid:1,ip:100,port:18000,sockid:6,sock:10
		// familyid:1,ip:100,port:18000,sockid:1,sock:10

		cout << "familyid:" << n.clt_familyid
			<< ",ip:" << n.clt_ip
			<< ",port:" << n.clt_port
			<< ",sockid:" << csk
			<< ",sock:" << ssk
			<< endl;
	}
	cout << endl;

	// no find element
	BOOST_CHECK(clt_index.find(make_tuple(1, 100, 28000)) == clt_index.end());

	// modify element value.
	CLT_INDEX_ITERATOR it= clt_index.find(make_tuple(1, 100, 18000, &csock6));
	BOOST_CHECK_EQUAL((*it).srv_ch, 0x00);

	clt_index.modify(it,change_ch(0x0F));
	BOOST_CHECK_EQUAL((*it).srv_ch, 0x0F);

	// erase element

	std::pair< CLT_INDEX_ITERATOR, CLT_INDEX_ITERATOR > iters = clt_index.equal_range(make_tuple(1, 100, 18000));
	clt_index.erase(iters.first, iters.second);

	BOOST_CHECK(clt_index.find(make_tuple(1, 100, 18000)) == clt_index.end());
	foreach_(const sock_data& n, srv_index.equal_range(&ssock1)){


		int csk = 0, ssk = 0;
		memcpy(&csk, n.clt_sockid, 4);
		memcpy(&ssk, n.srv_sock, 4);

		// output:
		// familyid:2,ip:101,port:18000,sockid:2,sock:10
		// familyid:3,ip:102,port:18000,sockid:3,sock:10

		cout << "familyid:" << n.clt_familyid
			<< ",ip:" << n.clt_ip
			<< ",port:" << n.clt_port
			<< ",sockid:" << csk
			<< ",sock:" << ssk
			<< endl;
	}
	cout << endl;
}

#pragma pack(1)			// set byte align

struct device_data{
	uint 	familyid;		// 4 bytes
	char    dev_type;		// 1 bytes
	uint	dev_id;			// 4 bytes
	char	dev_sid;		// 1 bytes
	uint 	dev_data;		// 4 bytes

	device_data(uint familyid, char type, uint id, char sid, uint data = 0):
		familyid(familyid),dev_type(type), dev_id(id), dev_sid(sid), dev_data(data){}
};
#pragma pack()

/* tags for accessing the corresponding indices of device_data_set */
struct device_data_index{};

// define a multiply indexed set with indices
typedef multi_index_container<
		device_data,
		indexed_by<
		// sort by composite_key
		ordered_unique<
			tag<device_data_index>,
			composite_key<
				device_data,
				member<device_data,uint, 	&device_data::familyid>,
				member<device_data,char,   	&device_data::dev_type>,
				member<device_data,uint, 	&device_data::dev_id>,
				member<device_data,char,  	&device_data::dev_sid>
			>
		>
  >
> device_data_set;

typedef device_data_set::index<device_data_index>::type			 	INDEX_TYPE;
typedef device_data_set::index<device_data_index>::type::iterator 	INDEX_ITERATOR;

// Modify element predicate
struct change_data
{
	change_data(const uint d):d_(d){}

  void operator()(device_data& e)
  {
    e.dev_data = d_;
  }

private:
  uint d_;
};

BOOST_AUTO_TEST_CASE(t_ordered_index_performance){

	// output: 14
	cout << sizeof(device_data) << endl;

	device_data_set dds;

	// construct data.
	// one family = 4 dev_type x 5 dev_id x 5 dev_sid = 100 device
	{
		START_COST_TIMER;

		for (uint familyid = 1; familyid <= 10000; ++familyid) {

			char dev_type = 0x01;

			for (int type = 0; type < 4; type++) {

				for (uint id = 1; id < 6; id++) {

					char dev_sid = 0x01;

					for (int sid = 0; sid < 5; sid++) {

						dds.insert(device_data(familyid, dev_type, id, dev_sid));

						dev_sid += 1;
					}
				}

				dev_type += 1;
			}
		}

		END_COST_TIMER("construct");		// output: cost time:6.59 s


	}

	BOOST_CHECK_EQUAL(dds.size(), 100 * 10000u);

	INDEX_TYPE& index = dds.get<device_data_index>();

	// amount of one family's device.
	{
		START_COST_TIMER;
		BOOST_CHECK_EQUAL(index.count(1), 100u);
		END_COST_TIMER("amount of one family's device");		// output: cost time:0.01 s
	}


	// obtain all of one family's device.
	{
		START_COST_TIMER;
		BOOST_CHECK_EQUAL(index.count(10), 100u);
		foreach_(const device_data& n, index.equal_range(10)){

		}
		END_COST_TIMER("obtain all of one family's device");	// output: cost time:0.00 s
	}

	// find one device
	{
		START_COST_TIMER;
		BOOST_CHECK(index.find(make_tuple(9000, 0x04, 5, 0x05)) != index.end());
		END_COST_TIMER("find one device");						// output: cost time:0.01 s

	}

	// erase one family all of device
	{
		START_COST_TIMER;
		std::pair< INDEX_ITERATOR, INDEX_ITERATOR > iters = index.equal_range(5000);
		index.erase(iters.first, iters.second);

		BOOST_CHECK_EQUAL(dds.size(), 100 * 10000u - 100u);
		END_COST_TIMER("erase one family all of device");		// output: cost time:0.01 s

	}

	// insert one element, and then find inserted element
	{
		START_COST_TIMER;
		dds.insert(device_data(5000, 0x10, 23, 0xFF));
		BOOST_CHECK(index.find(make_tuple(5000, 0x10, 23, 0xFF)) != index.end());
		END_COST_TIMER("insert one element, and then find inserted element"); // output: cost time:0.01 s

	}

	// search one elemnet, and then modify element value, search this element again.
	{
		START_COST_TIMER;
		INDEX_ITERATOR it= index.find(make_tuple(5000, 0x10, 23, 0xFF));
		BOOST_CHECK_EQUAL((*it).dev_data, 0u);

		index.modify(it,change_data(111));
		BOOST_CHECK_EQUAL((*it).dev_data, 111u);
		END_COST_TIMER("search one elemnet, and then modify element value, search this element again"); 	// output: cost time:0.01 s

	}

	// traversal all elements
	{
		START_COST_TIMER;
		foreach_(const device_data& n, index){

		}
		END_COST_TIMER("traversal all elements");	// output: cost time:0.09 s
	}
}

BOOST_AUTO_TEST_SUITE_END()


