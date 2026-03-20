#ifndef LINKEDLIST_MODI_IPP
#define LINKEDLIST_MODI_IPP

#ifndef LINKEDLIST_HPP
#error "This file should not be included outside of the LinkedList header file."
#endif


// --- Class Modifiers (including helpers)

template<typename T>
void LinkedList<T>::push_back(typename LinkedList<T>::const_reference data_in) {

	std::unique_ptr<Node> new_node{ std::make_unique<Node>(data_in) };
	Node* raw{ new_node.get() };

	if (!front_) { // empty list

		front_ = std::move(new_node);
	}
	else {

		raw->prev_ = back_;
		back_->next_ = std::move(new_node);
	}

	back_ = raw;
	++size_;
}


template<typename T>
void LinkedList<T>::push_front(typename LinkedList<T>::const_reference data_in) {

	std::unique_ptr<Node> new_node{ std::make_unique<Node>(data_in) };

	if (!front_) { 

		back_ = new_node.get();
	}
	else {

		front_->prev_ = new_node.get();
		new_node->next_ = std::move(front_);
	}

	front_ = std::move(new_node);
	++size_;
}


template<typename T>
void LinkedList<T>::pop_back() {

	if (!back_) { return; }

	if (back_->prev_) {

		Node* prev{ back_->prev_ };
		prev->next_.reset();
		back_ = prev;

		--size_;
	}
	else {
		clear();
	}
}


template<typename T>
void LinkedList<T>::pop_front() {

	if (!front_) { return; }

	if (front_->next_) {

		front_ = std::move(front_->next_);
		front_->prev_ = nullptr;
		--size_;
	}
	else {
		clear();
	}
}


template<typename T>
void LinkedList<T>::resize_helper_(
	typename LinkedList<T>::size_type size_in,
	typename LinkedList<T>::const_reference data_in) {

	if (size_in == size_) { return; }

	if (size_in > size_) {
		while (size_in > size_) {
			push_back(data_in);
		}
	}
	else {
		while (size_in < size_) {
			pop_back();
		}
	}
}


template<typename T>
void LinkedList<T>::resize(typename LinkedList<T>::size_type size_in) {
	resize_helper_(size_in, T());
}


template<typename T>
void LinkedList<T>::resize(
	typename LinkedList<T>::size_type size_in,
	typename LinkedList<T>::const_reference data_in) {
	resize_helper_(size_in, data_in);
}


template<typename T>
void LinkedList<T>::clear() {
	front_.reset();
	back_ = nullptr;
	size_ = 0;
}


#endif // LINKEDLIST_MODI_IPP