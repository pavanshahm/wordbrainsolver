//builds trie with all the words in the dictionary
#include <string>
#include <vector>
#include <fstream>
#include <queue>

class dict_trie {
	struct Node {
		char elt;
		Node *parent;
		Node *sibling;
		Node *child;
	};

	Node *root = 0;

	//return depth of node
	int node_depth(Node *node) {
		if (!node->parent) {
			return 1;
		}
		else {
			return 1 + node_depth(node->parent);
		}
	}

	//inserts word into trie
	void insert_word(const std::string &word) {
		//for every letter in word
		Node *at = root;
		Node *par = root;

		for (unsigned int i = 0; i != word.size() + 1; ++i) {
			at = find(at, at, par, word[i]);
			par = at;
			at = at->child;
		}

	}

	Node* find(Node *prev, Node *at, Node *par, char val) {
		if (!at) {
			Node *temp = new Node{ val, par, 0, 0 };

			if (prev == at) {
				//empty 
				if (par) {
					par->child = temp;
				}
			}
			else {
				prev->sibling = temp;
			}

			at = temp;

			if (!root) {
				root = at;
			}

			return at;

		}
		else {
			if (at->elt == val) {
				return at;
			}
			else {
				return find(at, at->sibling, par, val);
			}
		}
	}

public:
	//init trie
	dict_trie(std::string filename) {
		std::fstream ins;
		ins.open(filename + ".txt");
		std::string word;
		while (getline(ins, word)) {
			insert_word(word);
		}

		ins.close();
	}

	//delete trie
	~dict_trie() {
		std::queue<Node*> to_delete;
		if (root) {
			to_delete.push(root);
		}

		while (!to_delete.empty()) {
			Node *temp = to_delete.front();
			to_delete.pop();

			if (temp->sibling) {
				to_delete.push(temp->sibling);
			}

			if (temp->child) {
				to_delete.push(temp->child);
			}

			delete temp;
		}
	}

	//returns true if a word is promising candidate
	bool is_candidate(std::string key, const int length) {
		Node *t = root;

		for (unsigned int i = 0; i != key.length(); ++i) {
			while (t && t->elt != key[i]) {
				t = t->sibling;
			}

			if (!t) {
				return false;
			}

			t = t->child;
		}

		return true;

		//if location is valid, push all possible words into vector. 
		//do a depth-first searching
		/*
		std::string to_add;
		to_add.push_back(t->elt);
		poss_strings(key, words, to_add, t, length);
		*/
	}

	bool is_word(std::string key) {
		Node *t = root;
		for (unsigned int i = 0; i != key.length(); ++i) {
			while (t && t->elt != key[i]) {
				t = t->sibling;
			}

			if (!t) {
				return false;
			}

			t = t->child;
		}
		
		while (t) {
			if (t->elt == '\0') {
				return true;
			}
			else {
				t = t->sibling;
			}
		}

		return false;
	}

	//modifies and adds all possible words that derive from loc
	void poss_strings(const std::string &key, std::vector<std::string> &words, std::string &to_add, Node* loc) {
		//base case, we've reached a terminating character
		if (loc->elt == '\0') {
			std::string temp_word = key + to_add;
			words.push_back(temp_word);
		}
		else {
			if (loc->child) {
				to_add.push_back(loc->child->elt);
				poss_strings(key, words, to_add, loc->child);
				to_add.pop_back();
			}

			if (loc->sibling) {
				char temp = to_add.back();
				to_add.pop_back();
				to_add.push_back(loc->sibling->elt);
				poss_strings(key, words, to_add, loc->sibling);
				to_add.pop_back();
				to_add.push_back(temp);
			}
		}
	}
	

};
