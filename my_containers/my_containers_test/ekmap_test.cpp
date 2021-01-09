#include "pch.h"

#include "../my_containers/ekmap.h"
#include "../my_containers/ekmap.cpp"

namespace
{

std::vector<int> s_get_random_order( unsigned n )
{
	// returns n numbers in interval [0..n) without repetitions
	// example for n ==5: -> { 3, 0, 2, 4, 1 }

	std::srand( 10 );
	std::vector<int> r_order;
	r_order.resize( n, -1 );
	for ( int i = 0; i < n; ++i )
	{
		while ( true )
		{
			int r = std::rand() % n;
			bool exists = false;
			for ( auto elt : r_order )
			{
				if ( elt == r )
				{
					exists = true;
					break;
				}
			}

			if ( !exists )
			{
				r_order[i] = r;
				break;
			}
		}
	}
	return r_order;
}

void s_erase_elt_from_list( std::list<std::pair<int, std::string>>& list, int key )
{
	for ( auto iter = list.begin(); iter != list.end(); ++iter )
	{
		if ( iter->first == key )
		{
			list.erase( iter );
			return;
		}
	}
	assert( false );
}

} // nameless namespace

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

TEST( ekmap, simple_erase )
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

TEST( ekmap, multi_erase )
{
	std::list<std::pair<int, std::string>> control_list;
	auto m = EK::Map();
	size_t n = 100;
	for ( int i = 0; i < n; ++i )
	{
		auto pair = std::make_pair( i, std::to_string( i * 100 ) );
		control_list.push_back( pair );
		m.insert( pair );
	}

	auto r_order = s_get_random_order( n );

	for ( int i = 0; i < n; ++i )
	{
		if ( i ==  5)
		{
			std::cout << "&";
		}

		EXPECT_EQ( m.size(), n - i );
		int key = r_order[i];
		m.erase( key );
		s_erase_elt_from_list( control_list, key );
		auto clist_iter = control_list.begin();
		for ( auto& map_pair : m )
		{
			EXPECT_EQ( map_pair.first, clist_iter->first);
			EXPECT_EQ( map_pair.second, clist_iter->second );
			++clist_iter;
		}
		EXPECT_TRUE( m.check_red_black_tree_properties().empty() );
	}
	EXPECT_EQ( m.size(), 0 );
}

TEST( ekmap, copy_constructor )
{
	const std::vector<std::pair<int, std::string>> control =
		{ { 3, "Aharon" }, { 6, "Baruch" }, { 10, "Sarah" } }; // must be ordered by key
	EK::Map m1( control );
	const EK::Map m2( m1 );
	// changes of m1 must not apply on m2 anymore:
	m1.erase( 6 ); 
	auto control_iter = control.begin();
	for ( auto& pair : m2 )
	{
		EXPECT_EQ( pair.first, control_iter->first );
		EXPECT_EQ( pair.second, control_iter->second );
		++control_iter;
	}
	EXPECT_EQ( control_iter, control.end());
}

TEST( ekmap, assignment_operator )
{
	const std::vector<std::pair<int, std::string>> control =
		{ { 3, "Aharon" }, { 6, "Baruch" }, { 10, "Sarah" } }; // must be ordered by key
	EK::Map m1( control );
	auto m2 = EK::Map();
	m2 = m1;
	// changes of m1 must not apply on m2 anymore:
	m1.erase( 6 );

	auto control_iter = control.begin();
	for ( auto& pair : m2 )
	{
		EXPECT_EQ( pair.first, control_iter->first );
		EXPECT_EQ( pair.second, control_iter->second );
		++control_iter;
	}
	EXPECT_EQ( control_iter, control.end() );
}