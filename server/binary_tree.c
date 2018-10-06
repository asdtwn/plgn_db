#include "binary_tree.h"

b_tree* create_tree(cmp_bf* _cmp, cmp_be* _cmp_e) {
	b_tree* new_tree = NULL;
	if (_cmp != NULL && _cmp_e != NULL) {
		new_tree = (b_tree*)malloc(sizeof(b_tree));
		if (new_tree != NULL) {
			new_tree->compare = _cmp;
			new_tree->compare_elements = _cmp_e;
			new_tree->root = NULL;
		}
	}
	return new_tree;
}

static bt_node* init_node(b_tree* _tree, unsigned long long int _key, bt_node* _parent, bt_node* _left, bt_node* _right) {
	bt_node* new_node = NULL;
	if (_tree != NULL) {
		new_node = (bt_node*)malloc(sizeof(bt_node));
		if (new_node != NULL) {
			new_node->tree = _tree;
			new_node->key = _key;
			new_node->data = NULL;
			new_node->parent = NULL;
			new_node->left = NULL;
			new_node->right = NULL;
			new_node->parent = _parent;
			new_node->left = _left;
			new_node->right = _right;
		}
	}
	return new_node;
}

static void deinit_node(bt_node** _node) {
	if (_node != NULL) {
		free(*_node);
		*_node = NULL;
	}
}

static void kill_node(bt_node* _node) {
	if(_node != NULL){
		switch (check_node_status(_node)) {
			case LEFT_EXISTS:
			if(is_root_node(_node)){
				_node->tree->root = _node->left;
				_node->left->parent = NULL;
			} else {
				*(node_relation(_node)) = _node->left;
				_node->left->parent = _node->parent;
			}
			break;
			case RIGHT_EXISTS:
			if(is_root_node(_node)){
				_node->tree->root = _node->right;
				_node->right->parent = NULL;
			} else {
				*(node_relation(_node)) = _node->right;
				_node->right->parent = _node->parent;
			}
			break;
			case BOTH_EXISTS:
			if(is_root_node(_node)){
				_node->tree->root = _node->right;
				_node->right->parent = NULL;
				rebuild_tree(_node);
			} else {
				rebuild_tree(_node);
			}
			break;
			case NONE_EXISTS:
			if(is_root_node(_node)){
				_node->tree->root = NULL;
			} else {
				*(node_relation(_node)) = NULL;
			}
			break;
		}
		deinit_node(&_node);
	}
}

static bt_data* init_data(bt_data** _head, ht_element* _element) {
	bt_data* new_data = NULL;
	if (_head != NULL && _element != NULL) {
		new_data = (bt_data*)malloc(sizeof(bt_data));
		if (new_data != NULL) {
			new_data->data = _element;
			new_data->head = _head;
			new_data->next = NULL;
			new_data->prev = NULL;
		}
	}
	return new_data;
}

static void deinit_data(bt_data** _data) {
	if(_data != NULL) {
		free(*_data);
		*_data = NULL;	
	}
}

void kill_bt_data(bt_data* _data) {
	if(_data != NULL) {
		if(*(_data->head) == _data) { // if head of list
			*(_data->head) = _data->next;
		}
		if(_data->prev != NULL) {
			_data->prev->next = _data->next;
		}
		if (_data->next != NULL) {
			_data->next->prev = _data->prev;
		}
		deinit_data(&_data);
	}
}

void kill_bt_node(bt_node* _node) {
	if(_node != NULL) {
		kill_node(_node);
	}
}

int is_empty_node(bt_node* _node) {
	int res = 0;
	if(_node != NULL) {
		(_node->data == NULL) ? (res = 1) : (res = 0);
	}
	return res;
}

void insert_bt_data(bt_node* _node, ht_element* _element) {
	bt_data* new_data = NULL;
	bt_data* curr = NULL;
	if(_node != NULL && _element != NULL) {
		new_data = init_data(&(_node->data), _element);
		if(new_data != NULL) {
			if (*(new_data->head) != NULL) {
				curr = _node->data;
				while(curr != NULL) {
					if(_node->tree->compare_elements(curr->data, new_data->data)) {
						kill_bt_data(new_data); // if same data already exists
						return;
					}
					if(curr->next == NULL) { // if element not found
						new_data->prev = curr;
						curr->next = new_data; // #atmf#
						return;
					}
					curr = curr->next;
				}
			} else {
				_node->data = new_data; // #atmf#
			}
		}
	}
}

bt_node* insert_bt_node(b_tree* _tree, unsigned long long int _key) {
	bt_node* curr_node = NULL;
	int loop_flag = 1;
	if(_tree != NULL) {
		if (_tree->root != NULL) {
			curr_node = _tree->root;
			while(loop_flag) {
				switch(_tree->compare(curr_node->key, _key)) {
					case LEFT_WAY:
					if(curr_node->left != NULL) {
						curr_node = curr_node->left;
					} else {
						curr_node->left = init_node(_tree, _key, curr_node, NULL, NULL); // insert to the left
						curr_node = curr_node->left;
						loop_flag = 0;
					}
					break;
					case RIGHT_WAY:
					if(curr_node->right != NULL) {
						curr_node = curr_node->right;
					} else {
						curr_node->right = init_node(_tree, _key, curr_node, NULL, NULL); // insert to the right
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
			curr_node = init_node(_tree, _key, NULL, NULL, NULL);
			_tree->root = curr_node;
		}
	}
	return curr_node;
}

bt_node* find_bt_node(b_tree* _tree, unsigned long long int _key) {
	bt_node* curr_node = NULL;
	cmp_res status = NOT_IDENTIFIED;
	if(_tree != NULL) {
		if (_tree->root != NULL) {
			curr_node = _tree->root;
			while(status != EQUAL_VAL) {
				status = _tree->compare(curr_node->key, _key);
				switch (status) {

					case LEFT_WAY:
					if(curr_node->left != NULL) {
						curr_node = curr_node->left;
					} else {
						return NULL;
					}
					break;

					case RIGHT_WAY:
					if(curr_node->right != NULL){
						curr_node = curr_node->right;
					} else {
						return NULL;
					}
					break;

					case EQUAL_VAL:
					return curr_node;
					break;
				}
			}
		}
	}
	return curr_node;
}

static bt_data* find_data_by_ptr(b_tree* _tree, ht_element* _element) {
	bt_node* curr_node = NULL;
	bt_data* curr_data = NULL;
	bt_data* data = NULL;
	if(_element != NULL && _tree != NULL) {
		curr_node = find_bt_node(_tree, _element->l_time);
		if( curr_node != NULL) {
			curr_data = curr_node->data;
			while(data == NULL) {
				if(curr_data == NULL) {
					break;
				}
				if(curr_data->data == _element) {
					data = curr_data;
					break;
				}
				curr_data = curr_data->next;
			}
		}
	}
	return data;
}

bt_data* find_bt_data_by_node(bt_node* _node, ht_element* _element) {
	bt_data* curr_data = NULL;
	bt_data* tmp_data = NULL;
	if(_node != NULL && _element != NULL) {
		tmp_data = _node->data;
		while(tmp_data != NULL) {
			if(_node->tree->compare_elements(tmp_data->data, _element)) {
				curr_data = tmp_data;
				break;
			}
			tmp_data = tmp_data->next;
		}
	}
	return curr_data;
}

bt_node* find_bt_expired_node(b_tree* _tree, unsigned long long int _key) {
	bt_node* expired_node = NULL;
	bt_node* tmp_node = NULL;
	if(_tree != NULL) {
		tmp_node = _tree->root;
		while(tmp_node != NULL) {
			switch (_tree->compare(tmp_node->key, _key)) {
				case LEFT_WAY:
				tmp_node = tmp_node->left;
				break;

				case RIGHT_WAY:
				expired_node = tmp_node;
				tmp_node = NULL;
				break;

				case EQUAL_VAL:
				expired_node = tmp_node;
				tmp_node = NULL;
				break;
			}
		}
	}
	return expired_node;
}

bt_data* get_bt_data_from_node(bt_node* _node) {
	bt_data* curr_data = NULL;
	if(_node != NULL) {
		if(_node->data != NULL) {
			curr_data = _node->data;
		}
	}
	return curr_data;
}

/*...........................some static functions....................*/
static bt_node* find_max_leaf(bt_node* _parent) {
	bt_node* leaf = NULL;
	if(_parent != NULL) {
		leaf =_parent;
		while(leaf->right != NULL) {
			leaf = leaf->right;
		}
	}
	return leaf;
}

static bt_node* find_min_leaf(bt_node* _parent) {
	bt_node* leaf = NULL;
	if(_parent != NULL) {
		leaf = _parent;
		while(leaf->left != NULL) {
			leaf = leaf->left;
		}
	}
	return leaf;
}

static node_stat check_node_status(bt_node* _node) {
	if (_node->left != NULL && _node->right != NULL){
		return BOTH_EXISTS;
	}
	if (_node->left != NULL && _node->right == NULL) {
		return LEFT_EXISTS;
	}
	if (_node->left == NULL && _node->right != NULL) {
		return RIGHT_EXISTS;
	}
	return NONE_EXISTS;
}

static int is_root_node(bt_node* _node) {
	if(_node == NULL) {
		return 0;
	}
	return (_node->tree->root == _node) ? 1 : 0 ;
}

static bt_node** node_relation(bt_node* _node) {
	if (_node == NULL) {
		return NULL;
	}
	return (_node == _node->parent->left) ?	&(_node->parent->left) : &(_node->parent->right);
}

static void rebuild_tree(bt_node* _node) {
	bt_node* _tmp;
	if (_node != NULL) {
		/*....1.right node move here......................................*/
		_node->right->parent = _node->parent;
		if(_node->parent != NULL) {
			*(node_relation(_node)) = _node->right;
		}
		/*....2.move right_node left_branch to left_node right_branch.....*/
		_tmp = find_max_leaf(_node->left);
		if(_tmp != NULL) {
			_tmp->right = _node->right->left;
			_tmp->right->parent = _tmp;
			_node->right->left = NULL;
		}
		/*....3.assign left and right nodes...............................*/
		_node->right->left = _node->left;
		_node->left->parent = _node->right;
	}
}

/*.........................compare functioins..........................*/
cmp_res compare_uu(unsigned long long int _num1, unsigned long long int _num2) {
	if(_num2 < _num1) {
		return LEFT_WAY;
	}
	if (_num2 > _num1) {
		return RIGHT_WAY;
	}
	if (_num2 == _num1) {
		return EQUAL_VAL;
	}
}

int compare_elements_hess(const void* _elem1, const void* _elem2) {
	if (_elem1 != NULL && _elem2 != NULL) {
		return (strcmp(((ht_element*)_elem1)->key, ((ht_element*)_elem2)->key) == 0);
	}
}