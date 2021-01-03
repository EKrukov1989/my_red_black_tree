#include <iostream>
#include "ekmap.h"
#include <map>

int main()
{
	EK::Map m = { { 1, "Aharon" },
					{ 8, "Baruch" },
					{ 2, "Sarah" },
					{ 4, "Ichak" },
					{ 13, "Esther" },
					{ 6, "Rachael" },
					{ 0, "Caleb" },
					{ 3, "Mendel" } };

	std::cout << "Initial state:" << std::endl;
	std::cout << "DEBUG OUTPUT:" << std::endl << m.get_debug_output();

	m.erase( 4 );
	std::cout << "After erase( 4 ):" << std::endl;
	std::cout << "DEBUG OUTPUT:" << std::endl << m.get_debug_output();
	std::cout << "BLACK HEIGHT: " << m.get_black_height() << std::endl;
	std::cout << m.check_red_black_tree_properties();

	system( "pause" );
	return 0;
}