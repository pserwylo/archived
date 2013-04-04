#include <string>
#include <cstring>
#include <vector>
#include <iostream>

struct Attribute
{
    std::string name;
    std::string value;
};

std::vector<Attribute>* getAttributes( std::string );

int main()
{
    std::string comments = "\
/**                                             \n\
 * Some normal comments                         \n\
 * can live in here before                      \n\
 * we follow up with some attributes...         \n\
 * @sb.access 1 Only group 1 can access this... \n\
 * @sb.log info New contact added. This is an   \n\
 * example of a multiline attribute...          \n\
 * @sb.log info Another message to log.         \n\
 */";

    std::vector<Attribute>* attributes = getAttributes( comments );

    for (std::vector<Attribute>::iterator it = attributes->begin(); it != attributes->end(); it ++)
    {
        std::cout << "@" << it->name << ": '" << it->value << "'" << std::endl;
    }

    return 1;
}

std::vector<Attribute>* getAttributes( std::string comments )
{
    std::vector<Attribute>* attributes = new std::vector<Attribute>();

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
    		Attribute attr;
    		attr.name = attribute;
    		attr.value = value;
    		attributes->push_back( attr );

    	}

    }

    return attributes;
}
