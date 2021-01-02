#include <iostream>
#include "ekmap.h"
#include <map>

int main()
{
	//EK::Map m;
	//m.insert( 1, "Aharon" );
	//m.insert( 2, "Baruch" );
	//m.insert( 4, "Sarah" );
	//m.insert( 5, "Mendel" );

	EK::Map m = { { 1, "Aharon" }, { 2, "Baruch" }, { 3, "Sarah" }, { 5, "Mendel" } };

	std::cout << m.get_debug_output() << std::endl;
	std::cout << std::endl;


	system( "pause" );
	return 0;
}