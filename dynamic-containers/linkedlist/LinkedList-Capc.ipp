#ifndef LINKEDLIST_CAPC_IPP
#define LINKEDLIST_CAPC_IPP

#ifndef LINKEDLIST_HPP
#error "This file should not be included outside of the LinkedList header file."
#endif


template<typename T>
typename LinkedList<T>::size_type LinkedList<T>::size() const noexcept {
	return size_;
}


template<typename T>
bool LinkedList<T>::empty() const noexcept {
	return size_ == 0;
}


#endif // LINKEDLIST_CAPC_IPP