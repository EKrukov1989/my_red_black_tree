#include "pch.h"
#include "../my_containers/ekmap.h"
#include "../my_containers/ekmap.cpp"

TEST(ekmap, insertion )
{
	auto m = EK::Map();
	EXPECT_EQ( m.size(), 0);
	m.insert( 12, "Aaron" );
	EXPECT_EQ( m.size(), 1 );
}

TEST( ekmap, search )
{
	auto m = EK::Map();
	m.insert( 2, "Aaron" );
	m.insert( 4, "Sarah" );
	m.insert( 3, "Boruh" );
	EXPECT_EQ( m.size(), 3 );

	EXPECT_EQ( m.at(2), "Aaron" );
	EXPECT_EQ( m.at( 3 ), "Boruh" );
	EXPECT_EQ( m.at( 4 ), "Sarah" );

	ASSERT_THROW( m.at( 1 ), std::out_of_range );
}

TEST( ekmap, erase )
{
	auto m = EK::Map();
	m.insert( 2, "Aaron" );
	m.insert( 4, "Sarah" );
	m.insert( 3, "Boruh" );
	EXPECT_EQ( m.size(), 3 );
	m.erase( 3 );
	EXPECT_EQ( m.size(), 2 );
	ASSERT_NO_THROW( m.erase( 1 ) );
	EXPECT_EQ( m.size(), 2 );

	EXPECT_EQ( m.at( 2 ), "Aaron" );
	ASSERT_THROW( m.at( 3 ), std::out_of_range );
	EXPECT_EQ( m.at( 4 ), "Sarah" );
}