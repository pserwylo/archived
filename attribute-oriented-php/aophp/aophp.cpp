
#include <aophp.h>

PHP_MINIT_FUNCTION(libaophp)
{
	// Adapted from xdebug, cheers to Derick Rethans.
	// TODO: Like in xdebug, override the zend_compile function so that doc comments can be sorted then...
	old_zend_execute = zend_execute;
	zend_execute = aophp::aophp_zend_execute;
}

PHP_FUNCTION(aophp_add_hook)
{

	char* attribute_name;
	int attribute_name_length;

	char* hook_handler;
	int hook_handler_length;

	int accept_multiple_attributes;

	int allow_prevent;

	if ( zend_parse_parameters( ZEND_NUM_ARGS()	TSRMLS_CC, "ssll", 
			&attribute_name, &attribute_name_length,
			&hook_handler, &hook_handler_length,
			&accept_multiple_attributes,
			&allow_prevent ) == FAILURE )
	{
		return;
	}

	// Validate that the function passed in as the handler is infact callable...

	std::map<std::string, aophp::FunctionHook>::iterator found = aophp::function_hooks.find( attribute_name );
	if (found == aophp::function_hooks.end() )
	{
		aophp::FunctionHook hook;

		hook.hook_handler = hook_handler;
		hook.attribute_name = attribute_name;
		hook.accept_multiple_attributes = accept_multiple_attributes;
		hook.allow_prevent;

		aophp::function_hooks.insert( std::make_pair( attribute_name, hook ) );
	}

	return;

}

PHP_FUNCTION(aophp_remove_hook)
{
	char* attribute_name;
	int attribute_name_length;
	
	if ( zend_parse_parameters( ZEND_NUM_ARGS()	TSRMLS_CC, "s", &attribute_name, &attribute_name_length ) == FAILURE )
	{
		return;
	}

	std::map<std::string, aophp::FunctionHook>::iterator found = aophp::function_hooks.find( attribute_name );
	if ( found != aophp::function_hooks.end() )
	{
		aophp::function_hooks.erase( found );
	}

	return;
}	

void aophp::aophp_zend_execute( zend_op_array* op_array TSRMLS_DC )
{
	// Make sure to check if any of the specified function hooks (if any)
	// ask for the execution of the original function to be prevented...
	bool run_original_function = true;

	if ( aophp::function_hooks.size() > 0 )
	{
		// Look at the attributes for the called function...
		std::vector<aophp::Attribute>* attributes = aophp::get_attributes( op_array ); 
		// If any of the attributes have been added as a function hook,
		// then we want to call the appropriate function...
		for ( std::map<std::string, aophp::FunctionHook>::iterator hookIt = aophp::function_hooks.begin();hookIt != aophp::function_hooks.end(); hookIt ++ )
		{
			for ( std::vector<aophp::Attribute>::iterator attributeIt = attributes->begin(); attributeIt != attributes->end(); attributeIt++ )
			{
				aophp::Attribute attribute = *attributeIt;
				std::pair<std::string, aophp::FunctionHook> pair = *hookIt;
				aophp::FunctionHook hook = pair.second;
				if ( hook.attribute_name == attribute.name )
				{
					// We want to get the attribute values and present them nicely to the hook function
					// so they don't have to go looking for them...
					zval* attributes_arg;
					MAKE_STD_ZVAL( attributes_arg );
	
					// Iterate through again, so that we can get a collection of all the attributes with the current name (if more than one)...	
					std::vector<std::string> values;
					for ( std::vector<aophp::Attribute>::iterator attributeIt2 = attributes->begin(); attributeIt2 != attributes->end(); attributeIt2 ++ )
					{
						aophp::Attribute dupAtt = *attributeIt2;
						if ( dupAtt.name == attribute.name )
						{
							values.push_back( dupAtt.value );
						}
					}
	
					// Build a zend_array of attributes to be passed to the hook function...
					if ( hook.accept_multiple_attributes )
					{
						array_init( attributes_arg );
						for ( std::vector<std::string>::iterator valueIt = values.begin(); valueIt != values.end(); valueIt++ )
						{
							add_next_index_string( attributes_arg, const_cast<char *>( valueIt->c_str() ), 1 );
						}
					}
					else
					{
						// If there is more than one tag with the specified name, but we were only expecting one...
						if ( values.size() > 1 )
						{
							// Really doesn't hurt anything, the hook will go ahead as planned...
							zend_error( E_NOTICE, "Found %d attributes with the name '%s', discarding all but the first", values.size(), attribute.name.c_str() );
						}
	
						ZVAL_STRING( attributes_arg, const_cast<char *>( attribute.value.c_str() ), 1);
					}
	
					zval* hook_handler_val;
					MAKE_STD_ZVAL( hook_handler_val );
					ZVAL_STRING( hook_handler_val, const_cast<char *>( hook.hook_handler.c_str() ), 1);
	
					zval** args[2];
					zval* arg1;
					MAKE_STD_ZVAL( arg1 );
					ZVAL_STRING( arg1, op_array->function_name, strlen( op_array->function_name ) );
					args[0] = &arg1;
					args[1] = &attributes_arg;
	
					zval* return_value;
					if ( call_user_function_ex( EG( function_table ), NULL, hook_handler_val, &return_value, 2, args, 0, NULL TSRMLS_CC ) != SUCCESS )
					{
						// Not the end of the world, but the hook will not work...
						zend_error( E_WARNING, "Cannot hook to function %s", hook.hook_handler.c_str() );
					}
					else
					{
						// Find out whether we got a true or false out of the user defined function...
						convert_to_boolean( return_value );
						zend_bool return_success = Z_BVAL( *return_value );
	
						// ...see if we want to continue with the existing function...
						run_original_function &= ( ! hook.allow_prevent || return_success );
					}
	
						// Break, so that we can continue on looking for other function hooks...
				// otherwise, we could end up hooking into the function once for each duplicate attribute,
					// which is crap.
					break;
				} 
			}
		}
	}
	
	// ... and then do whatever is required...
	// if ( run_original_function )
	// {
		old_zend_execute( op_array TSRMLS_CC );
	// }
	// else
	// {
		// throw exception?
	// }
}

std::vector<aophp::Attribute>* aophp::get_attributes( zend_op_array* op_array )
{
	// First check if the requested attributes exist in the cache...
	std::map<std::string, std::vector<aophp::Attribute>*>::iterator found = aophp::cached_function_attributes.find( op_array->function_name );
	if ( found != aophp::cached_function_attributes.end() )
	{
		return found->second;
	}
	// Extract the attributes from the doc comments, stuff them in the cache, and then return them...
	else
	{
		std::vector<aophp::Attribute>* attributes = aophp::extract_attributes( op_array->doc_comment );

		// Cache vector...
		aophp::cached_function_attributes.insert( std::make_pair( std::string( op_array->function_name ), attributes ) );		
	
		// return...
		return attributes;
	}
  
}


std::vector<aophp::Attribute>* aophp::extract_attributes( std::string comments )
{
    std::vector<aophp::Attribute>* attributes = new std::vector<aophp::Attribute>();

    // split comments into lines '\n'
    std::vector<std::string> lines;
    unsigned int currentIndex = 0;
    unsigned int nextIndex;
    while ( currentIndex != comments.npos )
    {
    	nextIndex = comments.find( '\n', currentIndex + 1 );
    	std::string line = comments.substr( currentIndex, nextIndex - currentIndex );

    	// Trim white space
    	while ( line.size() > 0 && ( line[0] == ' ' || line[0] == '\t' || line[0] == '\n' ) )
    	{
    		line.erase( 0, 1 );
    	}

    	// Remove any comment start/end tags
    	static std::string commentSpecifiers[] = { "/**", "*/", "*" };
    	static int commentSpecifiersCount = 3;
    	for ( int i = 0; i < commentSpecifiersCount; i ++ )
    	{
    		if ( line.substr( 0, commentSpecifiers[i].size() ) == commentSpecifiers[i] )
    		{
    			line.erase( 0, commentSpecifiers[i].size() );
    		}
    	}

    	// Then trim any more white space at the beginning...
    	while ( line.size() > 0 && ( line[0] == ' ' || line[0] == '\t' || line[0] == '\n' ) )
    	{
    		line.erase( 0, 1 );
    	}

    	// ...and also any whitespace at the end...
    	while ( line.size() > 0 && ( line[line.size() - 1] == ' ' || line[ line.size() - 1 ] == '\t' || line[ line.size() - 1 ] == '\n' ) )
    	{
    		line.erase( line.size() - 1 );
    	}

    	lines.push_back( line );
    	currentIndex = nextIndex;
    	// std::cout << "Line: '" << line << "'" << std::endl;
    }

    for ( std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); it ++)
    {
    	//   while first char == '*' or white space
    	std::string line = *it;

		//   if new first char == '@'
    	if ( line.size() > 1 && line[0] == '@' )
    	{
    		//     this line looks like an attribute...
			//     take the first word as the attribute name
			//     then start with the rest of the line as the value...
    		int attributeEndIndex = line.find( ' ' );
    		std::string attribute = line.substr( 1, attributeEndIndex - 1 );
    		std::string value = line.substr( attributeEndIndex + 1, line.size() );

    		//     while nextline's first char != '@'
    		while ( ( (it + 1) != lines.end() ) &&
    				( (it + 1)->size() > 0 )&&
    				( (it + 1)->at(0) != '@' ) )
    		{
    			value += ' ';
    			value += *(it + 1);
    			it ++;
    		}

    		// std::cout << "Attribute: '" << attribute << "', Value: '" << value << "'" << std::endl;
    		aophp::Attribute attr;
    		attr.name = attribute;
    		attr.value = value;
    		attributes->push_back( attr );

    	}

    }

    return attributes;
}

