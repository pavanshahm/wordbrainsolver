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
	int node_depth(Node *node);

	//inserts word into trie
	void insert_word(const std::string &word);

	//find and return the node that contains val
	Node* find(Node *prev, Node *at, Node *par, char val);

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

	//returns true if the key exists exists in the tree
	//eg. pag returns true
	bool is_candidate(std::string key, const int length);

	//returns true if the key is a word in the tree
	//eg. pag returns false, pagan returns true
	bool is_word(std::string key);

	//modifies and adds all possible strings to words that branch from loc
	void poss_strings(const std::string &key, std::vector<std::string> &words, std::string &to_add, Node* loc);

};
