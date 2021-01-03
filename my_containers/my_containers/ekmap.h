#pragma once
#include <string>
#include <vector>
#include <memory>

namespace EK
{

class Map
{
public:
	class Iterator;
	class CIterator;
	Map();
	Map( const std::vector<std::pair<int, std::string>>& );
	Map( const std::initializer_list<std::pair<int, std::string>>& );
	~Map();

	std::size_t count( int key ) const;
	std::string at( int key ) const;
	std::size_t size() const;

	void insert( int key, const std::string& value );
	void insert( const std::pair<int, std::string>& key_value_pair );
	void erase( int key );

	Iterator begin();
	Iterator end();
	Iterator rbegin();
	Iterator rend();
	CIterator begin() const;
	CIterator end() const;
	CIterator rbegin() const;
	CIterator rend() const;

	std::string get_debug_output() const;
	std::string check_red_black_tree_properties() const;
	unsigned get_black_height() const;

private:
	struct Node;
	class InternIter;
	class CInternIter;
	Node * root_ = nullptr;
	std::size_t counter_ = 0;

	Node * find_( int key ) const;
	Node * get_minimum_() const;
	Node * get_maximum_() const;
	void transplant_( const Node * target, Node * to_transplant );
	std::string format_line_( const Node * ) const;

	InternIter ibegin();
	InternIter iend();
	InternIter irbegin();
	InternIter irend();
	CInternIter ibegin() const;
	CInternIter iend() const;
	CInternIter irbegin() const;
	CInternIter irend() const;

	Node * insert_( int key, const std::string& value );
	void insert_fixup_( Node * n );
	Node * get_grandparent_( Node * node );
	Node * get_uncle_( Node * node );
	void left_rotate_( Node * node );
	void right_rotate_( Node * node );

	static const Node * s_find_successor_in_right_branch_( const Node * node );
	static const Node * s_find_predecessor_in_left_branch_( const Node * node );
	static const Node * s_find_successor_( const Node * node );
	static const Node * s_find_predecessor_( const Node * node );
	static Node * s_find_successor_( Node * node );
	static Node * s_find_predecessor_( Node * node );

	std::string check_red_black_tree_property_4_() const;
	std::string check_red_black_tree_property_5_() const;
};

} // namespace EK
