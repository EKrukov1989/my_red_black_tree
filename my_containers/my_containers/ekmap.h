#pragma once
#include <string>
#include <map>
#include <memory>

namespace EK
{

struct MapNode
{
	MapNode * parent = nullptr;
	MapNode * left = nullptr;
	MapNode * right = nullptr;
	bool is_black = true; // true - black, false - red
	std::string value;
	int key = 0;

	MapNode() = default;

	MapNode( MapNode * parent, MapNode * left, MapNode * right,
			 int key, bool is_black, const std::string& value )
	{
		this->parent = parent;
		this->left = left;
		this->right = right;
		this->key = key;
		this->is_black = is_black;
		this->value = value;
	}
};

class Map
{
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
		explicit Iterator( MapNode * node );
		MapNode * node_ = nullptr;
	};

	class ConstIterator
	{
		friend class Map;
	public:
		ConstIterator& operator++();
		ConstIterator& operator--();
		bool operator==( ConstIterator other ) const;
		bool operator!=( ConstIterator other ) const;
		std::pair<int, const std::string&> operator*();
		std::unique_ptr<std::pair<int, const std::string&>> operator->();
	private:
		explicit ConstIterator( const MapNode * node );
		const MapNode * node_ = nullptr;
	};

public:
	Map();
	~Map();

	std::size_t count( int key ) const;
	std::string at( int key ) const;
	std::size_t size() const;

	void insert( int key, const std::string& value );
	void erase( int key );

	Iterator begin();
	Iterator end();
	Iterator rbegin();
	Iterator rend();
	ConstIterator begin() const;
	ConstIterator end() const;
	ConstIterator rbegin() const;
	ConstIterator rend() const;

	std::string get_debug_output();
	std::string check_rb_tree();

private:
	MapNode * root_ = nullptr;
	std::size_t counter = 0;

	MapNode * find_( int key ) const;
	MapNode * get_minimum_() const;
	MapNode * get_maximum_() const;
	void transplant_( const MapNode * target, MapNode * to_transplant );
	std::string format_line_( const MapNode * ) const;
};

} // namespace EK
