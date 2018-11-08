#pragma once
#ifndef __AVLTREE_H
#define __AVLTREE_H

#include "Node.h"
#include <fstream>
#include <string>

using std::string;
using std::ofstream;

// Class declaration for EVLTree
class AVLTree {
private:
	Node * root;
	Node * findUnbalancedNode(const string &);

	void insert(const string &, Node *);
	void visualizeTree(ofstream &, Node *);
	void updateHeight(Node *);
	void printBalanceFactors(Node *);
	void setChild(Node *, const string &, Node *);
	void replaceChild(Node *, Node *, Node *);
	void rotateLeft(Node *);
	void rotateRight(Node *);
	void rebalance(const string &, Node *);

public:
	AVLTree();

	int balanceFactor(Node*);

	void insert(const string &);
	void printBalanceFactors();
	void visualizeTree(const string &);
};

#endif