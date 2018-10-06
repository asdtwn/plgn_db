#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

typedef enum compare_result {
	LEFT_WAY,
	RIGHT_WAY,
	EQUAL_VAL,
	NOT_IDENTIFIED
} cmp_res;

typedef enum node_status {
	LEFT_EXISTS,
	RIGHT_EXISTS,
	BOTH_EXISTS,
	NONE_EXISTS
} node_stat;

struct bin_tree;

typedef cmp_res (cmp_bf)(unsigned long long int, unsigned long long int);
typedef int (cmp_be)(const void*, const void*); // compare elements value

typedef struct b_data {
	ht_element* data;

	struct b_data** head;
	struct b_data* next;
	struct b_data* prev;
} bt_data;

typedef struct b_node {
	struct bin_tree* tree;
	unsigned long long int key;

	bt_data* data;

	struct b_node* parent;
	struct b_node* left;
	struct b_node* right;
} bt_node;

typedef struct bin_tree{
	bt_node* root;
	cmp_bf* compare;
	cmp_be* compare_elements;
} b_tree;

b_tree* create_tree(cmp_bf* _cmp, cmp_be* _cmp_e);
//void deinit_tree(b_tree* _tree);
static bt_node* init_node(b_tree* _tree, unsigned long long int _key, bt_node* _parent, bt_node* _left, bt_node* _right);
static void kill_node(bt_node* _node);
static void deinit_node(bt_node** _node);
static void insert_data(bt_node* _node, ht_element* _element);
static bt_node* insert_node(b_tree* _tree, unsigned long long int _key);
static bt_data* init_data(bt_data** _head, ht_element* _data);
static void kill_data(bt_data* _data);
static void deinit_data(bt_data** _data);
static bt_node* find_node(b_tree* _tree, unsigned long long int _key);
static bt_data* find_data(b_tree* _tree, ht_element* _element);

/*............................some staic functions.............................*/
static bt_node* find_max_leaf(bt_node* _parent);
static bt_node* find_min_leaf(bt_node* _parent);
static node_stat check_node_status(bt_node* _node);
static int is_root_node(bt_node* _node);
static bt_node** node_relation(bt_node* _node);
static void rebuild_tree(bt_node* _node);


/*............................compare functions.................................*/
cmp_res compare_uu(unsigned long long int _key1, unsigned long long _key2);
int compare_elements_hess(const void* _elem1, const void* _elem2);

#endif