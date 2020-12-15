#include "BinomialPrice.h"
#include <math.h>
#include <iostream>

using namespace std;

BinomialPrice::BinomialPrice(Options* pOpt, int nSteps,
	PathFunc pathFunc, PayOffFunc payF)
	: option(pOpt), steps(nSteps), pathF(pathFunc), payOffF(payF)
{
	tree = new Tree();
	dt = option->maturity / nSteps;
	u = exp(option->vol * sqrt(dt));
	d = 1 / u;
	pu = (exp(option->rate * dt) - d) / (u - d);
	pd = 1.0 - pu;
	discount = exp(-option->rate * dt);
};

void
BinomialPrice::buildTree(TreeNode* t, int nFromLeafNode)
{
	// TO DO:
	// Use recursion to build a binary tree for evaluating the exotic options
	/*        Pseudo Code for buildTree
	If tree is empty (t==0)
	Create root node
	Fill in data for root node
	EndIf
	If height of tree > 0 (nFromLeafNode > 0)
	Create leftChild node
	Fill in data for leftChild node
	build left sub-tree (Call buildTree recursively)
	Create rightChild node
	Fill in data for rightChild node
	build right sub-tree (Call buildTree recursively)
	EndIf
	*/
	if (!t)
	{
		tree->root = new TreeNode(option->spotPrice);
		t = tree->root;
		t->level = 0;
		t->pathData = option->spotPrice;
	}
	if(nFromLeafNode > 0)
	{
		t->leftChild = new TreeNode(t->sharePrice * d);
		t->leftChild->parent = t;
		t->leftChild->level = t->level + 1;
		static_cast<TreeNode*>(t->leftChild)->pathData = pathF(t->pathData, t->leftChild->level, static_cast<TreeNode*>(t->leftChild)->sharePrice);
		buildTree(static_cast<TreeNode*>(t->leftChild), nFromLeafNode - 1);
		t->rightChild = new TreeNode(t->sharePrice * u);
		t->rightChild->parent = t;
		t->rightChild->level = t->level + 1;
		static_cast<TreeNode*>(t->rightChild)->pathData = pathF(t->pathData, t->rightChild->level, static_cast<TreeNode*>(t->rightChild)->sharePrice);
		buildTree(static_cast<TreeNode*>(t->rightChild), nFromLeafNode - 1);
	}
}

void
BinomialPrice::backwardEval(TreeNode* t)
{
	// TO DO:
	// Use recursion to backward evaluate the binary tree to obtain the exotic options values
	if (t)
	{
		if (!(t->leftChild))
		{
			t->optionPrice = payOffF(t->pathData, t->sharePrice, option->strikePrice, option->c_p);
		}
		else
		{
			if (option->e_a == American)
			{
				t->optionPrice = MAX(payOffF(t->pathData, t->sharePrice, option->strikePrice, option->c_p), (pd * static_cast<TreeNode*>(t->leftChild)->optionPrice + pu * static_cast<TreeNode*>(t->rightChild)->optionPrice) * discount);
			}
			else if (option->e_a == European)
			{
				t->optionPrice = (pd * static_cast<TreeNode*>(t->leftChild)->optionPrice + pu * static_cast<TreeNode*>(t->rightChild)->optionPrice) * discount;
			}
			backwardEval(static_cast<TreeNode*>(t->parent));
		}
	}
}


void BinomialPrice::printTree(TreeNode* node) 
{
	// TO DO:
	// Use recursion to print the tree. Output format given in SampleOutput.docx

}