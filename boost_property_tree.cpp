/*
 * boost_property_tree.cpp
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


// Tree container
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
namespace pt = boost::property_tree;
using namespace boost;

BOOST_AUTO_TEST_SUITE(boost_property_tree)



bool is_xml_attr(pt::ptree::value_type&    v){return (v.first.compare("<xmlattr>") == 0)?true:false;}
bool is_xml_comment(pt::ptree::value_type& v){return (v.first.compare("<xmlcomment>") == 0)?true:false;}

void xml_attribute(pt::ptree& pt, string key, string& out){
	foreach_(pt::ptree::value_type& v, pt | filtered(is_xml_attr)){

		foreach_(pt::ptree::value_type& vt, v.second){
			if(vt.first.compare(key.c_str()) == 0){
				out.append(vt.second.data().c_str());
				break;
			}
		}

	}
};

void xml_comment(pt::ptree& pt, string& out){
	foreach_(pt::ptree::value_type& vt, pt | filtered(is_xml_comment)){
		out.append(vt.second.data().c_str());
		break;
	}
};

BOOST_AUTO_TEST_CASE(t_tree_xml){

	// ------------------------------- read xml ----------------------------- //

	// Create empty property tree object
	pt::ptree tree;

	// Parse the XML into the property tree.
	// read_xml flag Flags controlling the bahviour of the parser.
	// * no_concat_text -- Prevents concatenation of text nodes into
    //                     datastring of property tree.  Puts them in
	//					   separate @c \<xmltext\> strings instead.
    // * no_comments    -- Skip XML comments.
	// * trim_whitespace-- Whitespace should be collapsed and trimmed.
	pt::read_xml("conf.xml", tree, pt::xml_parser::no_comments);

	// Use the throwing version of get to find the debug filename.
	// If the path cannot be resolved, an exception is thrown.
	BOOST_CHECK(tree.get<std::string>("deploy_service.name").compare("this is a test") == 0);
	BOOST_CHECK_THROW(tree.get<std::string>("deploy_service.a"), boost::exception);		// node is no exist, throw exception.

	// Use the default-value version of get to find the debug level.
	// Note that the default value is used to deduce the target type.
	BOOST_CHECK_EQUAL(tree.get("deploy_service.phone.has_deploy", 0), 1);	// node is exist, return node's value.
	BOOST_CHECK_EQUAL(tree.get("deploy_service.phone.a", 0), 0);			// node is no exist, return 0.

	// Use get_child to find the node containing the modules, and iterate over
	// its children. If the path cannot be resolved, get_child throws.
	// A C++11 for-range loop would also work.
	foreach_(pt::ptree::value_type &v, tree.get_child("deploy_service.items")) {
		// The data function is used to access the data stored in a node.
		cout << "item:" << v.second.data() << endl;									// output: {one, two, three}

	}
	cout << endl;

	// Read xml attribute and comment
	BOOST_CHECK_EQUAL(tree.get<int>("deploy_service.name.<xmlattr>.old"),19);					// <xmlattr> is attribute
	BOOST_CHECK_EQUAL(tree.get("deploy_service.<xmlcomment>","no node"), "no node");			// <xmlcomment> is comment
	foreach_(pt::ptree::value_type &v, tree.get_child("deploy_service.gateway.ips")) {

		string attr, attr2;
		xml_attribute(v.second, string("value"), attr);
		xml_attribute(v.second, string("id")   , attr2);

		// output:
		// node:ip, attr:110.1.0.0/16, id:
		// node:ip, attr:110.2.0.0/16, id:
		cout << "node:" << v.first << ", attr:" << attr << ", id:" << attr2 << endl;
	}

	// ------------------------------- write xml ----------------------------- //

	pt::ptree wtree;

	// Put the simple values into the tree. The integer is automatically
	// converted to a string. Note that the "debug" node is automatically
	// created if it doesn't exist.
	wtree.put("deploy_service.debug.filename", "conf.xml");
	wtree.put("deploy_service.debug.level", 9);

	// Add all the modules. Unlike put, which overwrites existing nodes, add
	// adds a new node at the lowest level, so the "modules" node will have
	// multiple "module" children.
	std::set<std::string> m_modules;
	m_modules.insert("one");
	m_modules.insert("two");
	m_modules.insert("three");
	m_modules.insert("four");
	foreach_(const std::string &name, m_modules)
		wtree.add("deploy_service.debug.modules.module", name);

	// Write property tree to XML file
	boost::property_tree::xml_writer_settings<std::string> settings('\t', 1);
	pt::write_xml("confw.xml", wtree, std::locale(), settings);

}

BOOST_AUTO_TEST_SUITE_END()

