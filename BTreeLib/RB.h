#pragma once
//ფიქტიური ფოთოლი, როგორც MIT-ის ალგორითმებშია, და ასევე left, right  შვილები

#include <iostream>
#include <mutex>

using namespace std;

template<typename T>
struct Node
{
	T data;
	Node* p;
	Node* left, *right;
	Node() {};
	Node(T k) { data = k; };
	char color;
};

template<typename T>
inline void needstobecleared(T* t)
{
	delete t;
	cout << "ref" << endl;
}
template<typename T>
inline void needstobecleared(T &t)
{
	//cout << "val" << endl;
}

template<typename T>
class Tree
{
	mutex m;
	int size;
	Node<T>* tree_minimum(Node<T>*);
	void rotateLeft(Node<T>* x);
	void rotateRight(Node<T>* x);
	void destroyTree(Node<T>* node);
	void RBinsertFixup(Node<T>* z);
	void insertBST(Node<T>* z);
	void RBdeleteFixup(Node<T>* z);
	void transplant(Node<T>* u, Node<T>* v);
	Node<T>* nil;
	Node<T>* root;
public:
	bool(*p)(T, T) = [](T  a, T b) {return a < b; };
	Tree();
	Tree(bool(*cmd)(T, T));
	~Tree();
	void RB_insert(Node<T>* z);
	void RB_delete(Node<T>* z);
	Node<T>* search(T k);
	Node<T>* getNil() { return nil; }
	Node<T>* getRoot() { return root; }
	int getSize() { return size; }
};

template<typename T>
Tree<T>::Tree()
{
	std::lock_guard<std::mutex> guard(m);
	nil = new Node<T>;
	nil->color = 1;
	root = nil;
	size = 0;
}

template<typename T>
Tree<T>::Tree(bool(*cmd)(T, T))
{
	std::lock_guard<std::mutex> guard(m);
	p = cmd;
	nil = new Node<T>;
	nil->color = 1;
	root = nil;
	size = 0;
}


template<typename T>
Tree<T>::~Tree()
{
	std::lock_guard<std::mutex> guard(m);
	destroyTree(root);
	delete nil;
}
template<typename T>
void Tree<T>::destroyTree(Node<T>* node) {
	if (node != nil) {
		//თუ პოინტერებია T -ში ნაგულისხმები?
		destroyTree(node->right);
		destroyTree(node->left);
		needstobecleared(node->data);
		delete node;
	}
}

template<typename T>
void Tree<T>::insertBST(Node<T>* z)
{
	size++;
	Node<T>* x(root);
	Node<T>* y(nil);
	while (nil != x)
	{
		y = x;
		//if (z->data < x->data)
		if (p(z->data, x->data))
			x = x->left;
		else
			x = x->right;
	}
	z->p = y;
	if (y == nil)
		root = z;
	else
		//if (z->data < y->data)
		if (p(z->data, y->data))
			y->left = z;
		else
			y->right = z;
	z->left = nil;
	z->right = nil;
	z->color = 0;
}

template<typename T>
Node<T>* Tree<T>::search(T k)
{
	std::lock_guard<std::mutex> guard(m);
	Node<T>* x = root;
	while (nil != x && k != x->data)
	{
		//if (k < x->key)
		if (p(k, x->data))
			x = x->left;
		else
			x = x->right;
	}
	return x;
}

template<typename T>
void Tree<T>::rotateLeft(Node<T>* x)
{
	Node<T>* y = x->right;			// set y
	x->right = y->left;				// turn y’s left subtree into x’s right subtree
	if (y->left != nil)
		y->left->p = x;
	y->p = x->p;					// link x’s parent to y
	if (x->p == nil)
		root = y;
	else
		if (x == x->p->left)
			x->p->left = y;
		else
			x->p->right = y;
	y->left = x;					// put x on y’s left
	x->p = y;
}
template<typename T>
void Tree<T>::rotateRight(Node<T>* x)
{
	Node<T>* y = x->left;			// set y
	x->left = y->right;				// turn y’s left subtree into x’s right subtree
	if (y->right != nil)
		y->right->p = x;
	y->p = x->p;					// link x’s parent to y
	if (x->p == nil)
		root = y;
	else if (x == x->p->left)
		x->p->left = y;
	else
		x->p->right = y;
	y->right = x;					// put x on y’s left
	x->p = y;
}

template<typename T>
void Tree<T>::RBinsertFixup(Node<T>* z)
{
	while (z->p->color == 0)
	{
		if (z->p == z->p->p->left)
		{
			Node<T>* y = z->p->p->right;
			if (y->color == 0)				//თუ "ბიძა" წითელია
			{
				z->p->color = 1;				// შემთხვევა 1
				y->color = 1;					// შემთხვევა 1
				z->p->p->color = 0;				// შემთხვევა 1
				z = z->p->p;					// შემთხვევა 1
				continue;
			}
			if (z == z->p->right)
			{
				z = z->p;						// შემთხვევა 2
				rotateLeft(z);					// შემთხვევა 2
			}
			z->p->color = 1;					// შემთხვევა 3
			z->p->p->color = 0;					// შემთხვევა 3
			rotateRight(z->p->p);				// შემთხვევა 3
		}
		else // სიმეტრიული შემთხვევა, ”left”  და ”right” ცვლიან ადგილებს
		{
			Node<T>* y = z->p->p->left;
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
void Tree<T>::RB_insert(Node<T>* z)
{
	std::lock_guard<std::mutex> guard(m);
	insertBST(z);
	RBinsertFixup(z);
}
template<typename T>
Node<T>* Tree<T>::tree_minimum(Node<T>*x)
{
	while (x->left != nil)
		x = x->left;
	return x;
}
template<typename T>
void Tree<T>::transplant(Node<T>* u, Node<T>* v)
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
void Tree<T>::RB_delete(Node<T>* z)
{
	std::lock_guard<std::mutex> guard(m);
	Node<T>* x;
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

	Node<T>* y = tree_minimum(z->right);			/////////////
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

template<typename T >
void Tree<T>::RBdeleteFixup(Node<T>* x)
{
	Node<T>* w;
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
				w = x->p->left;   //აქ ეწერა რომ მარჯვენა სიბლინგი მიინიჭეო, ე. ი. კოპირებისა დარჩა
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

