
#ifndef _AOPHP_H_
#define _AOPHP_H_

#include <string>
#include <map>
#include <vector>

extern "C" {
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_libaophp.h"
}

namespace aophp
{

/**
 * This is the information that is specified by the PHP programmer when
 * they add a function hook.
 */ 
struct FunctionHook
{
	// The function to invoke if the tag specified by tag_name is present in the doc comments...
	std::string hook_handler;

	// The tag that must be present for a function to be hooked...
	std::string attribute_name;

	// If there are multiple tags of the single type, then we pass them all into the hook handler...
	bool accept_multiple_attributes;

	// If this is true, then when the function hook returns false, the original function is not allowed to be run...
	bool allow_prevent;
};

/**
 *
 */
struct Attribute
{
	std::string name;
	std::string value;
};

/**
 * A collection of all of the function hooks which have already been added.
 */
std::map<std::string, FunctionHook> function_hooks;

/**
 * This hashtable keeps a cache of all function's which have already had their doc_comments parsed for tags.
 */
std::map<std::string, std::vector<Attribute>*> cached_function_attributes;

std::vector<aophp::Attribute>* get_attributes( zend_op_array* op_array );

std::vector<Attribute>* extract_attributes( std::string comments );

void aophp_zend_execute(zend_op_array *op_array TSRMLS_DC);

}; // end namespace aophp;

void (*old_zend_execute)(zend_op_array *op_array TSRMLS_DC);

#endif
