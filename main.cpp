#include <vector>
#include <deque>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "dict_trie.h"

using std::vector;
using std::deque;
using std::string;
using std::iostream;

struct grid_node {
	char c;
	bool visited;
};

void read_grid(vector<vector<grid_node>> &problem);
bool is_valid(const int row, const int col, const int N, vector<vector<grid_node>> &grid);
void find_words(string curr, vector<vector<grid_node>> &grid, dict_trie &dictionary, vector<string> &solutions, int row, int col, deque<int> &length, int N);
void update_grid(vector<vector<grid_node>> &grid);


vector<vector<string>> true_solutions;


int main() {
	dict_trie dictionary("words");
	std::cout << "Welcome to wordbrain hints" << std::endl;
	std::cout << "Press q and then enter to quit; Press s and then enter to help solve: " << std::endl;

	char c;

	std::cin >> c;

	vector<vector<grid_node>> prev;

	while (c != 'q') {
		if (c != 's') {
			std::cout << "Error: Invalid Command" << std::endl;
		}

		int N;
		int size;

		true_solutions.clear();

		std::cout << "Please enter grid dimensions, enter -1 to use previous grid: ";
		std::cin >> N;
		
		while (N == -1) {
			if (prev.empty()) {
				std::cout << "No previous grid, please enter grid dimensions: ";
				std::cin >> N;
			}
			else {
				N = -2;
			}
		}

		if(N != -2) {
			vector<vector<grid_node>> grid(N, vector<grid_node>(N));

			std::cout << "Please enter all values on the grid, top-down, left to right, " << std::endl;
			std::cout << "Enter a * for a blank value:" << std::endl;

			read_grid(grid);

			prev = grid;
		}

		N = prev[0].size();

		deque<int> sizes;

		std::cout << "Please enter candidate word sizes: " << std::endl;
		std::cin >> size;
		sizes.push_back(size);

		while (std::cin.peek() != '\n') {
			std::cin >> size;
			sizes.push_back(size);
		}

		std::cout << "Solving problem....." << std::endl;
		vector<string> solutions;
		for (int i = 0; i != N; ++i) {
			for (int j = 0; j != N; ++j) {
				string a;
				a.push_back(prev[i][j].c);
				find_words(a, prev, dictionary, solutions, i, j, sizes, N);
			}
		}
		/*
		std::sort(solutions.begin(), solutions.end());
		auto it = std::unique(solutions.begin(), solutions.end());
		solutions.resize(std::distance(solutions.begin(), it));
		*/
		std::ofstream ins;
		ins.open("sol.txt");
		for (auto i : true_solutions) {
			for (auto j : i) {
				ins << j << std::endl;
			}
			ins << std::endl;
		}
		ins.close();
		std::cout << "Press q and then enter to quit; Press s and then enter to help solve: " << std::endl;
		std::cin >> c;
	}

	std::cout << "Done!";
	
	return 0;
}

void read_grid(vector<vector<grid_node>> &problem) {
	for (unsigned i = 0; i != problem[0].size(); ++i) {
		for (unsigned j = 0; j != problem[0].size(); ++j) {
			char c;
			std::cin >> c;
			if (c != '*') {
				problem[i][j] = { c, false };
			}
			else {
				problem[i][j] = { c, true };
			}
		}
	}
}

void find_words(string curr, vector<vector<grid_node>> &grid, dict_trie &dictionary, vector<string> &solutions, int row, int col, deque<int> &length, int N) {
	//if promising
	grid[row][col].visited = true;
	if (curr.length() <= length.front() && dictionary.is_candidate(curr, length.front())) {
		//if solution
		if (curr.length() == length.front()) {
			if (dictionary.is_word(curr)) {
				solutions.push_back(curr);
				//update grid find next solutions
				int word_length = length.front();
				length.pop_front();
				if (!length.empty()) {
					//update grid
					vector<vector<grid_node>> new_grid = grid;
					update_grid(new_grid);
					for (int i = 0; i != N; ++i) {
						for (int j = 0; j != N; ++j) {
							string a;
							a.push_back(new_grid[i][j].c);
							find_words(a, new_grid, dictionary, solutions, i, j, length, N);
						}
					}
					
				}
				else {
					true_solutions.push_back(solutions);
				}

				length.push_front(word_length);
				solutions.pop_back();
			}
		}
		else {
			if (is_valid(row - 1, col - 1, N, grid)) {
				find_words(curr + grid[row - 1][col - 1].c, grid, dictionary, solutions, row - 1, col - 1, length, N);
			}

			if (is_valid(row - 1, col, N, grid)) {
				find_words(curr + grid[row - 1][col].c, grid, dictionary, solutions, row - 1, col , length, N);
			}

			if (is_valid(row - 1, col + 1, N, grid)) {
				find_words(curr + grid[row - 1][col + 1].c, grid, dictionary, solutions, row - 1, col + 1, length, N);
			}


			if (is_valid(row, col - 1, N, grid)) {
				find_words(curr + grid[row][col - 1].c, grid, dictionary, solutions, row, col - 1, length, N);
			}
			if (is_valid(row, col + 1, N, grid)) {
				find_words(curr + grid[row][col + 1].c, grid, dictionary, solutions, row, col + 1, length, N);
			}


			if (is_valid(row + 1, col - 1, N, grid)) {
				find_words(curr + grid[row + 1][col - 1].c, grid, dictionary, solutions, row + 1, col - 1, length, N);
			}

			if (is_valid(row + 1, col, N, grid)) {
				find_words(curr + grid[row + 1][col].c, grid, dictionary, solutions, row + 1, col, length, N);
			}

			if (is_valid(row + 1, col + 1, N, grid)) {
				find_words(curr + grid[row + 1][col + 1].c, grid, dictionary, solutions, row + 1, col + 1, length, N);
			}
		}

	}

	grid[row][col].visited = false;
}

bool is_valid(const int row, const int col, const int N, vector<vector<grid_node>> &grid) {
	return row >= 0 && row < N && col >= 0 && col < N && (grid[row][col].visited == false);
}


//update grid after a solution is found O(2n) for now
void update_grid(vector<vector<grid_node>> &grid) {
	int size = grid[0].size();

	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			if (grid[i][j].visited) {
				grid[i][j].c = '*';
				grid[i][j].visited = false;
			}
		}
	}

	

	for (int i = 0; i < size - 1; ++i) {
		for (int j = 0; j < size; ++j) {
			if (grid[i + 1][j].c == '*') {
				int w = i + 1;
				while (w != 0) {
					grid[w][j].c = grid[w - 1][j].c;
					grid[w - 1][j].c = '*';
					--w;
				}
			}
		}
	}

	return;
}
