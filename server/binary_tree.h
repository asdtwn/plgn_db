#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <stdlib.h>
#include "hash_table.h"

typedef enum compre_result {
	LEFT_WAY,
	RIGHT_WAY,
	EQUAL_VAL
} cmp_res;

typedef cmp_res (cmp_bf)(unsigned long long int, unsigned long long int);

typedef struct b_data {
	ht_element* data;

	b_data** head;
	b_data* next;
	b_data* prev;
} bt_data;

typedef struct b_node {

	unsigned long long int key;

	bt_data* data;

	b_node* parent;
	b_node* left;
	b_node* right;
} bt_node;

typedef struct bin_tree{
	bt_node* root;
	cmp_bf* compare;
} b_tree;

b_tree* create_tree(cmp_bf* _cmp);
//void deinit_tree(b_tree* _tree);
bt_node* create_node(unsigned long long int _key, bt_node* _parent, bt_node* _left, bt_node* _right);
void insert_data(b_tree* _tree, unsigned long long int _key, ht_element* _element);
static bt_node* insert_node(b_tree* _tree, unsigned long long int _key);


/*............................compare functions.................................*/
cmp_res compare_uu(unsigned long long int _key1, unsigned long long _key2);

#endif