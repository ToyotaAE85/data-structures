#ifndef LINKEDLIST_ITER_IPP
#define LINKEDLIST_ITER_IPP

#ifndef LINKEDLIST_HPP
#error "This file should not be included outside of the LinkedList header file."
#endif


template<typename T>
template<bool isConst>
class LinkedList<T>::iterator_ {

	using node_ptr = std::conditional_t<isConst, const Node*, Node*>;


	friend class iterator_<false>; // conversion from const iterator -> iterator
	node_ptr current_;

public:

	// --- Container tags

	using iterator_category = std::bidirectional_iterator_tag;
	using value_type = typename LinkedList<T>::value_type;

	using pointer = std::conditional_t<isConst, typename LinkedList<T>::const_pointer, typename LinkedList<T>::pointer>;
	using reference = std::conditional_t<isConst, typename LinkedList<T>::const_reference, typename LinkedList<T>::reference>;


	iterator_() noexcept : current_{ nullptr } {} // end()

	explicit iterator_(node_ptr input_node) noexcept : current_{ input_node } {}


	template<bool B = isConst, typename = std::enable_if_t<B>>
	iterator_(const iterator_<false>& it) noexcept : current_{ it.current_ } {}


	reference operator*() const {

		return current_->data_;
	}

	pointer operator->() const {

		return &(current_->data_);
	}


	iterator_& operator++() {

		current_ = current_->next_.get();
		return *this;
	}

	iterator_ operator++(int) {

		iterator_ temp(*this);
		++(*this);
		return temp;
	}


	iterator_& operator--() {

		current_ = current_->prev_;
		return *this;
	}


	iterator_ operator--(int) {

		iterator_ temp(*this);
		--(*this);
		return temp;
	}


	bool operator==(const iterator_& rhs) const noexcept {

		return (current_ == rhs.current_);
	}

	bool operator!=(const iterator_& rhs) const noexcept {

		return !(*this == rhs);
	}
}; // class<bool> iterator_


// --- Iterators


template<typename T>
typename LinkedList<T>::iterator LinkedList<T>::begin() {

	return typename LinkedList<T>::iterator(front_.get());
}


template<typename T>
typename LinkedList<T>::iterator LinkedList<T>::end() {

	return typename LinkedList<T>::iterator();
}


template<typename T>
typename LinkedList<T>::const_iterator LinkedList<T>::begin() const {

	return typename LinkedList<T>::const_iterator(front_.get());
}


template<typename T>
typename LinkedList<T>::const_iterator LinkedList<T>::end() const {

	return typename LinkedList<T>::const_iterator();
}


template<typename T>
typename LinkedList<T>::const_iterator LinkedList<T>::cbegin() const {

	return typename LinkedList<T>::const_iterator(front_.get());
}


template<typename T>
typename LinkedList<T>::const_iterator LinkedList<T>::cend() const {

	return typename LinkedList<T>::const_iterator();
}


#endif // LINKEDLIST_ITER_IPP