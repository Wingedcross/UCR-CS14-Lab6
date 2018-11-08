#include "Node.h"

Node::Node(const string & key) {
	parent = left = right = nullptr;
	data = key;
	height = 0;
}