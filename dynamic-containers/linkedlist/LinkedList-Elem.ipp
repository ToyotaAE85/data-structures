#ifndef LINKEDLIST_ELEM_IPP
#define LINKEDLIST_ELEM_IPP

#ifndef LINKEDLIST_HPP
#error "This file should not be included outside of the LinkedList header file."
#endif


template<typename T>
typename LinkedList<T>::reference LinkedList<T>::front() {
	return front_->data_;
}


template<typename T>
typename LinkedList<T>::const_reference LinkedList<T>::front() const {
	return front_->data_;
}


template<typename T>
typename LinkedList<T>::reference LinkedList<T>::back() {
	return back_->data_;
}


template<typename T>
typename LinkedList<T>::const_reference LinkedList<T>::back() const {
	return back_->data_;
}


#endif // LINKEDLIST_ELEM_IPP