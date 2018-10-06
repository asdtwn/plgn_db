#include "binary_tree.h"

b_tree* create_tree(cmp_bf* _cmp) {
	b_tree* new_tree = NULL;
	if (_cmp != NULL) {
		new_tree = (b_tree*)malloc(sizeof(b_tree));
		if (new_tree != NULL) {
			new_tree->compare = _cmp;
			new_tree->root = NULL;
		}
	}
	return new_tree;
}

bt_node* create_node(unsigned long long int _key, bt_node* _parent, bt_node* _left, bt_node* _right) {
	bt_node* new_node = NULL;
	new_node = (bt_node*)malloc(sizeof(bt_node));
	if (new_node != NULL) {
		new_node->key = _key;
		new_node->data = NULL;
		new_node->parent = NULL;
		new_node->left = NULL;
		new_node->right = NULL;
		new_node->parent = _parent;
		new_node->left = _left;
		new_node->right = _right;
	}
	return new_node;
}

void insert_data(b_tree* _tree, unsigned long long int _key, bt_element* _element) {

}

static bt_node* insert_node(b_tree* _tree, unsigned long long int _key) {
	bt_node* curr_node = NULL;
	int loop_flag = 1;
	if (_tree->root != NULL) {
		curr_node = _tree->root;
		while(loop_flag) {
			switch(_tree->compare(curr_node->key, _key)) {
				case LEFT_WAY:
				if(curr_node->left != NULL) {
					curr_node = curr_node->left;
				} else {
					curr_node->left = create_node(_key, curr_node, NULL, NULL); // insert to the left
					curr_node = curr_node->left;
					loop_flag = 0;
				}
				break;
				case RIGHT_WAY:
				if(curr_node->right != NULL) {
					curr_node = curr_node->right;
				} else {
					curr_node->right = create_node(_key, curr_node, NULL, NULL); // insert to the right
					curr_node = curr_node->right;
					loop_flag = 0;
				}
				break;
				case EQUAL_VAL:
					loop_flag = 0;
				break;
			}
		}
	} else {
		curr_node = create_node(_key, NULL, NULL, NULL);
	}
	return curr_node;
}