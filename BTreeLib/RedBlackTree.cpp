#include "RedBlackTree.h"

namespace BTreeLib
{
	template<typename T>
	RedBlackTree<T>::RedBlackTree()
	{
		nil = new RedBlackTreeNode<T>;
		nil->color = 1;
		root = nil;
		size = 0;
	}
	template<typename T>
	RedBlackTree<T>::~RedBlackTree()
	{
		destroyTree(root);
		delete nil;
	}
	template<typename T>
	void RedBlackTree<T>::destroyTree(RedBlackTreeNode<T>* node) {
		if (node != nil) {
			destroyTree(node->right);
			destroyTree(node->left);
			delete node;
		}
	}
	template<typename T>
	void RedBlackTree<T>::insertBST(RedBlackTreeNode<T>* z)
	{
		size++;
		RedBlackTreeNode<T>* x(root);
		RedBlackTreeNode<T>* y(nil);
		while (nil != x)
		{
			y = x;
			if (z->key < x->key)
				x = x->left;
			else
				x = x->right;
		}
		z->p = y;
		if (y == nil)
			root = z;
		else
			if (z->key < y->key)
				y->left = z;
			else
				y->right = z;
		z->left = nil;
		z->right = nil;
		z->color = 0;
	}
	template<typename T>
	void RedBlackTree<T>::inorderWolk(RedBlackTreeNode<T>* x)
	{
		if (nil != x)
		{
			inorderWolk(x->left);
			cout << x->key << "\t";
			inorderWolk(x->right);
		}
	}
	template<typename T>
	RedBlackTreeNode<T>* RedBlackTree<T>::search(T k)
	{
		RedBlackTreeNode<T>* x = root;
		while (nil != x && k != x->key)
		{
			if (k < x->key)
				x = x->left;
			else
				x = x->right;
		}
		return x;
	}
	template<typename T>
	void RedBlackTree<T>::rotateLeft(RedBlackTreeNode<T>* x)
	{
		RedBlackTreeNode<T>* y = x->right;        // set y
		x->right = y->left;            // turn y’s left subtree into x’s right subtree
		if (y->left != nil)
			y->left->p = x;
		y->p = x->p;                // link x’s parent to y
		if (x->p == nil)
			root = y;
		else
			if (x == x->p->left)
				x->p->left = y;
			else
				x->p->right = y;
		y->left = x;                // put x on y’s left
		x->p = y;
	}
	template<typename T>
	void RedBlackTree<T>::rotateRight(RedBlackTreeNode<T>* x)
	{
		RedBlackTreeNode<T>* y = x->left;        // set y
		x->left = y->right;            // turn y’s left subtree into x’s right subtree
		if (y->right != nil)
			y->right->p = x;
		y->p = x->p;                // link x’s parent to y
		if (x->p == nil)
			root = y;
		else if (x == x->p->left)
			x->p->left = y;
		else
			x->p->right = y;
		y->right = x;                // put x on y’s left
		x->p = y;
	}
	template<typename T>
	void RedBlackTree<T>::RBinsertFixup(RedBlackTreeNode<T>* z)
	{
		while (z->p->color == 0)
		{
			if (z->p == z->p->p->left)
			{
				RedBlackTreeNode<T>* y = z->p->p->right;
				if (y->color == 0)                //if "uncle" is red
				{
					z->p->color = 1;                // case 1
					y->color = 1;                    // case 1
					z->p->p->color = 0;                // case 1
					z = z->p->p;                    // case 1
					continue;
				}
				if (z == z->p->right)
				{
					z = z->p;                    // case 2
					rotateLeft(z);                // case 2
				}
				z->p->color = 1;                    // case 3
				z->p->p->color = 0;                    // case 3
				rotateRight(z->p->p);                // case 3
			}
			else // symetrical case, "left" and "right" changes places
			{
				RedBlackTreeNode<T>* y = z->p->p->left;
				if (y->color == 0)
				{
					z->p->color = 1;
					y->color = 1;
					z->p->p->color = 0;
					z = z->p->p;
					continue;
				}
				if (z == z->p->left)
				{
					z = z->p;
					rotateRight(z);
				}
				z->p->color = 1;
				z->p->p->color = 0;
				rotateLeft(z->p->p);
			}
		}
		root->color = 1;
	}
	template<typename T>
	void RedBlackTree<T>::insert(RedBlackTreeNode<T>* z)
	{
		insertBST(z);
		RBinsertFixup(z);
	}
	template<typename T>
	RedBlackTreeNode<T>* RedBlackTree<T>::tree_minimum(RedBlackTreeNode<T>*x)
	{
		while (x->left != nil)
			x = x->left;
		return x;
	}
	template<typename T>
	void RedBlackTree<T>::transplant(RedBlackTreeNode<T>* u, RedBlackTreeNode<T>* v)
	{
		if (u->p == nil)
		{
			root = v;
			return;
		}
		if (u == u->p->left)
			u->p->left = v;
		else
			u->p->right = v;
		v->p = u->p;
	}
	template<typename T>
	void RedBlackTree<T>::remove(RedBlackTreeNode<T>* z)
	{
		RedBlackTreeNode<T>* x;
		if (z->left == nil)
		{
			x = z->right;
			transplant(z, x);
			if (z->color == 1)
				RBdeleteFixup(x);
			return;
		}
		if (z->right == nil)
		{
			x = z->left;
			transplant(z, x);
			if (z->color == 1)
				RBdeleteFixup(x);
			return;
		}

		RedBlackTreeNode<T>* y = tree_minimum(z->right);
		char y_original_color = y->color;
		x = y->right;
		if (y->p == z)
			x->p = y;
		else
		{
			transplant(y, x);
			y->right = z->right;
			y->right->p = y;
		}
		transplant(z, y);
		y->left = z->left;
		y->left->p = y;
		y->color = z->color;

		if (y_original_color == 1)
			RBdeleteFixup(x);
	}
	template<typename T>
	void RedBlackTree<T>::RBdeleteFixup(RedBlackTreeNode<T>* x)
	{
		RedBlackTreeNode<T>* w;
		while (x != root && x->color == 1)
		{
			if (x == x->p->left)
			{
				w = x->p->right;
				if (w->color == 0)
				{
					w->color = 1;
					x->p->color = 0;
					rotateLeft(x->p);
					w = x->p->right;
				}
				if (w->left->color == 1 && w->right->color == 1)
				{
					w->color = 0;
					x = x->p;
				}
				else
				{
					if (w->right->color == 1)
					{
						w->left->color = 1;
						w->color = 0;
						rotateRight(w);
						w = x->p->right;
					}
					w->color = x->p->color;
					x->p->color = 1;
					w->right->color = 1;
					rotateLeft(x->p);
					x = root;
				}
			}
			else
			{
				w = x->p->left;
				if (w->color == 0)
				{
					w->color = 1;
					x->p->color = 0;
					rotateRight(x->p);
					w = x->p->right;
				}
				if (w->left->color == 1 && w->right->color == 1)
				{
					w->color = 0;
					x = x->p;
				}
				else
				{
					if (w->left->color == 1)
					{
						w->right->color = 1;
						w->color = 0;
						rotateLeft(w);
						w = x->p->left;
					}
					w->color = x->p->color;
					x->p->color = 1;
					w->left->color = 1;
					rotateRight(x->p);
					x = root;
				}
			}
		}
		x->color = 1;
	}
}