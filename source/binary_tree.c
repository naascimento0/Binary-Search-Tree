#include <stdio.h>
#include <stdlib.h>
#include "binary_tree.h"

KeyValPair* key_val_pair_construct(void *key, void *val){
    KeyValPair *kvp = malloc(sizeof(KeyValPair));
    kvp->key = key;
    kvp->value = val;
    return kvp;
}

void key_val_pair_destroy(KeyValPair *kvp, KeyDestroyFn key_destroy_fn, ValDestroyFn val_destroy_fn){
	key_destroy_fn(kvp->key);
	val_destroy_fn(kvp->value);
	free(kvp);
}

Node* node_construct(void *key, void *value, Node *left, Node *right){
	Node *node = malloc(sizeof(Node));
	node->kvp = key_val_pair_construct(key, value);
	node->left = left;
	node->right = right;
	return node;
}
void node_destroy(Node *node, KeyDestroyFn key_destroy_fn, ValDestroyFn val_destroy_fn){
	key_val_pair_destroy(node->kvp, key_destroy_fn, val_destroy_fn);
	free(node);
}

BinaryTree* binary_tree_construct(CmpFn cmp_fn, KeyDestroyFn key_destroy_fn, ValDestroyFn val_destroy_fn){
	BinaryTree *bt = malloc(sizeof(BinaryTree));
	bt->root = NULL;
	bt->cmp_fn = cmp_fn;
	bt->key_destroy_fn = key_destroy_fn;
	bt->val_destroy_fn = val_destroy_fn;
	return bt;
}

void binary_tree_add(BinaryTree *bt, void *key, void *value){
	Node *parent = NULL;
	Node *current = bt->root;
	while(current != NULL){
		int cmp = bt->cmp_fn(current->kvp->key, key);
		parent = current;

		if(cmp > 0)
			current = current->left;

		else if(cmp < 0)
			current = current->right;

		else{
			bt->val_destroy_fn(current->kvp->value);
			current->kvp->value = value;
			return;
		}
	}

	if(parent == NULL){
		bt->root = parent = node_construct(key, value, NULL, NULL);
		return;
	}

	int add_cmp = bt->cmp_fn(parent->kvp->key, key);
	if(add_cmp > 0)
		parent->left = node_construct(key, value, NULL, NULL);

	else if(add_cmp < 0)
		parent->right = node_construct(key, value, NULL, NULL);
}

Node* _add_recursive(Node *node, CmpFn cmp_fn, void *key, void *value){
	if(node == NULL)
		return node_construct(key, value, NULL, NULL);

	if(cmp_fn(node->kvp->key, key) > 0)
		node->left = _add_recursive(node->left, cmp_fn, key, value);
	else
		node->right = _add_recursive(node->right, cmp_fn, key, value);

	return node;
}

void binary_tree_add_recursive(BinaryTree *bt, void *key, void *value){
	bt->root = _add_recursive(bt->root, bt->cmp_fn, key, value);
}

int binary_tree_empty(BinaryTree *bt){
	return bt->root == NULL;
}

void* binary_tree_get(BinaryTree *bt, void *key){
	Node *current = bt->root;
	while(current != NULL){
		int cmp = bt->cmp_fn(current->kvp->key, key);
		if(cmp > 0)
			current = current->left;
		else if(cmp < 0)
			current = current->right;
		else
			return current->kvp->value;
	}

	return NULL;
}

void _transplant_nodes(BinaryTree *bt, Node *parent, Node *current, Node *current_child){
	if(parent == NULL)
		bt->root = current_child;
	else if(current == parent->left)
		parent->left = current_child;
	else if(current == parent->right)
		parent->right = current_child;
}
Node* _binary_tree_get_sucessor(Node *node){
	Node *current = node->right;
	while(current != NULL && current->left != NULL)
		current = current->left;

	return current;
}

void binary_tree_remove(BinaryTree *bt, void *key){
	if(binary_tree_empty(bt))
		exit(printf("in 'binary_tree_remove': binary tree empty"));

	Node *parent = NULL;
	Node *current = bt->root;
	while(current != NULL && bt->cmp_fn(current->kvp->key, key)){
		parent = current;

		int cmp = bt->cmp_fn(current->kvp->key, key);
		if(cmp > 0)
			current = current->left;
		else if(cmp < 0)
			current = current->right;
	}

	if(current == NULL)
		exit(printf("in 'binary_tree_remove': node not found"));

	if(current->left == NULL)
		_transplant_nodes(bt, parent, current, current->right);
	else if(current->right == NULL)
		_transplant_nodes(bt, parent, current, current->left);
	else{
		Node *successor = _binary_tree_get_sucessor(current);
		if(current->right != successor){
			Node *temp = current->right;
			while(temp->left != successor)
				temp = temp->left;

			_transplant_nodes(bt, temp, successor, successor->right);
			successor->right = current->right;
		}

		_transplant_nodes(bt, parent, current, successor);
		successor->left = current->left;
	}

	node_destroy(current, bt->key_destroy_fn, bt->val_destroy_fn);
}

KeyValPair* binary_tree_min(BinaryTree *bt){
	Node *current = bt->root;
	while(current != NULL && current->left != NULL)
		current = current->left;

	KeyValPair *kvp = key_val_pair_construct(current->kvp->key, current->kvp->value);
	return kvp;
}

KeyValPair* binary_tree_max(BinaryTree *bt){
	Node *current = bt->root;
	while(current != NULL && current->right != NULL)
		current = current->right;

	KeyValPair *kvp = key_val_pair_construct(current->kvp->key, current->kvp->value);
	return kvp;
}

KeyValPair* binary_tree_pop_min(BinaryTree *bt){
	if(binary_tree_empty(bt))
		exit(printf("in 'binary_tree_pop_min': binary tree empty"));

	Node *parent = NULL;
	Node *current = bt->root;
	while(current->left != NULL){
		parent = current;
		current = current->left;
	}

	if(parent == NULL)
		bt->root = current->right;
	else
		parent->left = current->right;

	KeyValPair *kvp = current->kvp;
	free(current);
	return kvp;
}
KeyValPair* binary_tree_pop_max(BinaryTree *bt){
	if(binary_tree_empty(bt))
		exit(printf("in 'binary_tree_pop_max': binary tree empty"));

	Node *parent = NULL;
	Node *current = bt->root;
	while(current->right != NULL){
		parent = current;
		current = current->right;
	}

	if(parent == NULL)
		bt->root = current->left;
	else
		parent->right = current->left;

	KeyValPair *kvp = current->kvp;
	free(current);
	return kvp;
}

void _destroy_recursive(Node *root){
	if(root == NULL)
        return;
    else{
        _destroy_recursive(root->left);
        _destroy_recursive(root->right);
        free(root);
    }
}

void binary_tree_destroy(BinaryTree *bt){
	 if(bt != NULL){
		 _destroy_recursive(bt->root);
		 free(bt);
	 }
}

Vector* binary_tree_inorder_traversal(BinaryTree *bt){
	Vector *v = vector_construct();
	Stack *s = stack_construct();

	Node *current = bt->root;
	do{
		while(current != NULL){
			stack_push(s, current);
			current = current->left;
		}

		if(!stack_empty(s)){
			current = stack_pop(s);
			vector_push_back(v, current->kvp);
			current = current->right;
		}

	}while(!stack_empty(s) || current != NULL);

	stack_destroy(s);
	return v;
}

Vector* binary_tree_preorder_traversal(BinaryTree *bt){
	Vector *v = vector_construct();
	Stack *s = stack_construct();

	Node *current = bt->root;
	do{
		while(current != NULL){
			stack_push(s, current);
			vector_push_back(v, current->kvp);
			current = current->left;
		}

		if(!stack_empty(s)){
			current = stack_pop(s);
			current = current->right;
		}

	}while(!stack_empty(s) || current != NULL);

	stack_destroy(s);
	return v;
}

Vector* binary_tree_postorder_traversal(BinaryTree *bt){
	Vector *v = vector_construct();
	Stack *s = stack_construct();

	Node *current = bt->root;
	Node *last_visited = NULL;
	do{

		if(current != NULL){
			stack_push(s, current);
			current = current->left;
		}else{
			Node *top = stack_top(s);

			if(top->right != NULL && top->right != last_visited)
				current = top->right;
			else{
				vector_push_back(v, top->kvp);
				last_visited = top;
				stack_pop(s);
			}
		}

	}while(!stack_empty(s) || current != NULL);

	stack_destroy(s);
	return v;
}

Vector* binary_tree_levelorder_traversal(BinaryTree *bt){
	Vector *v = vector_construct();
	Queue *q = queue_construct();
	queue_enqueue(q, bt->root);

	do{
		Node *current = queue_dequeue(q);
		if(current != NULL){
			vector_push_back(v, current->kvp);
			queue_enqueue(q, current->left);
			queue_enqueue(q, current->right);
		}

	}while(!queue_empty(q));

	queue_destroy(q);
	return v;
}

Vector* _in_order(Vector *v, Node *current){
	if(current != NULL){
		v = _in_order(v, current->left);
		vector_push_back(v, current->kvp);
		v = _in_order(v, current->right);
	}

	return v;
} 

Vector* binary_tree_inorder_traversal_recursive(BinaryTree *bt){
	Vector *v = vector_construct();
	return _in_order(v, bt->root);
}

Vector* _pre_order(Vector *v, Node *current){
	if(current != NULL){
		vector_push_back(v, current->kvp);
		v = _pre_order(v, current->left);
		v = _pre_order(v, current->right);
	}
	
	return v;
} 

Vector* binary_tree_preorder_traversal_recursive(BinaryTree *bt){
	Vector *v = vector_construct();
	return _pre_order(v, bt->root);
}

Vector* _post_order(Vector *v, Node *current){
	if(current != NULL){
		v = _post_order(v, current->left);
		v = _post_order(v, current->right);
		vector_push_back(v, current->kvp);
	}
	
	return v;
} 

Vector* binary_tree_postorder_traversal_recursive(BinaryTree *bt){
	Vector *v = vector_construct();
	return _post_order(v, bt->root);
}
