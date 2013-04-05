
#include <string>
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
/**                                             \
 * Some normal comments                         \
 * can live in here before                      \
 * we follow up with some attributes...         \
 * @sb.access 1 Only group 1 can access this... \
 * @sb.log info New contact added.              \
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

	Attribute attr;
	attr.name = "sb.test-attribute";
	attr.value = "Some random value...";

	attributes->push_back( attr );

	// split comments into lines '\n'
	// foreach line
	//   while first char == '*' or white space
	//     remove first char
	//   end while
	//   if new first char == '@'
	//     this line looks like an attribute...
	//     take the first word as the attribute name
	//     then start with the rest of the line as the value...
	//     while nextline's first char == '@'
	//       attribute value += nextline, replacing '\n' with ' '
	//     end while
	//   end if
	// end foreach line

	return attributes;	
}
