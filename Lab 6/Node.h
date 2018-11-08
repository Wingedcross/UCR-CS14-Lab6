#pragma once
#include <string>

using std::string;

class Node {
private:

	friend class AVLTree;

	Node(const string & key);

	string data;
	int height;

	Node *parent;
	Node *left;
	Node *right;
};