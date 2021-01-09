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

Map::Iterator& Map::Iterator::operator++()
{
	++iter_;
	return *this;
}

Map::Iterator& Map::Iterator::operator--()
{ 
	--iter_;
	return *this;
}

bool Map::Iterator::operator==( Iterator other ) const
{
	return iter_ == other.iter_;
}

bool Map::Iterator::operator!=( Iterator other ) const
{
	return !( *this == other );
}

Map::Iterator::Iterator( InternIter intern_iter ) : iter_( intern_iter )
{
}

std::pair<int, std::string&> Map::Iterator::operator*()
{
	return { iter_->key, iter_->value };
};

std::unique_ptr<std::pair<int, std::string&>> Map::Iterator::operator->()
{
	return std::make_unique<std::pair<int, std::string&>>( iter_->key, iter_->value );
}

Map::CIterator& Map::CIterator::operator++()
{
	++iter_; return *this;
}

Map::CIterator& Map::CIterator::operator--()
{
	--iter_; return *this;
}

bool Map::CIterator::operator==( CIterator other ) const
{ 
	return iter_ == other.iter_;
}

bool Map::CIterator::operator!=( CIterator other ) const
{
	return !( *this == other );
}

Map::CIterator::CIterator( CInternIter iter ) : iter_( iter )
{
}

std::pair<int, const std::string&> Map::CIterator::operator*()
{
	return { iter_->key, iter_->value };
}

std::unique_ptr<std::pair<int, const std::string&>> Map::CIterator::operator->()
{
	return std::make_unique<std::pair<int, const std::string&>>( iter_->key, iter_->value );
}

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

Map::Map( Map& rhs )
{
	root_ = s_copy_tree_( rhs.root_ );
	counter_ = rhs.counter_;
}

Map& Map::operator=( const Map& rhs )
{
	root_ = s_copy_tree_( rhs.root_ );
	counter_ = rhs.counter_;
	return *this;
}

Map::~Map()
{
}

Map::InternIter Map::ibegin_() { return InternIter( get_minimum_() ); }
Map::InternIter Map::iend_() { return InternIter( nullptr ); }
Map::InternIter Map::irbegin_() { return InternIter( get_maximum_() ); }
Map::InternIter Map::irend_() { return InternIter( nullptr ); }
Map::CInternIter Map::ibegin_() const { return CInternIter( get_minimum_() ); }
Map::CInternIter Map::iend_() const { return CInternIter( nullptr ); }
Map::CInternIter Map::irbegin_() const { return CInternIter( get_maximum_() ); }
Map::CInternIter Map::irend_() const { return CInternIter( nullptr ); }

Map::Iterator Map::begin() { return Iterator( ibegin_() ); }
Map::Iterator Map::end() { return Iterator( iend_() ); }
Map::Iterator Map::rbegin() { return Iterator( irbegin_() ); }
Map::Iterator Map::rend() { return Iterator( irend_() ); }
Map::CIterator Map::begin() const { return CIterator( ibegin_() ); }
Map::CIterator Map::end() const { return CIterator( iend_() ); }
Map::CIterator Map::rbegin() const { return CIterator( irbegin_() ); }
Map::CIterator Map::rend() const { return CIterator( irend_() ); }

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

void Map::insert( int key, const std::string& value )
{
	auto n = insert_( key, value );
	if ( n != nullptr )
	{
		++counter_;
		insert_fixup_( n );
	}
}

void Map::insert( const std::pair<int, std::string>& key_value_pair )
{
	insert( key_value_pair.first, key_value_pair.second );
}

void Map::erase( int key )
{
	auto n = find_( key );
	if ( n == nullptr )
	{
		return;
	}

	if ( n->left != nullptr && n->right != nullptr )
	{
		auto successor = s_get_minimum_( n->right );
		swap_( n, successor );
	}

	erase_one_child_node_( n );
	--counter_;
}

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

std::string Map::get_debug_output() const
{
	std::string result;
	auto current = get_minimum_();

	while ( current != nullptr )
	{
		result += s_format_line_( current ) + '\n';
		current = s_find_successor_( current );
	}
	return result;
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

Map::Node * Map::find_( int key ) const
{
	auto current = root_;
	while ( current != nullptr && current->key != key )
	{
		current = ( current->key > key ) ? current->left : current->right;
	}
	return current;
}

Map::Node * Map::get_minimum_() const
{
	return s_get_minimum_( root_ );
}

Map::Node * Map::get_maximum_() const
{
	return s_get_maximum_( root_ );
}

void Map::left_rotate_( Node * n )
{
	auto rhs = n->right;
	if ( n->parent != nullptr )
	{
		auto*& parent_link = ( n->parent->left == n ) ? n->parent->left : n->parent->right;
		parent_link = rhs;
	}
	else
	{
		root_ = rhs;
	}

	rhs->parent = n->parent;

	n->right = rhs->left;
	if ( rhs->left != nullptr )
	{
		rhs->left->parent = n;
	}

	rhs->left = n;
	n->parent = rhs;
}

void Map::right_rotate_( Node * n )
{
	auto lhs = n->left;
	if ( n->parent != nullptr )
	{
		auto*& parent_link = ( n->parent->left == n ) ? n->parent->left : n->parent->right;
		parent_link = lhs;
	}
	else
	{
		root_ = lhs;
	}

	lhs->parent = n->parent;

	n->left = lhs->right;
	if ( lhs->right != nullptr )
	{
		lhs->right->parent = n;
	}

	lhs->right = n;
	n->parent = lhs;
}

void Map::swap_( Node * a, Node * b )
{
	if ( a == b )
	{
		return;
	}

	// 1. Save 'a' params:
	auto ap = a->parent;
	auto al = a->left;
	auto ar = a->right;
	bool ac = a->is_black;

	// 2. Copy params from 'b' to 'a':
	a->parent = b->parent;
	a->left = b->left;
	a->right = b->right;
	a->is_black = b->is_black;

	// 3. Fix links of other nodes that were connected with 'b':
	if ( a->parent == nullptr )
	{
		root_ = a;
	}
	else if ( a->parent == a )
	{
		a->parent = b;
	}
	else
	{
		auto*& child_link = ( a->parent->left == b ) ? a->parent->left : a->parent->right;
		child_link = a;
	}

	if ( a->left != nullptr )
	{
		if ( a->left == a )
		{
			a->left = b;
		}
		else
		{
			a->left->parent = a;
		}
	}

	if ( a->right != nullptr )
	{
		if ( a->right == a )
		{
			a->right = b;
		}
		else
		{
			a->right->parent = a;
		}
	}

	// 4. Copy saved params from 'a' to 'b':
	b->parent = ap;
	b->left = al;
	b->right = ar;
	b->is_black = ac;
	
	// 5. Fix external links for b:
	if ( b->parent == nullptr )
	{
		root_ = b;
	}
	else if ( b->parent == b )
	{
		b->parent = a;
	}
	else
	{
		auto*& child_link = ( b->parent->left == a ) ? b->parent->left : b->parent->right;
		child_link = b;
	}

	if ( b->left != nullptr )
	{
		if ( b->left == b )
		{
			b->left = a;
		}
		else
		{
			b->left->parent = b;
		}
	}

	if ( b->right != nullptr )
	{
		if ( b->right == b )
		{
			b->right = a;
		}
		else
		{
			b->right->parent = b;
		}
	}
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
	auto u = s_get_uncle_( n ); // may be == nullptr

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

void Map::remove_node_without_childs_( Node * node )
{
	if ( node->parent == nullptr )
	{
		root_ = nullptr;
	}
	else
	{
		auto*& parent_link = ( node->parent->left == node ) ? node->parent->left : node->parent->right;
		parent_link = nullptr;
	}
	delete node;
}

void Map::erase_one_child_node_( Node * node )
{
	// node may have at most one child
	if ( node->is_red() )
	{
		remove_node_without_childs_( node );
	}
	else
	{
		if ( node->left == nullptr && node->right == nullptr )
		{
			erase_fixup_( node );
			remove_node_without_childs_( node );
		}
		else
		{
			auto child = ( node->left != nullptr ) ? node->left : node->right;
			swap_( node, child );
			remove_node_without_childs_( node );
		}
	}
}

void Map::erase_fixup_( Node * node )
{
	if ( node->parent == nullptr )
	{
		// case 1:
		return;
	}

	auto sibling = s_get_sibling_( node );
	
	if ( sibling->is_red() )
	{
		// case 2:
		node->parent->is_black = false;
		sibling->is_black = true;
		if ( node->parent->left == node )
		{
			left_rotate_( node->parent );
		}
		else
		{
			right_rotate_( node->parent );
		}
		sibling = s_get_sibling_( node );
	}

	if ( node->parent->is_black && sibling->is_black &&
		 ( sibling->left == nullptr || sibling->left->is_black ) &&
		 ( sibling->right == nullptr || sibling->right->is_black ) )
	{
		// case 3:
		sibling->is_black = false;
		erase_fixup_( node->parent );
		return;
	}

	if ( node->parent->is_red() && sibling->is_black &&
		 ( sibling->left == nullptr || sibling->left->is_black ) &&
		 ( sibling->right == nullptr || sibling->right->is_black ) )
	{
		// case 4:
		node->parent->is_black = true;
		sibling->is_black = false;
		return;
	}

	if ( sibling->is_black )
	{
		// case 5:
		if ( node->parent->left == node )
		{
			if ( sibling->left != nullptr && sibling->left->is_red() &&
				( sibling->right == nullptr || sibling->right->is_black ) )
			{
				right_rotate_( sibling );
				sibling->is_black = false;
				sibling->parent->is_black = true;
				sibling = s_get_sibling_( node );
			}
		}
		else
		{
			if ( ( sibling->left == nullptr || sibling->left->is_black ) &&
				 sibling->right != nullptr && sibling->right->is_red() )
			{
				left_rotate_( sibling );
				sibling->is_black = false;
				sibling->parent->is_black = true;
				sibling = s_get_sibling_( node );
			}
		}
	}

	if ( sibling->is_black )
	{
		// case 6:
		if ( node->parent->left == node )
		{
			if ( sibling->right != nullptr && sibling->right->is_red() )
			{
				sibling->is_black = node->parent->is_black;
				node->parent->is_black = true;
				sibling->right->is_black = true;
				left_rotate_( node->parent );
			}
		}
		else
		{
			if ( sibling->left != nullptr && sibling->left->is_red() )
			{
				sibling->is_black = node->parent->is_black;
				node->parent->is_black = true;
				sibling->left->is_black = true;
				right_rotate_( node->parent );
			}
		}
	}
}

std::string Map::check_red_black_tree_property_4_() const
{
	std::string result;
	for ( auto iter = ibegin_(); iter != iend_(); ++iter )
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
	for ( auto iter = ibegin_(); iter != iend_(); ++iter )
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

Map::Node * Map::s_copy_tree_( Node* n )
{
	auto * n_copy = new Node( nullptr, nullptr, nullptr, n->key, n->is_black, n->value );

	if ( n->left != nullptr )
	{
		auto * left_copy = s_copy_tree_( n->left ); // left_copy - the root of n left subtree copy
		left_copy->parent = n_copy;
		n_copy->left = left_copy;
	}

	if ( n->right != nullptr )
	{
		auto * right_copy = s_copy_tree_( n->right ); // right_copy - the root of n right subtree copy
		right_copy->parent = n_copy;
		n_copy->right = right_copy;
	}
	return n_copy;
}

Map::Node * Map::s_get_grandparent_( Node * node )
{
	return ( node->parent != nullptr ) ? node->parent->parent : nullptr;
}

Map::Node * Map::s_get_sibling_( Node * node )
{
	if ( node->parent == nullptr )
	{
		return nullptr;
	}
	return ( node->parent->left == node ) ? node->parent->right : node->parent->left;
}

Map::Node * Map::s_get_uncle_( Node * node )
{
	auto g = s_get_grandparent_( node );
	if ( g == nullptr )
	{
		return nullptr;
	}
	return ( g->left == node->parent ) ? g->right : g->left;
}

Map::Node * Map::s_get_minimum_( Node * node )
{
	return const_cast<Node *>( s_get_minimum_( static_cast<const Node *>( node ) ) );
}

const Map::Node * Map::s_get_minimum_( const Node * node )
{
	if ( node == nullptr )
	{
		return nullptr;
	}

	auto current = node;
	while ( current->left != nullptr )
	{
		current = current->left;
	}
	return current;
}

Map::Node * Map::s_get_maximum_( Node * node )
{
	return const_cast<Node *>( s_get_maximum_( static_cast<const Node *>( node ) ) );
}

const Map::Node * Map::s_get_maximum_( const Node * node )
{
	if ( node == nullptr )
	{
		return nullptr;
	}

	auto current = node;
	while ( current->right != nullptr )
	{
		current = current->right;
	}
	return current;
}

Map::Node * Map::s_find_successor_( Node * node )
{
	return const_cast<Node *>( s_find_successor_( static_cast<const Node *>( node ) ) );
}

const Map::Node * Map::s_find_successor_( const Node * node )
{
	auto current = s_get_minimum_( static_cast<const Node *>( node->right ) );
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

Map::Node * Map::s_find_predecessor_( Node * node )
{
	return const_cast<Node *>( s_find_predecessor_( static_cast<const Node *>( node ) ) );
}

const Map::Node * Map::s_find_predecessor_( const Node * node )
{
	auto current = s_get_maximum_( static_cast<const Node *>( node->left ) );
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

std::string Map::s_format_line_( const Node * node )
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

} // namespace EK
