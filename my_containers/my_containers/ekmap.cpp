#include "ekmap.h"
#include <cstdio>
#include <stdexcept>

namespace
{

const EK::MapNode * s_find_successor_in_right_branch_( const EK::MapNode * node )
{
	if ( node->right == nullptr )
	{
		return nullptr;
	}
	auto current = node->right;
	while ( current->left != nullptr )
	{
		current = current->left;
	}
	return current;
}

const EK::MapNode * s_find_predecessor_in_left_branch_( const EK::MapNode * node )
{
	if ( node->left == nullptr )
	{
		return nullptr;
	}
	auto current = node->left;
	while ( current->right != nullptr )
	{
		current = current->right;
	}
	return current;
}

const EK::MapNode * s_find_successor_( const EK::MapNode * node )
{
	auto current = s_find_successor_in_right_branch_( node );
	if ( current == nullptr )
	{
		current = node;
		while ( current != nullptr && current->key <= node->key )
		{
			current = current->parent;
		}
	}
	return current;
}

EK::MapNode * s_find_successor_( EK::MapNode * node )
{
	return const_cast<EK::MapNode *>( s_find_successor_( static_cast<const EK::MapNode *>( node ) ) );
}

const EK::MapNode * s_find_predecessor_( const EK::MapNode * node )
{
	auto current = s_find_predecessor_in_left_branch_( node );
	if ( current == nullptr )
	{
		current = node;
		while ( current != nullptr && current->key >= node->key )
		{
			current = current->parent;
		}
	}
	return current;
}

EK::MapNode * s_find_predecessor_( EK::MapNode * node )
{
	return const_cast<EK::MapNode *>( s_find_predecessor_( static_cast<const EK::MapNode *>( node ) ) );
}

} // nameless namespace

namespace EK
{

Map::Iterator::Iterator( MapNode * node ) : node_( node )
{
}

Map::Iterator& Map::Iterator::operator++()
{
	if ( node_ != nullptr )
	{
		node_ = s_find_successor_( node_ );
	}
	return *this;
}

Map::Iterator& Map::Iterator::operator--()
{
	if ( node_ != nullptr )
	{
		node_ = s_find_predecessor_( node_ );
	}
	return *this;
}

bool Map::Iterator::operator==( Map::Iterator other ) const
{
	return node_ == other.node_;
}
bool Map::Iterator::operator!=( Map::Iterator other ) const
{
	return !( *this == other );
}

std::pair<int, std::string&> Map::Iterator::operator*()
{
	if ( node_ == nullptr )
	{
		throw std::length_error( "Iterator is out of range." );
	}
	return { node_->key, node_->value };
}

std::unique_ptr<std::pair<int, std::string&>> Map::Iterator::operator->()
{
	if ( node_ == nullptr )
	{
		throw std::length_error( "Iterator is out of range." );
	}
	return std::make_unique<std::pair<int, std::string&>>( node_->key, node_->value );
}

Map::ConstIterator::ConstIterator( const MapNode * node ) : node_( node )
{
}

Map::ConstIterator& Map::ConstIterator::operator++()
{
	if ( node_ != nullptr )
	{
		node_ = s_find_successor_( node_ );
	}
	return *this;
}

Map::ConstIterator& Map::ConstIterator::operator--()
{
	if ( node_ != nullptr )
	{
		node_ = s_find_predecessor_( node_ );
	}
	return *this;
}

bool Map::ConstIterator::operator==( Map::ConstIterator other ) const
{
	return node_ == other.node_;
}

bool Map::ConstIterator::operator!=( Map::ConstIterator other ) const
{
	return !( *this == other );
}

std::pair<int, const std::string&> Map::ConstIterator::operator*()
{
	if ( node_ == nullptr )
	{
		throw std::length_error( "Iterator is out of range." );
	}
	return { node_->key, node_->value };
}

std::unique_ptr<std::pair<int, const std::string&>> Map::ConstIterator::operator->()
{
	if ( node_ == nullptr )
	{
		throw std::length_error( "Iterator is out of range." );
	}
	return std::make_unique<std::pair<int, const std::string&>>( node_->key, node_->value );
}

Map::Map()
{
	root_ = nullptr;
}

Map::~Map()
{
}

Map::Iterator Map::begin() { return Iterator( get_minimum_() ); }
Map::Iterator Map::end() { return Iterator( nullptr ); }
Map::Iterator Map::rbegin() { return Iterator( get_maximum_() ); }
Map::Iterator Map::rend() { return Iterator( nullptr ); }
Map::ConstIterator Map::begin() const { return ConstIterator( get_minimum_() ); }
Map::ConstIterator Map::end() const { return ConstIterator( nullptr ); }
Map::ConstIterator Map::rbegin() const { return ConstIterator( get_maximum_() ); }
Map::ConstIterator Map::rend() const { return ConstIterator( nullptr ); }

Map::Iterator Map::begin()
{
	return Iterator( get_minimum_() );
}

Map::Iterator Map::end()
{
	return Iterator( nullptr );
}

Map::Iterator Map::rbegin()
{
	return Iterator( get_maximum_() );
}

Map::Iterator Map::rend()
{
	return Iterator( nullptr );
}

Map::ConstIterator Map::begin() const
{
	return ConstIterator( get_minimum_() );
}

Map::ConstIterator Map::end() const
{
	return ConstIterator( nullptr );
}

Map::ConstIterator Map::rbegin() const
{
	return ConstIterator( get_maximum_() );
}

Map::ConstIterator Map::rend() const
{
	return ConstIterator( nullptr );
}

std::size_t Map::count( int key ) const
{
	return static_cast<bool>( find_( key ) == nullptr );
}

std::string Map::at( int key ) const
{
	auto node = find_( key );
	if ( node == nullptr )
	{
		throw std::out_of_range( "Key is not found." );
	}
	else
	{
		return node->value;
	}
}

std::size_t Map::size() const
{
	return counter;
}

MapNode * Map::find_( int key ) const
{
	auto current = root_;
	while ( current != nullptr && current->key != key )
	{
		current = ( current->key > key ) ? current->left : current->right;
	}
	return current;
}

void Map::insert( int key, const std::string& value )
{
	if ( root_ == nullptr )
	{
		auto newNode = new MapNode( nullptr, nullptr, nullptr,
									key, true, value );
		root_ = newNode;
		++counter;
		return;
	}

	auto current = root_;
	while ( true )
	{
		if ( current->key == key )
		{
			current->value = value;
			break;
		}
		else if ( current->key > key )
		{
			if ( current->left == nullptr )
			{
				auto newNode = new MapNode( current, nullptr, nullptr,
											key, true, value );
				current->left = newNode;
				++counter;
				break;
			}
			current = current->left;
		}
		else
		{
			if ( current->right == nullptr )
			{
				auto newNode = new MapNode( current, nullptr, nullptr,
											key, true, value );
				current->right = newNode;
				++counter;
				break;
			}
			current = current->right;
		}
	}
}

MapNode * Map::get_minimum_() const
{
	if ( root_ == nullptr )
	{
		return nullptr;
	}

	auto current = root_;
	while ( current->left != nullptr )
	{
		current = current->left;
	}
	return current;
}

MapNode * Map::get_maximum_() const
{
	if ( root_ == nullptr )
	{
		return nullptr;
	}

	auto current = root_;
	while ( current->right != nullptr )
	{
		current = current->right;
	}
	return current;
}

std::string Map::get_debug_output()
{
	std::string result;
	auto current = get_minimum_();

	while ( current != nullptr )
	{
		result += format_line_( current ) + '\n';
		current = s_find_successor_( current );
	}
	return result;
}

std::string Map::format_line_( const MapNode * node ) const
{
	auto k = std::to_string( node->key );
	auto p = ( node->parent != nullptr ) ? std::to_string( node->parent->key ) : "nul";
	auto l = ( node->left != nullptr ) ? std::to_string( node->left->key ) : "nul";
	auto r = ( node->right != nullptr ) ? std::to_string( node->right->key ) : "nul";
	auto c = ( node->is_black ) ? "B" : "R";
	auto v = ( node->value.length() < 10 ) ? node->value : node->value.substr( 10 );

	char line[80];
	sprintf_s( line, "K=%-3s PK=%-3s LK=%-3s RK=%-3s C=%s V=%s",
			   k.data(), p.data(), l.data(), r.data(), c, v.data() );
	auto result = std::string( line );
	return result;
}

void Map::transplant_( const MapNode * target, MapNode * to_transplant )
{
	if ( target->parent == nullptr )
	{
		to_transplant->parent = nullptr;
		return;
	}
	
	if ( target->parent->left == target )
	{
		target->parent->left = to_transplant;
	}
	else
	{
		target->parent->right = to_transplant;
	}
	to_transplant->parent = target->parent;
}

MapNode * find_successor_in_right_branch_( MapNode * node )
{
	// failed
	auto current = node->right;
	while ( current->left != nullptr )
	{
		current = current->left;
	}
	return current;
}

void Map::erase( int key )
{
	//auto to_erase = find_( key );
	//if ( to_erase == nullptr )
	//{
	//	return;
	//}

	//auto current = node;
	//if ( node->right != nullptr )
	//{
	//	current = node->right;
	//	while ( current->left != nullptr )
	//	{
	//		current = current->left;
	//	}
	//}
	//else
	//{
	//	while ( current != nullptr && current->key <= node->key )
	//	{
	//		current = current->parent;
	//	}
	//}

	//return current;

	//auto successor = find_successor_( to_erase );
	//if ( successor == nullptr )
	//{
	//	auto left_node = to_erase->left;
	//	if ( left_node == nullptr )
	//	{
	//		if ( to_erase->parent->left == to_erase )
	//		{
	//			to_erase->parent->left = nullptr;
	//		}
	//		else
	//		{
	//			to_erase->parent->right = nullptr;
	//		}
	//	}

	//	left_node->parent = to_erase->parent;
	//	if ( to_erase->parent != nullptr )
	//	{
	//		if ( to_erase->parent->left == to_erase )
	//		{
	//			to_erase->parent->left = left_node;
	//		}
	//		else
	//		{
	//			to_erase->parent->right = left_node;
	//		}
	//	}
	//}
}

std::string Map::check_rb_tree()
{
	/*
	Properties of red-black tree:
	1. Each node is either red of black.
	2. The root is black.
	3. All leaves are black.
	4. If a node is red, then both its children are black.
	5. Every path from a given node to any of its descendant leaves
	goes through the same number of black nodes.

	Properties 1 and 3 don't require a check.
	*/

	if ( root_ == nullptr )
	{
		return {};
	}

	std::string result;
	if ( !root_->is_black )
	{
		result.append( "Property 2 is violated: Root is not black.\n" );
	}
}

} // namespace EK
