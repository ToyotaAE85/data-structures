#include "Trie.hpp"

#include <array>
#include <memory>
#include <string_view>

#include <vector>
#include <string>


static std::size_t toIndex(const char ch) {
	return static_cast<std::size_t>(ch - 'a');
}


struct Trie::Node {

	bool final_;
	std::array<std::unique_ptr<Node>, 26> children_;


	Node() : final_{ false }, children_{ } {}
	~Node() noexcept = default;
};


// --- special member functions

Trie::Trie() noexcept : root_{ std::make_unique<Node>() } {}

Trie::~Trie() noexcept = default;



// --- class modifiers operations


bool Trie::insert(std::string_view input) {

	Node* current{ root_.get() };

	for (const char ch : input) {
		size_t index{ static_cast<std::size_t>(ch - 'a') };

		if (!current->children_[index]) {
			current->children_[index] = std::make_unique<Node>();
		}

		current = current->children_[index].get();
	}

	if (current->final_) {
		return false;
	}

	current->final_ = true;

	return true;
}


/* we remove the "word" only if it exists, as in, it's flagged as such.
 * nodes are not physically removed to avoid breaking shared prefixes, which means memory is not really freed.
 * for now, we leave the data hanging for academic purposes. (also i am lazy)
 */

bool Trie::remove(std::string_view input) {

	Node* current{ findNode_(input) };

	if (!current->final_) {
		return false;
	}

	current->final_ = false;

	return true;
}

std::vector<std::string> Trie::collectWords(std::string_view prefix) {

	Node* current{ findNode_(prefix) };
	if (!current) { return { }; }


	std::vector<std::string> list{ };
	std::string current_word{ prefix };

	collectWordsDfs_(current_word, current, list);

	return list;
}


void Trie::clear() noexcept {
	root_.reset();
}


// --- class access operations

bool Trie::startsWith(std::string_view input) const noexcept {
	return findNode_(input);
}


bool Trie::contains(std::string_view input) const noexcept {
	const Node* const dummy{ findNode_(input) };

	return dummy && dummy->final_;
}



// --- helper functions

const Trie::Node* Trie::findNode_(std::string_view input) const {

	const Node* current{ root_.get() };

	for (const char ch : input) {
		size_t index{ toIndex(ch) };

		if (!current->children_[index]) {
			return nullptr;
		}

		current = current->children_[index].get();
	}

	return current;
}


Trie::Node* Trie::findNode_(std::string_view input) {

	Node* current{ root_.get() };

	for (const char ch : input) {
		size_t index{ toIndex(ch) };

		if (!current->children_[index]) {
			return nullptr;
		}

		current = current->children_[index].get();
	}

	return current;
}


// pray we don't go too deep in the recursion, otherwise we might exhaust the stack

void Trie::collectWordsDfs_(std::string& current, Node* node, std::vector<std::string>& list) {
	if (!node) { return; }

	if (node->final_) {
		list.push_back(current);
	}

	for (auto i{ 0 }; i < 26; ++i) {
		if (!node->children_[i]) { continue; }

		char next_char{ 'a' + static_cast<char>(i) };


		current.push_back(next_char);
		
		collectWordsDfs_(current, node->children_[i].get(), list);

		current.pop_back();
	}
}