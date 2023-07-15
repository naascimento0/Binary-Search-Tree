#include <stdio.h>
#include <stdlib.h>
#include "binary_tree.h"

KeyValPair* key_val_pair_construct(void *key, void *val){
    KeyValPair *kvp = malloc(sizeof(KeyValPair));
    kvp->key = key;
    kvp->value = val;
    return kvp;
}
void key_val_pair_destroy(KeyValPair *kvp){
	free(kvp);
}

Node* node_construct(void *key, void *value, Node *left, Node *right){
	Node *node = malloc(sizeof(Node));
	node->kvp = key_val_pair_construct(key, value);
	node->left = left;
	node->right = right;
	return node;
}
void node_destroy(Node *node){
	key_val_pair_destroy(node->kvp);
	free(node);
}

BinaryTree* binary_tree_construct(CmpFn cmp_fn, KeyDestroyFn key_destroy_fn,
    ValDestroyFn val_destroy_fn){

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
	while(current->left != NULL)
		current = current->left;

	return current;
}

void binary_tree_remove(BinaryTree *bt, void *key){
	if(binary_tree_empty(bt))
		exit(printf("in 'binary_tree_remove': binary tree empty"));
//teste 1,2,3 e 4 deram ruim
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
			_transplant_nodes(bt, current, successor, successor->right);
			successor->right = current->right;
		}
		_transplant_nodes(bt, parent, current, successor);
		successor->left = current->left;
	}

	bt->key_destroy_fn(current->kvp->key);
	bt->val_destroy_fn(current->kvp->value);
	node_destroy(current);
}

KeyValPair* binary_tree_min(BinaryTree *bt){
	return bt->root != NULL ? bt->root->kvp : NULL;
}





KeyValPair* binary_tree_max(BinaryTree *bt);
KeyValPair* binary_tree_pop_min(BinaryTree *bt);
KeyValPair* binary_tree_pop_max(BinaryTree *bt);
void *binary_tree_get(BinaryTree *bt, void *key);

//void binary_tree_destroy(BinaryTree *bt){
//	Node *current = bt->root;
//	while(!binary_tree_empty(bt)){
//
//	}
//}
