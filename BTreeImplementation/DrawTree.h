#ifndef DRAW_TREE_H
#define DRAW_TREE_H

#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <cstdio>
#include <cstring>

#include "RedBlackTreeNode.h"

/* Draws ascii picture of a tree.  Allows the user to splay */
/* any node, and then redraws the tree.                     */
/* written by Daniel Sleator http://www.cs.cmu.edu/~sleator */


#define INFINITY (1<<20)
#define LABLEN 20


int min(int X, int Y) { return ((X) < (Y)) ? (X) : (Y); }
int max(int X, int Y) { return ((X) > (Y)) ? (X) : (Y); }

typedef struct pnode_struct Pnode;

struct pnode_struct {
	Pnode * left, *right;
	int edge_length; /* length of the edge from this node to its children */
					 /* number of "\" or "/".  so it's at least 1         */
					 /* unless both children are null.  Then it's 0       */

	int height;      /* The number of rows required to print this tree */
	int lablen;
	int parent_dir;       /* -1=I am left, 0=I am root, 1=right       */
						  /* this is used to decide how to break ties */
						  /* when the label is of even length         */
	char label[LABLEN + 1];
};

int allocs_in_use;
void my_free(void * p) {
	allocs_in_use--;
	free(p);
}
void * my_alloc(int size) {
	void * p = malloc(size);
	allocs_in_use++;
	if (p == NULL) {
		fprintf(stderr, "Ran out of space.  Requested size=%d.\n", size);
		exit(1);
	}
	return p;
}

/* Free all the nodes of the given tree */
void free_ptree(Pnode *pn) {
	if (pn == NULL) return;
	free_ptree(pn->left);
	free_ptree(pn->right);
	my_free(pn);
}

Pnode * build_ptree_rec(RedBlackTreeNode<int> * t, RedBlackTreeNode<int> *end) {
	Pnode * pn;
	if (t == end) return NULL;
	pn = (Pnode *)my_alloc(sizeof(Pnode));
	pn->left = build_ptree_rec(t->left, end);
	pn->right = build_ptree_rec(t->right, end);
	if (pn->left != NULL) pn->left->parent_dir = -1;
	if (pn->right != NULL) pn->right->parent_dir = 1;

	printf(pn->label, "%d(-)", t->key);
	//printf(pn->label, "%d(%d)", t->key, t->N);
	pn->lablen = strlen(pn->label);
	return pn;
}

/*
* Copy the tree from the original structure into the Pnode structure
* fill in the parent_dir, label, and labelen fields, but not the
* edge_length or height fields.
*/
Pnode * build_ptree(RedBlackTreeNode<int> * t, RedBlackTreeNode<int> *end) {
	Pnode *pn;
	if (t == end) return NULL;
	pn = build_ptree_rec(t, end);
	pn->parent_dir = 0;
	return pn;
}
/*
* The lprofile array is description of the left profile of a tree.
* Assuming the root is located at (0,0), lprofile[i] is the leftmost
* point used on row i of the tree.  rprofile is similarly defined.
*/
#define MAX_HEIGHT 1000
int lprofile[MAX_HEIGHT];
int rprofile[MAX_HEIGHT];
/*
* The following function fills in the lprofile array for the given tree.
* It assumes that the center of the label of the root of this tree
* is located at a position (x,y).  It assumes that the edge_length
* fields have been computed for this tree.
*/
void compute_lprofile(Pnode *pn, int x, int y) {
	int i, isleft;
	if (pn == NULL) return;
	isleft = (pn->parent_dir == -1);
	lprofile[y] = min(lprofile[y], x - ((pn->lablen - isleft) / 2));
	if (pn->left != NULL) {
		for (i = 1; i <= pn->edge_length && y + i < MAX_HEIGHT; i++) {
			lprofile[y + i] = min(lprofile[y + i], x - i);
		}
	}
	compute_lprofile(pn->left, x - pn->edge_length - 1, y + pn->edge_length + 1);
	compute_lprofile(pn->right, x + pn->edge_length + 1, y + pn->edge_length + 1);
}
void compute_rprofile(Pnode *pn, int x, int y) {
	int i, notleft;
	if (pn == NULL) return;
	notleft = (pn->parent_dir != -1);
	rprofile[y] = max(rprofile[y], x + ((pn->lablen - notleft) / 2));
	if (pn->right != NULL) {
		for (i = 1; i <= pn->edge_length && y + i < MAX_HEIGHT; i++) {
			rprofile[y + i] = max(rprofile[y + i], x + i);
		}
	}
	compute_rprofile(pn->left, x - pn->edge_length - 1, y + pn->edge_length + 1);
	compute_rprofile(pn->right, x + pn->edge_length + 1, y + pn->edge_length + 1);
}
/*
* This function fills in the edge_length and height fields of the
* specified tree.
*/
void compute_edge_lengths(Pnode *pn) {
	int h, hmin, i, delta;
	if (pn == NULL) return;
	compute_edge_lengths(pn->left);
	compute_edge_lengths(pn->right);

	/* first fill in the edge_length of pn */
	if (pn->right == NULL && pn->left == NULL) {
		pn->edge_length = 0;
	}
	else {
		if (pn->left != NULL) {
			for (i = 0; i<pn->left->height && i < MAX_HEIGHT; i++) {
				rprofile[i] = -INFINITY;
			}
			compute_rprofile(pn->left, 0, 0);
			hmin = pn->left->height;
		}
		else {
			hmin = 0;
		}
		if (pn->right != NULL) {
			for (i = 0; i<pn->right->height && i < MAX_HEIGHT; i++) {
				lprofile[i] = INFINITY;
			}
			compute_lprofile(pn->right, 0, 0);
			hmin = min(pn->right->height, hmin);
		}
		else {
			hmin = 0;
		}
		delta = 4;
		for (i = 0; i<hmin; i++) {
			delta = max(delta, 2 + 1 + rprofile[i] - lprofile[i]);
			/* the "2" guarantees a gap of 2 between different parts of the tree */
		}
		/* If the node has two children of height 1, then we allow the
		two leaves to be within 1, instead of 2 */
		if (((pn->left != NULL && pn->left->height == 1) ||
			(pn->right != NULL && pn->right->height == 1)) && delta>4) delta--;
		pn->edge_length = ((delta + 1) / 2) - 1;
	}

	/* now fill in the height of pn */
	h = 1;
	if (pn->left != NULL) {
		h = max(pn->left->height + pn->edge_length + 1, h);
	}
	if (pn->right != NULL) {
		h = max(pn->right->height + pn->edge_length + 1, h);
	}
	pn->height = h;
}

int print_next;  /* used by print_level.  If you call "printf()" at   */
				 /* any point, this is the x coordinate of the next   */
				 /* char printed.                                     */
				 /*
				 * This function prints the given level of the given tree, assuming
				 * that the node pn has the given x cordinate.
				 */
void print_level(Pnode *pn, int x, int level) {
	int i, isleft;
	if (pn == NULL) return;
	isleft = (pn->parent_dir == -1);
	if (level == 0) {
		for (i = 0; i<(x - print_next - ((pn->lablen - isleft) / 2)); i++) {
			printf(" ");
		}
		print_next += i;
		printf("%s", pn->label);
		print_next += pn->lablen;
	}
	else if (pn->edge_length >= level) {
		if (pn->left != NULL) {
			for (i = 0; i<(x - print_next - (level)); i++) {
				printf(" ");
			}
			print_next += i;
			printf("/");
			print_next++;
		}
		if (pn->right != NULL) {
			for (i = 0; i<(x - print_next + (level)); i++) {
				printf(" ");
			}
			print_next += i;
			printf("\\");
			print_next++;
		}
	}
	else {
		print_level(pn->left, x - pn->edge_length - 1, level - pn->edge_length - 1);
		print_level(pn->right, x + pn->edge_length + 1, level - pn->edge_length - 1);
	}
}
/*
* This pretty-prints the given tree, left-justified.
* The tree is drawn in such a way that both of the edges down from
* a node are the same length.  This length is the minimum such that
* the two subtrees are separated by at least two blanks.
*/
void pretty_print_tree(RedBlackTreeNode<int> * t, RedBlackTreeNode<int> * end) {
	Pnode *proot;
	int xmin, i;
	if (t == end) return;
	proot = build_ptree(t, end);
	compute_edge_lengths(proot);
	for (i = 0; i<proot->height && i < MAX_HEIGHT; i++) {
		lprofile[i] = INFINITY;
	}
	compute_lprofile(proot, 0, 0);
	xmin = 0;
	for (i = 0; i < proot->height && i < MAX_HEIGHT; i++) {
		xmin = min(xmin, lprofile[i]);
	}
	for (i = 0; i < proot->height; i++) {
		print_next = 0;
		print_level(proot, -xmin, i);
		printf("\n");
	}
	if (proot->height >= MAX_HEIGHT) {
		printf("(This tree is taller than %d, and may be drawn incorrectly.)\n", MAX_HEIGHT);
	}
	free_ptree(proot);
}

#endif