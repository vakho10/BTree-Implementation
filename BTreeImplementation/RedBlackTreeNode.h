#ifndef RED_BLACK_TREE_NODE_H
#define RED_BLACK_TREE_NODE_H

#pragma once

// This is a simple red-black tree's node which has no .cpp file.
template<typename T>
class RedBlackTreeNode
{
public:
	T key;
	RedBlackTreeNode* p;
	RedBlackTreeNode* left, *right;
	char color;

	RedBlackTreeNode() {};
	RedBlackTreeNode(T k) 
	{ 
		key = k; 
	};
};

#endif