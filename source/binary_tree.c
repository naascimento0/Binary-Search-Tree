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

void binary_tree_add(BinaryTree *bt, void *key, void *value);
int binary_tree_empty(BinaryTree *bt);
void binary_tree_remove(BinaryTree *bt, void *key);
KeyValPair* binary_tree_min(BinaryTree *bt);
KeyValPair* binary_tree_max(BinaryTree *bt);
KeyValPair* binary_tree_pop_min(BinaryTree *bt);
KeyValPair* binary_tree_pop_max(BinaryTree *bt);
void *binary_tree_get(BinaryTree *bt, void *key);
void binary_tree_destroy(BinaryTree *bt);
