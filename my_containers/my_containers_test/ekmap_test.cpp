#include "pch.h"
#include "../my_containers/ekmap.h"
#include "../my_containers/ekmap.cpp"

TEST(ekmap, insertion )
{
	auto m = EK::Map();
	EXPECT_EQ( m.size(), 0);
	m.insert( 12, "Aharon" );
	EXPECT_EQ( m.size(), 1 );
}

TEST( ekmap, search )
{
	auto m = EK::Map();
	m.insert( 2, "Aharon" );
	m.insert( 4, "Sarah" );
	m.insert( 3, "Baruch" );
	EXPECT_EQ( m.size(), 3 );

	EXPECT_EQ( m.at(2), "Aharon" );
	EXPECT_EQ( m.at( 3 ), "Baruch" );
	EXPECT_EQ( m.at( 4 ), "Sarah" );

	ASSERT_THROW( m.at( 1 ), std::out_of_range );
}

TEST( ekmap, list_initilaization )
{
	EK::Map m = { { 1, "Aharon" }, { 2, "Baruch" }, { 3, "Sarah" }, { 5, "Mendel" } };
	EXPECT_EQ( m.size(), 4 );
	EXPECT_EQ( m.at( 2 ), "Baruch" );
	EXPECT_EQ( m.at( 5 ), "Mendel" );
}

TEST( ekmap, iteration )
{
	const EK::Map m = { { 10, "Aharon" }, { 6, "Baruch" }, { 3, "Sarah" }, { 5, "Mendel" }, { 6, "Ichak" } };
	std::vector<std::pair<int, std::string>> right_order =
		{ { 3, "Sarah" }, { 5, "Mendel" }, { 6, "Ichak" }, { 10, "Aharon" } };
	size_t counter = 0;
	for ( auto& pair : m )
	{
		EXPECT_EQ( pair.first, right_order[counter].first );
		EXPECT_EQ( pair.second, right_order[counter].second );
		counter++;
	}
	EXPECT_EQ( counter, 4 );
}

TEST( ekmap, reversed_iteration )
{
	const EK::Map m = { { 10, "Aharon" }, { 6, "Baruch" }, { 3, "Sarah" }, { 5, "Mendel" }, { 6, "Ichak" } };
	std::vector<std::pair<int, std::string>> right_order =
		{ { 3, "Sarah" }, { 5, "Mendel" }, { 6, "Ichak" }, { 10, "Aharon" } };
	size_t counter = right_order.size() - 1;
	for ( auto iter = m.rbegin(); iter != m.rend(); --iter )
	{
		EXPECT_EQ( iter->first, right_order[counter].first );
		EXPECT_EQ( iter->second, right_order[counter].second );
		counter--;
	}
	EXPECT_EQ( counter, -1 );
}

TEST( ekmap, insertion_rb_check )
{
	std::vector<std::pair<int, std::string>> pairs =
		{ { 1, "Aharon" }, { 8, "Baruch" }, { 2, "Sarah" }, { 4, "Ichak" },
		{ 13, "Esther" }, { 6, "Rachael" }, { 0, "Caleb" }, { 3, "Mendel" } };
	EK::Map m;
	EXPECT_TRUE( m.check_red_black_tree_properties().empty() );
	for ( auto pair : pairs )
	{
		m.insert( pair );
		EXPECT_TRUE( m.check_red_black_tree_properties().empty() );
	}
}

TEST( ekmap, find )
{
	const EK::Map m = { { 10, "Aharon" }, { 6, "Baruch" }, { 3, "Sarah" } };
	EXPECT_EQ( m.find( 2 ), m.end() );
	auto it6 = m.find( 6 );
	EXPECT_EQ( it6->first, 6 );
	EXPECT_EQ( it6->second, "Baruch" );
	--it6;
	EXPECT_EQ( it6, m.begin());
}

TEST( ekmap, erase )
{
	auto m = EK::Map();
	m.insert( 2, "Aharon" );
	m.insert( 4, "Sarah" );
	m.insert( 3, "Baruch" );
	EXPECT_EQ( m.size(), 3 );
	m.erase( 3 );
	EXPECT_EQ( m.size(), 2 );
	ASSERT_NO_THROW( m.erase( 1 ) );
	EXPECT_EQ( m.size(), 2 );

	EXPECT_EQ( m.at( 2 ), "Aharon" );
	ASSERT_THROW( m.at( 3 ), std::out_of_range );
	EXPECT_EQ( m.at( 4 ), "Sarah" );
}

