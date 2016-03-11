/*
 * boost_pointer_container.cpp
 *
 *  Created on: 2016-1-13
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

// Pointer-container
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/ptr_deque.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <boost/ptr_container/ptr_map.hpp>

using namespace boost;

BOOST_AUTO_TEST_SUITE(boost_pointer_container)

class animal : boost::noncopyable
{
public:
	virtual ~animal(){};
	virtual void eat(){};
	int k;
	string str;
    // ...
};

class mammal : public animal
{
public:
	~mammal(){
		cout << this << " destory!" << endl;
	}
	mammal(const char* s){k = 1; str.append(s);}
	void eat(){cout << "eat: mammal" << endl;}
    // ...
};

class bird : public animal
{
public:
	~bird(){
		cout << this << " destory!" << endl;
	}
	bird(const char* s){k = 2;str.append(s);}
	void eat(){cout << "eat: bird" << endl;}
    // ...
};

bool is_equal_one(animal& an){ return (an.k == 1)?true:false; };
bool is_equal_two(animal& an){ return (an.k == 2)?true:false; };
bool is_equal_three(animal& an){ return (an.k == 3)?true:false; };

bool is_equal_thrity(animal& an){ return (an.str.compare("thirty") == 0)?true:false; };

bool is_equal_one_map(boost::ptr_map<string, animal>::value_type v){
	return (v.second->k == 1)?true:false;
}

BOOST_AUTO_TEST_CASE(t_ptr_vector){

	boost::ptr_vector<animal> v, v2;

	// Heap allocate
	v.push_back( new mammal("first") ); // Heap-allocate all elements of the container never rely on copy-semantics.
	v.push_back( new bird("second") );
	v.push_back( new mammal("thirty") );
	v.push_back( new bird("four") );

	// output: eat: mammal
	v[0].eat();

	typedef boost::ptr_vector<animal> ptr_vec;
	ptr_vec::iterator i = v.begin();
	// output: eat: mammal
	i->eat();

	// traversal
	foreach_(animal& n, v | filtered(is_equal_one)){
		// output:
		// k:1,str:first,eat: mammal
		// k:1,str:thirty,eat: mammal
		cout << "k:" << n.k << "," << &n << ",";
		cout << "str:" << n.str << ",";
		n.eat();
	}
	cout << endl;

	foreach_(animal& n, v | filtered(is_equal_one) | filtered(is_equal_thrity)){
		// output:
		// k1:1,str1:thirty,eat: mammal
		cout << "k1:" << n.k << "," << &n << ",";
		cout << "str1:" << n.str << ",";
		n.eat();
	}
	cout << endl;

	// transformed
	foreach_(animal& n, v | filtered(is_equal_two)){
		n.k = 9;
	}
	foreach_(animal& n, v){
		// output:
		// k3:1,str3:first,eat: mammal
		// k3:9,str3:second,eat: bird
		// k3:1,str3:thirty,eat: mammal
		// k3:9,str3:four,eat: bird
		cout << "k3:" << n.k << "," << &n << ",";
		cout << "str3:" << n.str << ",";
		n.eat();

	}


	BOOST_CHECK_EQUAL(v.size(), (uint)4);

	// delete all elements with satisfy prd.
	v.erase_if(is_equal_one);
	v.erase_if(is_equal_three);

	BOOST_CHECK_EQUAL(v.size(), (uint)2);
	BOOST_CHECK_EQUAL(v[0].k, 9);
	BOOST_CHECK_EQUAL(v[0].str, "second");
	BOOST_CHECK_EQUAL(v[1].str, "four");

	ptr_vec::iterator iter = v.begin();
	v.erase(iter);

	BOOST_CHECK_EQUAL(v.size(), (uint)1);
	BOOST_CHECK_EQUAL(v[0].k, 9);
	BOOST_CHECK_EQUAL(v[0].str, "four");

	// delete all elements.
	v.clear();
	BOOST_CHECK_EQUAL(v.size(), (uint)0);

}

BOOST_AUTO_TEST_CASE(t_ptr_map){

	typedef boost::ptr_map<string, animal> ptr_mp;
	typedef boost::ptr_map<string, animal>::value_type ptr_mp_value;

	ptr_mp mp;

	string str1("a"), str2("b");

	mp.insert(str1,new mammal("first"));
	mp.insert(str2,new bird("second"));
	mp.insert(str2,new bird("second22")); // insert duplicate key, the container is ignore.

	BOOST_CHECK_EQUAL(mp.size(), (uint)2);	// mp is {a:first, b:second}

	// []operator returns the object with key key if it exists;
	// otherwise a new object is allocated and inserted and its reference returned.
	BOOST_CHECK_EQUAL(mp["a"].str.compare("first"), 0);
	BOOST_CHECK_EQUAL(mp["c"].str.compare(""), 0);   // make new object

	// traversal
	foreach_(ptr_mp_value n, mp){

		// output:
		// key:a,value:first
		// key:b,value:second
		// key:c,value:
		cout << "key:" << n.first << ",value:" << n.second->str << endl;
	}
	cout << endl;

	foreach_(ptr_mp_value n, mp | filtered(is_equal_one_map)){

		// output:
		// key:a,value:first
		cout << "key:" << n.first << ",value:" << n.second->str << endl;
	}

	// find
	BOOST_CHECK((*(mp.find(string("a")))).first.compare("a") == 0);
	BOOST_CHECK(mp.find(string("d")) == mp.end());

	// erase
	mp.erase(string("a"));   // delete key is 'a'.
	mp.erase(string("d"));	 // delete key is no exist.

	BOOST_CHECK_EQUAL(mp.size(), (uint)2);
	BOOST_CHECK_EQUAL(mp["b"].str.compare("second"), 0);

	mp.clear();      // erase all elements.
	BOOST_CHECK_EQUAL(mp.size(), (uint)0);

	// another ptr_map type
	typedef boost::ptr_map<int, animal> ptr_mp2;

	ptr_mp2 mp2;
	int one = 1, two = 2;
	mp2.insert(one,new mammal("first"));
	mp2.insert(two,new bird("second"));
}

BOOST_AUTO_TEST_CASE(t_ptr_multimap){

	cout << "----------- ptr_multimap ------------" << endl;
	typedef boost::ptr_multimap<string, animal> ptr_mmp;
	typedef boost::ptr_multimap<string, animal>::value_type ptr_mmp_value;

	ptr_mmp mp;

	string str1("a"), str2("b");

	mp.insert(str1,new mammal("first"));
	mp.insert(str2,new bird("second"));
	mp.insert(str2,new bird("second22"));

	BOOST_CHECK_EQUAL(mp.size(), (uint)3);

	// find
	foreach_(ptr_mmp_value n, mp.equal_range(str2)){

		// output:
		// key:b,value:second
		// key:b,value:second22

		cout << "key:" << n.first << ",value:" << n.second->str << endl;
	}

	// erase
	mp.erase(mp.equal_range(str2));

	BOOST_CHECK_EQUAL(mp.size(), (uint)1);

	mp.clear();
}

BOOST_AUTO_TEST_CASE(t_ptr_queue){

	typedef boost::ptr_deque<animal> ptr_deq;

	ptr_deq dep;

	BOOST_CHECK_EQUAL(dep.empty(), true);

	dep.push_back(new mammal("first"));
	dep.push_back(new bird("second"));
	dep.push_back(new bird("second22"));

	BOOST_CHECK_EQUAL(dep.empty(), false);
	BOOST_CHECK_EQUAL(dep.size(), 3u);

	foreach_(animal& n, dep){

		// output:
		// 0x551dec0,n:first
		// 0x551df80,n:second
		// 0x551e040,n:second22
		cout << &n << "," << "n:" << n.str << endl;
	}
	cout << endl;

	// top first element
	animal& front = dep.front();
	cout << &front << ",front:" << front.str << endl << endl; // output: 0x551dec0,front:first

	// pop and erase
	dep.pop_front();		// 0x551dec0 destory!
	cout << endl;

	foreach_(animal& n, dep){

		// output:
		// 0x551df80,n:second
		// 0x551e040,n:second22
		cout << &n << "," << "n:" << n.str << endl;
	}
	cout << endl;

	// erase all elements
	dep.clear();
	// output:
	// 0x551df80 destory!
	// 0x551e040 destory!

	cout << "destroy all" << endl;
}

BOOST_AUTO_TEST_SUITE_END()

