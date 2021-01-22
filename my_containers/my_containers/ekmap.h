#pragma once
#include <string>
#include <vector>
#include <memory>

namespace EK
{

class Map
{
private:
	struct Node;

	class InternIter
	{
		// Internal interator provides access to nodes directly.
		// It's required for a few private methods. As well it is a basis for external iterators.
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

	class CInternIter
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

public:
	class Iterator
	{
		friend class Map;
	public:
		Iterator& operator++();
		Iterator& operator--();
		bool operator==( Iterator other ) const;
		bool operator!=( Iterator other ) const;
		std::pair<int, std::string&> operator*();
		std::unique_ptr<std::pair<int, std::string&>> operator->();
	private:
		InternIter iter_;
		explicit Iterator( InternIter intern_iter );
	};

	class CIterator
	{
		friend class Map;
	public:
		CIterator& operator++();
		CIterator& operator--();
		bool operator==( CIterator other ) const;
		bool operator!=( CIterator other ) const;
		std::pair<int, const std::string&> operator*();
		std::unique_ptr<std::pair<int, const std::string&>> operator->();
	private:
		CInternIter iter_;
		explicit CIterator( CInternIter iter );
	};

	Iterator begin();
	Iterator end();
	Iterator rbegin();
	Iterator rend();
	CIterator begin() const;
	CIterator end() const;
	CIterator rbegin() const;
	CIterator rend() const;

	Map();
	Map( const std::vector<std::pair<int, std::string>>& );
	Map( const std::initializer_list<std::pair<int, std::string>>& );
	Map( Map& rhs );
	Map& operator=( const Map& rhs );
	Map( Map&& rhs ) noexcept; 
	Map& operator=( Map&& rhs ) noexcept;
	~Map();

	std::size_t count( int key ) const;
	std::string at( int key ) const;
	std::size_t size() const;

	void insert( int key, const std::string& value );
	void insert( int key, std::string&& value );
	void insert( const std::pair<int, std::string>& key_value_pair );
	void insert( std::pair<int, std::string>&& key_value_pair );
	void erase( int key );

	Iterator find( int key );
	CIterator find( int key ) const;

	std::string get_debug_output() const;
	std::string check_red_black_tree_properties() const;
	unsigned get_black_height() const;

private:
	template<typename ValueType>
	void t_insert_( int key, ValueType&& value);

	Node * root_ = nullptr;
	std::size_t counter_ = 0;

	InternIter ibegin_();
	InternIter iend_();
	InternIter irbegin_();
	InternIter irend_();
	CInternIter ibegin_() const;
	CInternIter iend_() const;
	CInternIter irbegin_() const;
	CInternIter irend_() const;

	Node * find_( int key ) const;
	Node * get_minimum_() const;
	Node * get_maximum_() const;

	void left_rotate_( Node * node );
	void right_rotate_( Node * node );
	void swap_( Node * a, Node * b );
	void insert_fixup_( Node * n );
	void remove_node_without_childs_( Node * node );
	void erase_one_child_node_( Node * node );
	void erase_fixup_( Node * node );
	std::string check_red_black_tree_property_4_() const;
	std::string check_red_black_tree_property_5_() const;

	template<typename ValueType>
	Node * t_insert_node_( int key, ValueType&& value );

	static Node * s_copy_tree_( Node * n );
	static Node * s_get_grandparent_( Node * node );
	static Node * s_get_sibling_( Node * node );
	static Node * s_get_uncle_( Node * node );
	static Node * s_get_minimum_( Node * );
	static const Node * s_get_minimum_( const Node * );
	static Node * s_get_maximum_( Node * );
	static const Node * s_get_maximum_( const Node * );
	static Node * s_find_successor_( Node * node );
	static const Node * s_find_successor_( const Node * node );
	static Node * s_find_predecessor_( Node * node );
	static const Node * s_find_predecessor_( const Node * node );
	static std::string s_format_line_( const Node * );
};

} // namespace EK

