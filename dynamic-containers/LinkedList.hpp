#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include <memory>			// unique_ptr
#include <cstddef>			// size_t
#include <initializer_list>


/* todo: add a sentinel node so that we have a true bidirectional iterator, 
 * though this proved a bit more difficult due to the way the struct Node is presented.
 * my current approach was to get rid of unique ptr altogether and manage memory manually,
 * so that I can manually adjust pointers without too much of a hassle.
 * other than that I'd say this is fine as is? maybe add erase(iter) once I get the ideas flowing. who knows
 */


/* a few things of this implementation (which is for academic purposes obviously):
 * performing any access functions on an empty list means horrible things, don't do it.
 * the implemented iterator is strictly bidirectional, in case you're like me and didn't know about these things:
 * it means it lacks random access functions, such as + or -, so no sorting, etc. (yet... hehe)
 */


template<typename T>
class LinkedList {

	template<bool isConst>
	class iterator_;


	struct Node {
		T data_;

		std::unique_ptr<Node> next_;
		Node* prev_;

		Node() noexcept : data_{ }, next_{ nullptr }, prev_{ nullptr } { }
		Node(T data_in) noexcept : Node() { data_ = data_in; }
	};


	std::size_t size_;

	std::unique_ptr<Node> front_;
	Node* back_;
	

public:

	// ---- ---- MEMBER TYPE TAGS.


	using value_type		= T;
	using size_type			= std::size_t;
	using reference			= T&;
	using const_reference	= const T&;
	using pointer			= T*;
	using const_pointer		= const T*;
	using iterator			= iterator_<false>;
	using const_iterator	= iterator_<true>;


	// ---- ---- SPECIAL MEMBERS (Spec)

	LinkedList();

	LinkedList(std::initializer_list<T> list);
	
	LinkedList(const LinkedList& rhs);
	LinkedList& operator=(const LinkedList& rhs);

	LinkedList(LinkedList&& rhs);
	LinkedList& operator=(LinkedList&& rhs);

	~LinkedList() = default;


	// ---- ---- ELEMENT ACCESS (Elem)

	reference front();
	const_reference front() const;

	reference back();
	const_reference back() const;


	// ---- ---- ITERATORS (Iter)

	iterator begin();
	iterator end();

	const_iterator begin() const;
	const_iterator end() const;

	const_iterator cbegin() const;
	const_iterator cend() const;


	// ---- ---- CAPACITY (Capc)

	size_type size() const noexcept;
	bool empty() const noexcept;


	// ---- ---- MODIFIERS (Modi)

	void push_back(const_reference data_in);
	void push_front(const_reference data_in);

	void pop_back();
	void pop_front();

	void resize(size_type size_in);
	void resize(size_type size_in, const_reference data_in);
	void clear();


private:

	void resize_helper_(size_type size_in, const_reference data_in);

};


// ---- ---- CLASS MEMBER DEFINITIONS.

#include "./LinkedList-Spec.ipp"

#include "./LinkedList-Elem.ipp"

#include "./LinkedList-Iter.ipp"

#include "./LinkedList-Capc.ipp"

#include "./LinkedList-Modi.ipp"


#endif // LINKEDLIST_HPP