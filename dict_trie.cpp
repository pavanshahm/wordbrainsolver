#include <string>
#include <vector>
#include <fstream>
#include <queue>

#include "dict_trie.h"

int dict_trie::node_depth(Node *node) {
	if (!node->parent) {
		return 1;
	}
	else {
		return 1 + node_depth(node->parent);
	}
}

void dict_trie::insert_word(const std::string &word) {
	//for every letter in word
	Node *at = root;
	Node *par = root;

	for (unsigned int i = 0; i != word.size() + 1; ++i) {
		at = find(at, at, par, word[i]);
		par = at;
		at = at->child;
	}

}

dict_trie::Node* dict_trie::find(Node *prev, Node *at, Node *par, char val) {
	if (!at) {
		//if at is not a valid node, build a node to take its place
		Node *temp = new Node{ val, par, 0, 0 };

		if (prev == at) {
			//if prev == at, then temp has no siblings
			//if parent exists, make temp parent's child
			if (par) {
				par->child = temp;
			}
		}
		else {
			//if prev is valid, then make temp prev's sibling
			prev->sibling = temp;
		}

		at = temp;

		//if root does not exit, make at the root
		if (!root) {
			root = at;
		}

		return at;

	}
	else {
		//if at is valid and contains val, return
		if (at->elt == val) {
			return at;
		}
		else {
			//check next node
			return find(at, at->sibling, par, val);
		}
	}
}

bool dict_trie::is_candidate(std::string key, const int length) {
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

bool dict_trie::is_word(std::string key) {
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

void dict_trie::poss_strings(const std::string &key, std::vector<std::string> &words, std::string &to_add, Node* loc) {
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