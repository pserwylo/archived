/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2007 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id: header,v 1.16.2.1.2.1 2007/01/01 19:32:09 iliaa Exp $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_libaophp.h"

/* If you declare any globals in php_libaophp.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(libaophp)
*/

/* True global resources - no need for thread safety here */
static int le_libaophp;

/* {{{ libaophp_functions[]
 *
 * Every user visible function must have an entry in libaophp_functions[].
 */
zend_function_entry libaophp_functions[] = {
	PHP_FE(confirm_libaophp_compiled, NULL)	/* For testing, remove later. */
	PHP_FE(aophp_add_hook, NULL)
	PHP_FE(aophp_remove_hook, NULL)
	{NULL, NULL, NULL}	/* Must be the last line in libaophp_functions[] */
};
/* }}} */

/* {{{ libaophp_module_entry
 */
zend_module_entry libaophp_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"libaophp",
	libaophp_functions,
	PHP_MINIT(libaophp),
	PHP_MSHUTDOWN(libaophp),
	PHP_RINIT(libaophp),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(libaophp),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(libaophp),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_LIBAOPHP
ZEND_GET_MODULE(libaophp)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("libaophp.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_libaophp_globals, libaophp_globals)
    STD_PHP_INI_ENTRY("libaophp.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_libaophp_globals, libaophp_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_libaophp_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_libaophp_init_globals(zend_libaophp_globals *libaophp_globals)
{
	libaophp_globals->global_value = 0;
	libaophp_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
/* Implemented in aophp.cpp
PHP_MINIT_FUNCTION(libaophp)
{
	// If you have INI entries, uncomment these lines 
	// REGISTER_INI_ENTRIES();
	return SUCCESS;
}*/
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(libaophp)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(libaophp)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(libaophp)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(libaophp)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "libaophp support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_libaophp_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_libaophp_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "libaophp", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/

void* getAttributesFromComments( char* comments )
{
	// Strip all the fluffy "/**", "*" and "*/" characters from the comments.
	// Look for any line beginning with '@' (or better yet, use the php.ini file to define what begins an attribute.
	// Go from there...
}

PHP_FUNCTION(getAttribute)
{

	char* functionName;
	int functionNameLength;

	if ( zend_parse_parameters( ZEND_NUM_ARGS()	TSRMLS_CC, "s", &functionName, &functionNameLength ) == FAILURE )
	{
		return;
	}

	char* functionNameLower = zend_str_tolower_dup(functionName, functionNameLength);

	HashTable* functionTable = EG( function_table );

	zend_function* function;
	if ( zend_hash_find(functionTable, functionNameLower, functionNameLength + 1, (void**)&function) == FAILURE )
	{
		zend_error(E_WARNING, "Function '%s' does not exist", functionName);
		RETURN_NULL();
	}
	else
	{
		RETURN_STRING( function->op_array.doc_comment, 1 );
	}	
}

/**
 * Add a function hook for the specified function.
 * @param String Function name to be intercepted
 * @param Whether or not the attribute hook is allowed to have multiples of a given tag.
 * @param Whether or not to allow the hook to prevent execution of the function.
 */
PHP_FUNCTION(add_function_hook)
{
	
}

PHP_FUNCTION(remove_function_hook)
{
	
}



/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
