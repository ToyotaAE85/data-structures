#ifndef BST_HPP
#define BST_HPP

// todo: properly implement copy/assignment operator(copy) constructors. <- must be a hard-copy, later.. maybe
//		 implement a sentinel node (?????) so that doing --end() doesn't kill the system

#include <memory>		// unique_ptr
#include <queue>		// needed for level traversal (bfs)
#include <stack>		// needed for iterator class/traversals (inorder, postorder, preorder).
#include <utility>		// std::move
#include <cstddef>		// std::size_t
#include <type_traits>	// nothrow, conditional, enableif, etc

#include <initializer_list>

/* a few notes of this implementation:
 * it doesn't self balance, which means its worst time complexity could be O(n) on all operations, 
 * which kinda defeats the entire purpose of a binary tree... but it's ok dw about it
 * we assume T is a primitive type OR has overloaded valid comparison operators, otherwise u get a mean compiler error
 * doing anything with iterators end() on the tree is dangerous, don't do it mate
 * also calling min/max on an empty tree will upset the OS tremendously, don't do it
 * and as always, beware of iterator invalidation because there are no checks against that here
 */

template <typename T>
class BinarySearchTree {

	struct Node {
		// what a relief, not using manual memory allocations... phew...

		std::unique_ptr<Node> left_;
		std::unique_ptr<Node> right_;
		T data_;

		Node(T data_in) noexcept : data_{ data_in }, left_{ nullptr }, right_{ nullptr } {}
	};

	template <bool isConst>
	class iterator_;

	// --- HELPER TRAVERSAL FUNCTIONS DATA

	enum class Traversal {
		Inorder,
		Preorder,
		Postorder
	};

	template<typename Function>
	void traversal_(Function& obj,Traversal type, const Node* const input) const {
		if (input == nullptr) { return; }


		if (type == Traversal::Preorder) { obj(input->data_); }


		traversal_(obj, type, input->left_.get());


		if (type == Traversal::Inorder) { obj(input->data_); }


		traversal_(obj, type, input->right_.get());


		if (type == Traversal::Postorder) { obj(input->data_); }

	}

	std::unique_ptr<Node> root_;
	std::size_t size_;

public:

	using value_type = T;
	
	using reference			= T&;
	using const_reference	= const T&;
	using iterator			= iterator_<false>;
	using const_iterator	= iterator_<true>;


	// --- SPECIAL MEMBER FUNCTIONS.

	BinarySearchTree() noexcept : root_{ nullptr }, size_{ 0 } {}

	BinarySearchTree(const std::initializer_list<T>& init) noexcept : root_{ nullptr }, size_{ 0 } {
		for (const T& in : init) {
			insert(in);
		}
	}

	BinarySearchTree(BinarySearchTree&& rhs) noexcept {
		root_ = std::move(rhs.root_);
		size_ = rhs.size_;

		rhs.size_ = 0;
	}

	BinarySearchTree& operator=(BinarySearchTree&& rhs) noexcept {
		if (&rhs != this) {
			root_ = std::move(rhs.root_);
			size_ = rhs.size_;

			rhs.size_ = 0;
		}

		return *this;
	}

	BinarySearchTree(const BinarySearchTree& rhs) = delete;
	BinarySearchTree& operator=(const BinarySearchTree& rhs) = delete;
	
	~BinarySearchTree() noexcept = default;


	// --- MODIFIERS

	bool insert(const T& data_in) noexcept(std::is_nothrow_default_constructible_v<T>)  {
		// make_unique might throw bad_alloc if memory is insufficient, at which point
		// we might as well call std::terminate ourselves. BUT what if T() throws instead...
		// making it noexcept is bad, unless we use traits....

		if (root_ == nullptr) {
			root_ = std::make_unique<Node>(data_in);
		}
		else {
			Node* it_ptr{ root_.get() };

			while (1) {
				if (it_ptr->data_ == data_in) {
					return false;
				}

				if (it_ptr->data_ > data_in) {
					if (it_ptr->left_ == nullptr) {
						it_ptr->left_ = std::make_unique<Node>(data_in);
						break;
					}
					it_ptr = it_ptr->left_.get();
				}
				else {
					if (it_ptr->right_ == nullptr) {
						it_ptr->right_ = std::make_unique<Node>(data_in);
						break;
					}
					it_ptr = it_ptr->right_.get();
				}
			}
		}

		++size_;
		return true;
	} // insert()


	bool contains(const T& data_in) const {
		const Node* it{ root_.get() };
		
		while (it) {
			if (it->data_ == data_in) {
				return true;
			}

			if (it->data_ > data_in) {
				it = it->left_.get();
			}
			else {
				it = it->right_.get();
			}
		}

		return false;
	} // contains()


	void clear() {
		if (root_ == nullptr) { return; }

		root_.reset();
		size_ = 0;
	} // clear()


	bool erase(const T& data_in) {
		if (root_ == nullptr) { return false; }

		std::unique_ptr<Node>* cur{ &root_ }; // woah

		while (*cur) {
			if ((*cur)->data_ == data_in) {

				// we check if we have one child first.

				if ((*cur)->left_ == nullptr) {
					*cur = std::move((*cur)->right_);
				}
				else if ((*cur)->right_ == nullptr) {
					*cur = std::move((*cur)->left_);
				}
				else { 
					// two child sceneario, move to first in-order successor.
					std::unique_ptr<Node>* in_suc{ &(*cur)->right_ }; //dear god

					while ((*in_suc)->left_) {
						in_suc = &(*in_suc)->left_;
					}

					// in case T is massive (also assuming its a safe operation lmao)
					std::swap((*cur)->data_, (*in_suc)->data_);

					*in_suc = std::move((*in_suc)->right_);
				}

				--size_;
				return true;
			} // couldn't find our requested node.
			else if ((*cur)->data_ > data_in) {
				cur = &(*cur)->left_;
			}
			else {
				cur = &(*cur)->right_;
			}
		}

		return false;
	} // erase()


	// ---- TRAVERSALS

	template<typename Function>
	void inorder(Function obj) const {
		traversal_(obj, Traversal::Inorder, root_.get());
	}


	template<typename Function>
	void postorder(Function obj) const {
		traversal_(obj, Traversal::Postorder, root_.get());
	}


	template<typename Function>
	void preorder(Function obj) const {
		traversal_(obj, Traversal::Preorder, root_.get());
	}


	template<typename Function>
	void bfs(Function obj) const {
		if (root_ == nullptr) { return; }

		std::queue<Node*> que{ };
		que.push(root_.get());
		std::size_t level{ 0 };

		while (!que.empty()) {

			const std::size_t cur_size{ que.size() };

			for (std::size_t i{ 0 }; i < cur_size; ++i) {
				Node* top{ que.front() };

				obj(top->data_);

				if (top->left_) {
					que.push(top->left_.get());
				}

				if (top->right_) {
					que.push(top->right_.get());
				}

				que.pop();
			}

		}
	} // bfs()


	// --- CAPACITY

	std::size_t size() const noexcept { return size_; }

	bool empty() const noexcept { return size_ == 0; }


	const_reference min() const {
		const Node* out{ root_.get() };

		while (out->left_) {
			out = out->left_.get();
		}

		return out->data_;
	}


	const_reference max() const {
		const Node* out{ root_.get() };

		while (out->right_) {
			out = out->right_.get();
		}

		return out->data_;
	}


	// --- ITERATOR (i wonder if these work with the stl, maybe not but it works for range loop so it ok)

	iterator begin() {
		return iterator(root_.get());
	}

	iterator end() {
		return iterator();
	}

	const_iterator begin() const {
		return const_iterator(root_.get());
	}

	const_iterator end() const {
		return const_iterator();
	}
	
	
}; // BinarySearchTree


// written outside the class due to it being easier on my eyes.
template<typename T>
template<bool isConst>
class BinarySearchTree<T>::iterator_ { // unidirectional iterator. questionmark
	std::stack<Node*> st_;

	void fill(Node* root) {
		if (root == nullptr) { return; }

		while (root) {
			st_.push(root);
			root = root->left_.get();
		}
	}

public:

	using value_type = T;

	using pointer = std::conditional_t<isConst, const T*, T*>;
	using reference = std::conditional_t<isConst, const T&, T&>;

	iterator_() noexcept = default;
	iterator_(Node* root) noexcept : st_{ } { fill(root); }

	// allows us to copy from non-const to const, i think
	template<bool B = isConst, typename = std::enable_if_t<B>>
	iterator_(const iterator_<false>& rhs) : st_{ rhs.st_ } { }


	reference operator*() const {
		return st_.top()->data_;
	}

	pointer operator->() const {
		return &st_.top()->data_;
	}

	iterator_& operator++() {

		Node* temp{ st_.top() };
		st_.pop();

		fill(temp->right_.get());
		return *this;
	}

	iterator_ operator++(int) {
		iterator_ temp{ *this };
		++(*this);
		return temp;
	}


	bool operator==(const iterator& rhs) const {
		return st_ == rhs.st_;
	}

	bool operator!=(const iterator& rhs) const {
		return !(*this == rhs);
	}
};


#endif //BST_HPP