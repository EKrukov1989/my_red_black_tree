#include <iostream>
#include "ekmap.h"
#include <map>

int main()
{
	std::vector<std::pair<int, std::string>> v =
	{ { 1, "Aharon" },
	{ 8, "Baruch" },
	{ 2, "Sarah" },
	{ 4, "Ichak" },
	{ 13, "Esther" },
	{ 6, "Rachael" },
	{ 0, "Caleb" },
	{ 3, "Mendel" } };

	EK::Map m;

	for ( auto pair : v )
	{
		m.insert( pair );
		std::cout << "Insertion of: " << pair.first << " " << pair.second << std::endl;
		std::cout << "DEBUG OUTPUT:" << std::endl << m.get_debug_output();
		std::cout << "BLACK HEIGHT: " << m.get_black_height() << std::endl;
		std::cout << m.check_red_black_tree_properties();
		std::cout << std::endl << std::endl;
	}

	system( "pause" );
	return 0;
}