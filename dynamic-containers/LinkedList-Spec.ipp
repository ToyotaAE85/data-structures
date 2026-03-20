#ifndef LINKEDLIST_SPEC_IPP
#define LINKEDLIST_SPEC_IPP

#ifndef LINKEDLIST_HPP
#error "This file should not be included outside of the LinkedList header file."
#endif


// --- Constructors

template<typename T>
LinkedList<T>::LinkedList() : size_{ 0 }, front_{ nullptr }, back_{ nullptr } {}


template<typename T>
LinkedList<T>::LinkedList(std::initializer_list<T> list) : LinkedList() {

	for (const auto& item : list) {
		push_back(item);
	}
}


// --- Copy constructor & assignment operator
// --- Note, we rely on iterators for a deep copy.

template<typename T>
LinkedList<T>::LinkedList(const LinkedList<T>& rhs) : LinkedList() {

	for (const auto& item : rhs) {
		push_back(item);
	}
}


template<typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& rhs) {

	if (this != &rhs) {

		clear();

		for (const auto& item : rhs) {
			push_back(item);
		}
	}

	return *this;
}


// --- Move constructor & assignment operator

template<typename T>
LinkedList<T>::LinkedList(LinkedList<T>&& rhs) :
	front_{ std::move(rhs.front_) },
	back_{ rhs.back_ },
	size_{ rhs.size_ } {

	rhs.clear();
}


template<typename T>
LinkedList<T>& LinkedList<T>::operator=(LinkedList<T>&& rhs) {

	if (this != &rhs) {

		front_ = std::move(rhs.front_);
		back_ = rhs.back_;
		size_ = rhs.size_;

		rhs.clear();
	}

	return *this;
}


#endif // LINKEDLIST_SPEC_IPP