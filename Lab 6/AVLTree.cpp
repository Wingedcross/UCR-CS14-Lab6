#include "AVLTree.h"
#include "Node.h"
#include <fstream>
#include <iostream>

using namespace std;


AVLTree::AVLTree() {
	root = nullptr;
}


void AVLTree::rebalance(const string & key, Node *node) {

	// Update heights
	updateHeight(node);

	// Determine if any unbalanced nodes were caused. Returns nullptr if none were found.
	// Returns the last unbalanced node if any was found.
	Node * unbalancedNode = findUnbalancedNode(key);

	// If an unbalanced node was found
	if (unbalancedNode != nullptr) {

		if (balanceFactor(unbalancedNode) == -2) {
			if (balanceFactor(unbalancedNode->right) == 1) {

				// Double rotation right-left case
				rotateRight(unbalancedNode->right);
			}

			rotateLeft(unbalancedNode);
		}
		else if (balanceFactor(unbalancedNode) == 2) {

			if (balanceFactor(unbalancedNode->left) == -1) {

				// Double rotation left-right case
				rotateLeft(unbalancedNode->left);
			}

			rotateRight(unbalancedNode);
		}
	}

}


//Public.
void AVLTree::insert(const string & key) {

	// If node doesn't exist, create one.
	if (root == nullptr) {
		root = new Node(key);
	}
	else {
		insert(key, root);
	}
}


//Private. Allows the tree to travel down the leaves
void AVLTree::insert(const string & key, Node *node) {

	// If the value is smaller than or equal to our current node's data
	if (key < node->data) {

		// If a left node exists, call the same function on that node.
		if (node->left != nullptr) {
			insert(key, node->left);
		}
		else {
			// Make new node
			node->left = new Node(key);

			// Update parent
			node->left->parent = node;

			// rebalance our tree
			rebalance(key, node->left);
		}
	}

	// If key is bigger than node's data..
	else if (key > node->data) {

		// If a right node exists, call the same function on that node.
		if (node->right != nullptr) {
			insert(key, node->right);
		}
		else {
			// Make new node
			node->right = new Node(key);

			// Update parent
			node->right->parent = node;

			// rebalance our tree
			rebalance(key, node->right);
		}
	}
}


// Recursive - TODO change to nonrecursive
void AVLTree::updateHeight(Node *node) {

	int leftHeight = -1;
	int rightHeight = -1;

	int max = 0;

	if (node->left != nullptr) {
		leftHeight = node->left->height;
	}

	if (node->right != nullptr) {
		rightHeight = node->right->height;
	}

	// If left is bigger, then left. Otherwise, right.
	max = leftHeight > rightHeight ? leftHeight : rightHeight;

	// Increase node's height by 1.
	node->height = max + 1;

	// As long as the node's parent is not null, update the ancestor's heights
	if (node->parent != nullptr) {
		updateHeight(node->parent);
	}
}


int AVLTree::balanceFactor(Node *node) {

	// Assume our node has no children until we evaluate it.
	int leftHeight = -1;
	int rightHeight = -1;

	if (node->left != nullptr) {
		leftHeight = node->left->height;
	}

	if (node->right != nullptr) {
		rightHeight = node->right->height;
	}

	// In the case where our node has no children, we simply return 0
	return leftHeight - rightHeight;
}


void AVLTree::printBalanceFactors() {
	if (root != nullptr) {
		printBalanceFactors(root);
	}

	cout << endl;
}


void AVLTree::printBalanceFactors(Node *node) {
	if (node != nullptr) {
		printBalanceFactors(node->left);

		cout << node->data << "(" << balanceFactor(node) << "), ";

		printBalanceFactors(node->right);
	}
}


Node * AVLTree::findUnbalancedNode(const string & key) {

	// If an unbalanced node was found, we change this. Otherwise, we return nullptr.
	Node *lastUnbalancedNode = nullptr;

	// We start the search at root
	Node *curr = root;

	// Start the search for our key.
	// Overwrite lastUnbalancedNode every time we encounter an unbalanced node.
	while (curr->data.compare(key) != 0) {

		// Find the balance factor of the current node.
		int bf = balanceFactor(curr);

		// Keep a reference to our current node's data
		string currData = curr->data;

		// If the balance factor is NOT -1, 0, or 1, then this node is unbalanced. Store it.
		if (bf > 1 || bf < -1) {
			lastUnbalancedNode = curr;
		}

		if (currData.compare(key) < 0) {
			curr = curr->right;
		}
		else if (currData.compare(key) > 0) {
			curr = curr->left;
		}
	}

	return lastUnbalancedNode;
}


void AVLTree::setChild(Node *parent, const string & whichChild, Node *child) {

	if (whichChild != "left" && whichChild != "right") {
		return;
	}

	if (whichChild == "left") {
		parent->left = child;
	}
	else {
		parent->right = child;
	}

	if (child != nullptr) {
		child->parent = parent;
	}

	updateHeight(parent);
}


void AVLTree::replaceChild(Node *parent, Node *currentChild, Node *newChild) {
	if (parent->left == currentChild) {
		setChild(parent, "left", newChild);
	}
	else if (parent->right == currentChild) {
		setChild(parent, "right", newChild);
	}
}


void AVLTree::rotateLeft(Node *node) {

	// ASK: Why not nullptr?
	Node *rightLeftChild = node->right->left;

	if (node->parent != nullptr) {
		replaceChild(node->parent, node, node->right);
	}
	else { // node is root
		root = node->right;
		root->parent = nullptr;
	}

	setChild(node->right, "left", node);
	setChild(node, "right", rightLeftChild);
}


void AVLTree::rotateRight(Node *node) {

	// ASK: Why not nullptr?
	Node *leftRightChild = node->left->right;

	if (node->parent != nullptr) {
		replaceChild(node->parent, node, node->left);
	}
	else { // node is root
		root = node->left;
		root->parent = nullptr;
	}

	setChild(node->left, "right", node);
	setChild(node, "left", leftRightChild);
}


void AVLTree::visualizeTree(const string &outputFilename) {
	ofstream outFS(outputFilename.c_str());
	if (!outFS.is_open()) {
		cout << "Error" << endl;
		return;
	}
	outFS << "digraph G {" << endl;
	visualizeTree(outFS, root);
	outFS << "}";
	outFS.close();
	string jpgFilename = outputFilename.substr(0, outputFilename.size() - 4) + ".jpg";
	string command = "dot -Tjpg " + outputFilename + " -o " + jpgFilename;
	system(command.c_str());
}


void AVLTree::visualizeTree(ofstream & outFS, Node *n) {
	if (n) {
		if (n->left) {
			visualizeTree(outFS, n->left);
			outFS << n->data << " -> " << n->left->data << ";" << endl;
		}


		if (n->right) {
			visualizeTree(outFS, n->right);
			outFS << n->data << " -> " << n->right->data << ";" << endl;
		}
	}
}
