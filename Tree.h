#ifndef TREE_H
#define TREE_H

class Node
{
public:
	Node*	leftChild;
	Node*	rightChild;
	Node*	parent;
	int		level;

	Node()
		: leftChild(0), rightChild(0), parent(0) {
	}
};

// TO DO:
// Specialize Node to a TreeNode class
// Each TreeNode contains the following data items:
// 1. Share Price
// 2. Path Data (e.g. running average of share price along the path of binary tree)
// 3. Option Price (intermediate value calculated during backward evaluation)
class TreeNode : public Node
{
public:
	TreeNode(double s) { sharePrice = s; }
	double sharePrice=0.0;
	double pathData=0.0;
	double optionPrice=0.0;
};


class Tree
{
public:
	TreeNode*	root;

	Tree() : root(0), nLevel(0) {};
	void setLevel(int n) { nLevel = n; }
	int getLevel() { return nLevel; }

private:
	int		nLevel;

};

#endif