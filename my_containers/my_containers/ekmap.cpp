#include "ekmap.h"
#include <cstdio>
#include <stdexcept>

namespace EK
{

struct Map::Node
{
	Node * parent = nullptr;
	Node * left = nullptr;
	Node * right = nullptr;
	bool is_black = true; // true - black, false - red
	std::string value;
	int key = 0;

	Node() = default;

	Node( Node * parent, Node * left, Node * right,
			 int key, bool is_black, const std::string& value )
	{
		this->parent = parent;
		this->left = left;
		this->right = right;
		this->key = key;
		this->is_black = is_black;
		this->value = value;
	}

	bool is_red() { return !is_black; }
};

class Map::InternIter
{
// Internal interator provides access to nodes directly.
// It's required for a few private methods.
// Also it is a basis for external iterators.
public:
	explicit InternIter( Node * );
	InternIter& operator++();
	InternIter& operator--();
	bool operator==( InternIter ) const;
	bool operator!=( InternIter ) const;
	Map::Node& operator*();
	Map::Node * operator->();
private:
	Node * node_ = nullptr;
};

Map::InternIter::InternIter( Node * node ) : node_( node )
{
}

Map::InternIter& Map::InternIter::operator++()
{
	if ( node_ != nullptr )
	{
		node_ = s_find_successor_( node_ );
	}
	return *this;
}

Map::InternIter& Map::InternIter::operator--()
{
	if ( node_ != nullptr )
	{
		node_ = s_find_predecessor_( node_ );
	}
	return *this;
}

bool Map::InternIter::operator==( Map::InternIter other ) const
{
	return node_ == other.node_;
}
bool Map::InternIter::operator!=( Map::InternIter other ) const
{
	return !( *this == other );
}

Map::Node& Map::InternIter::operator*()
{
	if ( node_ == nullptr )
	{
		throw std::length_error( "Iterator is out of range." );
	}
	return *node_;
}

Map::Node * Map::InternIter::operator->()
{
	if ( node_ == nullptr )
	{
		throw std::length_error( "Iterator is out of range." );
	}
	return node_;
}

class Map::CInternIter
{
public:
	explicit CInternIter( Node * );
	CInternIter& operator++();
	CInternIter& operator--();
	bool operator==( CInternIter ) const;
	bool operator!=( CInternIter ) const;
	Map::Node& operator*();
	Map::Node * operator->();
private:
	Node * node_ = nullptr;
};

Map::CInternIter::CInternIter( Node * node ) : node_( node )
{
}

Map::CInternIter& Map::CInternIter::operator++()
{
	if ( node_ != nullptr )
	{
		node_ = s_find_successor_( node_ );
	}
	return *this;
}

Map::CInternIter& Map::CInternIter::operator--()
{
	if ( node_ != nullptr )
	{
		node_ = s_find_predecessor_( node_ );
	}
	return *this;
}

bool Map::CInternIter::operator==( Map::CInternIter other ) const
{
	return node_ == other.node_;
}
bool Map::CInternIter::operator!=( Map::CInternIter other ) const
{
	return !( *this == other );
}

Map::Node& Map::CInternIter::operator*()
{
	if ( node_ == nullptr )
	{
		throw std::length_error( "Iterator is out of range." );
	}
	return *node_;
}

Map::Node * Map::CInternIter::operator->()
{
	if ( node_ == nullptr )
	{
		throw std::length_error( "Iterator is out of range." );
	}
	return node_;
}

class Map::Iterator
{
	friend class Map;
public:
	Iterator& operator++() { ++iter_; return *this; }
	Iterator& operator--() { --iter_; return *this; }
	bool operator==( Iterator other ) const { return iter_ == other.iter_; }
	bool operator!=( Iterator other ) const { return !( *this == other ); }
	std::pair<int, std::string&> operator*() { return { iter_->key, iter_->value }; };
	std::unique_ptr<std::pair<int, std::string&>> operator->()
	{
		return std::make_unique<std::pair<int, std::string&>>( iter_->key, iter_->value );
	}
private:
	InternIter iter_;
	explicit Iterator( InternIter intern_iter ) : iter_( intern_iter ) {}
};

class Map::CIterator
{
	friend class Map;
public:
	CIterator& operator++() { ++iter_; return *this; }
	CIterator& operator--() { --iter_; return *this; }
	bool operator==( CIterator other ) const { return iter_ == other.iter_; }
	bool operator!=( CIterator other ) const { return !( *this == other ); }
	std::pair<int, const std::string&> operator*() { return { iter_->key, iter_->value }; };
	std::unique_ptr<std::pair<int, const std::string&>> operator->()
	{
		return std::make_unique<std::pair<int, const std::string&>>( iter_->key, iter_->value );
	}
private:
	CInternIter iter_;
	explicit CIterator( CInternIter iter ) : iter_( iter ) {}
};

Map::Map()
{
	root_ = nullptr;
}

Map::Map( const std::vector<std::pair<int, std::string>>& v )
{
	for ( auto& pair : v )
	{
		insert( pair );
	}
}

Map::Map( const std::initializer_list<std::pair<int, std::string>>& list )
{
	for ( auto& pair : list )
	{
		insert( pair );
	}
}

Map::~Map()
{
}

Map::InternIter Map::ibegin() { return InternIter( get_minimum_() ); }
Map::InternIter Map::iend() { return InternIter( nullptr ); }
Map::InternIter Map::irbegin() { return InternIter( get_maximum_() ); }
Map::InternIter Map::irend() { return InternIter( nullptr ); }
Map::CInternIter Map::ibegin() const { return CInternIter( get_minimum_() ); }
Map::CInternIter Map::iend() const { return CInternIter( nullptr ); }
Map::CInternIter Map::irbegin() const { return CInternIter( get_maximum_() ); }
Map::CInternIter Map::irend() const { return CInternIter( nullptr ); }

Map::Iterator Map::begin() { return Iterator( ibegin() ); }
Map::Iterator Map::end() { return Iterator( iend() ); }
Map::Iterator Map::rbegin() { return Iterator( irbegin() ); }
Map::Iterator Map::rend() { return Iterator( irend() ); }
Map::CIterator Map::begin() const { return CIterator( ibegin() ); }
Map::CIterator Map::end() const { return CIterator( iend() ); }
Map::CIterator Map::rbegin() const { return CIterator( irbegin() ); }
Map::CIterator Map::rend() const { return CIterator( irend() ); }

Map::Iterator Map::find( int key )
{
	auto node = find_( key );
	return ( node != nullptr ) ? Iterator( InternIter( node ) ) : end();
}

Map::CIterator Map::find( int key ) const
{
	auto node = find_( key );
	return ( node != nullptr ) ? CIterator( CInternIter( node ) ) : end();
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
	return counter_;
}

Map::Node * Map::find_( int key ) const
{
	auto current = root_;
	while ( current != nullptr && current->key != key )
	{
		current = ( current->key > key ) ? current->left : current->right;
	}
	return current;
}

void Map::insert( const std::pair<int, std::string>& key_value_pair )
{
	insert( key_value_pair.first, key_value_pair.second );
}

Map::Node * Map::get_minimum_() const
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

Map::Node * Map::get_maximum_() const
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

void Map::transplant_( const Node * target, Node * to_transplant )
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

const Map::Node * Map::s_find_successor_in_right_branch_( const Node * node )
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

const Map::Node * Map::s_find_predecessor_in_left_branch_( const Node * node )
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

const Map::Node * Map::s_find_successor_( const Node * node )
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

Map::Node * Map::s_find_successor_( Node * node )
{
	return const_cast<Node *>( s_find_successor_( static_cast<const Node *>( node ) ) );
}

const Map::Node * Map::s_find_predecessor_( const Node * node )
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

Map::Node * Map::s_find_predecessor_( Node * node )
{
	return const_cast<Node *>( s_find_predecessor_( static_cast<const Node *>( node ) ) );
}

std::string Map::get_debug_output() const
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

std::string Map::format_line_( const Node * node ) const
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

std::string Map::check_red_black_tree_property_4_() const
{
	std::string result;
	for ( auto iter = ibegin(); iter != iend(); ++iter )
	{
		if ( iter->is_red() )
		{
			auto l = iter->left;
			auto r = iter->right;
			bool childs_are_black = ( l == nullptr || l->is_black ) && ( r == nullptr || r->is_black );
			if ( !childs_are_black )
			{
				std::string msg( "Property 4 is violated: Node with key " );
				msg += std::to_string( iter->key );
				msg += " has red child.\n";
				result.append( msg );
			}
		}
	}
	return result;
}

std::string Map::check_red_black_tree_property_5_() const
{
	std::vector<std::pair<int, unsigned>> black_heights;
	for ( auto iter = ibegin(); iter != iend(); ++iter )
	{
		if ( iter->left == nullptr || iter->right == nullptr )
		{
			unsigned black_counter = 0;
			auto current = &*iter;
			while ( current != nullptr )
			{
				if ( current->is_black )
				{
					++black_counter;
				}
				current = current->parent;
			}
			black_heights.push_back( { iter->key, black_counter } );
		}
	}

	if ( black_heights.size() < 2 )
	{
		return {};
	}

	for ( auto pair : black_heights )
	{
		if ( pair.second != black_heights[0].second )
		{
			std::string msg( "Property 5 is violated: black heights for nodes (key-height): " );
			for ( size_t i = 0; i < black_heights.size(); ++i )
			{
				auto pair = black_heights[i];
				auto sep = ( i != black_heights.size() - 1 ) ? ", " : "\n";
				msg += "(" + std::to_string( pair.first ) + "-" + std::to_string( pair.second ) + ")" + sep;
			}
			return msg;
		}
	}
	return {};
}

std::string Map::check_red_black_tree_properties() const
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
	if ( root_->is_red() )
	{
		result.append( "Property 2 is violated: Root is not black.\n" );
	}

	result += check_red_black_tree_property_4_();
	result += check_red_black_tree_property_5_();
	return result;
}

unsigned Map::get_black_height() const
{
	if ( root_ == nullptr )
	{
		return 0;
	}
	auto current = root_;
	unsigned black_node_counter = 1;
	while ( current->left != nullptr )
	{
		current = current->left;
		++black_node_counter;
	}
	return black_node_counter;
}

Map::Node * Map::insert_( int key, const std::string& value )
{
	// just insert node in binary tree and mark it as red
	if ( root_ == nullptr )
	{
		auto newNode = new Node( nullptr, nullptr, nullptr,
								 key, false, value );
		root_ = newNode;
		return newNode;
	}

	auto current = root_;
	while ( true )
	{
		if ( current->key == key )
		{
			current->value = value;
			return nullptr;
		}
		else if ( current->key > key )
		{
			if ( current->left == nullptr )
			{
				auto newNode = new Node( current, nullptr, nullptr,
										 key, false, value );
				current->left = newNode;
				return newNode;
			}
			current = current->left;
		}
		else
		{
			if ( current->right == nullptr )
			{
				auto newNode = new Node( current, nullptr, nullptr,
										 key, false, value );
				current->right = newNode;
				return newNode;
			}
			current = current->right;
		}
	}
}

Map::Node * Map::get_grandparent_( Node * node )
{
	return ( node->parent != nullptr ) ? node->parent->parent : nullptr;
}

Map::Node * Map::get_uncle_( Node * node )
{
	auto g = get_grandparent_( node );
	if ( g == nullptr )
	{
		return nullptr;
	}
	return ( g->left == node->parent ) ? g->right : g->left;
}

void Map::left_rotate_( Node * node )
{
	auto rhs = node->right;
	if ( node->parent != nullptr )
	{
		if ( node->parent->left == node->parent )
		{
			node->parent->left = rhs;
		}
		else
		{
			node->parent->right = rhs;
		}
	}
	else
	{
		root_ = rhs;
	}

	rhs->parent = node->parent;

	node->right = rhs->left;
	if ( rhs->left != nullptr )
	{
		rhs->left->parent = node;
	}

	rhs->left= node;
	node->parent = rhs;
}

void Map::right_rotate_( Node * node )
{
	auto lhs = node->left;
	if ( node->parent != nullptr )
	{
		if ( node->parent->left == node->parent )
		{
			node->parent->left = lhs;
		}
		else
		{
			node->parent->right = lhs;
		}
	}
	else
	{
		root_ = lhs;
	}

	lhs->parent = node->parent;

	node->left = lhs->right;
	if ( lhs->right != nullptr )
	{
		lhs->right->parent = node;
	}

	lhs->right = node;
	node->parent = lhs;
}

void Map::insert_fixup_( Node * n )
{
	// case 1:
	if ( n->parent == nullptr )
	{
		n->is_black = true;
		return;
	}
	
	// case 2:
	if ( n->parent->is_black )
	{
		return;
	}

	auto p = n->parent; // must be != nullptr and must be red
	auto g = n->parent->parent; // must be != nullptr and must be black
	auto u = get_uncle_( n ); // may be == nullptr

	// we assume that p is red
	// case 3:
	if ( u != nullptr && u->is_red() )
	{
		p->is_black = true;
		u->is_black = true;
		g->is_black = false;
		insert_fixup_( g );
		return;
	}

	// we assume that u is black (or nullptr).
	// Next cases have suffixes L and R. L means that p is left child of g and vice versa.

	// case 4L:
	if ( p->right == n && g->left == p )
	{
		left_rotate_( p );
		insert_fixup_( p );
		return;
	}

	// case 4R:
	if ( p->left == n && g->right == p )
	{
		right_rotate_( p );
		insert_fixup_( p );
		return;
	}

	// case 5L:
	if ( p->left == n && g->left == p )
	{
		right_rotate_( g );
		p->is_black = true;
		g->is_black = false;
		return;
	}

	// case 5R:
	if ( p->right == n && g->right == p )
	{
		left_rotate_( g );
		p->is_black = true;
		g->is_black = false;
		return;
	}
}

void Map::insert( int key, const std::string& value )
{
	auto n = insert_( key, value );
	if ( n != nullptr )
	{
		++counter_;
		insert_fixup_( n );
	}
}

void Map::erase( int key )
{

}

} // namespace EK

// TODO: consider moving static functions in nameless namespace
// it requires usually move Node definition in the top... 