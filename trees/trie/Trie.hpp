#ifndef TRIE_HPP
#define TRIE_HPP


// note, this implementation relies on the user using lowercase english chars. (a-z)
// failure to do so will result in the compiler being very upset at you.
// maybe i'll add support for UTF32 later on, so we can check for emojis

// todo: nothing, optimize memory usage maybe, current struct uses a whooping 216bytes per node, maybe later though


#include <memory>		// unique_ptr
#include <string_view>	// non-owning string

#include <vector>
#include <string>

class Trie {

	struct Node;

	std::unique_ptr<Node> root_;

public:

	Trie() noexcept;

	~Trie() noexcept;


	bool insert(std::string_view input);


	bool remove(std::string_view input);


	void clear() noexcept;


	std::vector<std::string> collectWords(std::string_view prefix);


	bool startsWith(std::string_view input) const noexcept;


	bool contains(std::string_view input) const noexcept;


private:

	// --- helper functions

	const Node* findNode_(std::string_view input) const;

	Node* findNode_(std::string_view input);

	void collectWordsDfs_(std::string& input, Node* node, std::vector<std::string>& list);
};




#endif // TRIE_HPP