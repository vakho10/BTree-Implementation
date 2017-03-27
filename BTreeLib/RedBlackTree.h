#pragma once

#include "RedBlackTreeNode.h"

namespace BTreeLib
{
	template<typename T>
	class RedBlackTree
	{
		int size;
		RedBlackTreeNode<T>* nil;
		RedBlackTreeNode<T>* root;

		RedBlackTreeNode<T>* tree_minimum(RedBlackTreeNode<T>*);
		void rotateLeft(RedBlackTreeNode<T>* x);
		void rotateRight(RedBlackTreeNode<T>* x);
		void RedBlackTree<T>::destroyTree(RedBlackTreeNode<T>* node);
		void RBinsertFixup(RedBlackTreeNode<T>* z);
		void insertBST(RedBlackTreeNode<T>* z);
		void RBdeleteFixup(RedBlackTreeNode<T>* z);
		void transplant(RedBlackTreeNode<T>* u, RedBlackTreeNode<T>* v);
		void inorderWolk(RedBlackTreeNode<T>* x);
	public:
		RedBlackTree();
		~RedBlackTree();

		RedBlackTreeNode<T>* search(T k);
		void insert(RedBlackTreeNode<T>* z);
		void remove(RedBlackTreeNode<T>* z);

		int getSize() { return size; }
		RedBlackTreeNode<T>* getNil() { return nil; }
		RedBlackTreeNode<T>* getRoot() { return root; }
	};
}