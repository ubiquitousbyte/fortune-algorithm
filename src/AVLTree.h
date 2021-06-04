//
// Created by denko on 4/30/2021.
//

#ifndef VORONOI_AVLTREE_H
#define VORONOI_AVLTREE_H

typedef struct AVLTree_Node AVLTree_Node_t;
typedef AVLTree_Node_t* AVLTree_Node_ptr_t;

typedef struct AVLTree AVLTree_t;
typedef AVLTree_t* AVLTree_ptr_t;

typedef int8_t (*avl_tree_node_comparator)(void *first, void *second);

/**
 * Allocates memory for a tree node holding some data
 *
 * @param data the data that the tree node will hold
 * @return a handle to the node
 */
AVLTree_Node_ptr_t avl_tree_node_new(void *data);

/**
 * Deallocates the memory for the given node
 * This function also deallocates the memory of the node's children
 *
 * @param node the node
 */
void avl_tree_node_destroy(AVLTree_Node_ptr_t node);

/**
 * Checks whether node is a leaf node or not
 *
 * @param node the node to check
 * @return 1 if it is, 0 otherwise
 */
int8_t avl_tree_node_is_leaf(AVLTree_Node_ptr_t node);

/**
 * Yields the height of the node
 *
 * @param node the node
 * @return its height
 */
int32_t avl_tree_node_height(AVLTree_Node_ptr_t node);

/**
 * Allocates memory for the balanced binary search tree
 *
 * @param cmp the comparator function to be used when traversing the tree
 * @return a tree handle
 */
AVLTree_ptr_t avl_tree_new(avl_tree_node_comparator cmp);

/**
 * Deallocates the memory used by self
 *
 * @param self the tree handle
 */
void avl_tree_destroy(AVLTree_ptr_t self);

/**
 * Inserts the data into the tree
 *
 * @param self the tree
 * @param data the data
 */
void avl_tree_insert(AVLTree_ptr_t self, void *data);

/**
 * Removes the node holding the data from the tree
 *
 * @param self the tree handle
 * @param data the data
 */
AVLTree_Node_ptr_t avl_tree_remove(AVLTree_ptr_t self, void *data);

/**
 * Finds the node in the tree that holds data
 *
 * @param self the tree
 * @param data the data
 * @return the node that holds the data
 */
AVLTree_Node_ptr_t avl_tree_find(AVLTree_ptr_t self, void *data);

/**
 * Replaces the node with a subtree
 * The node must be a leaf node for this to work
 *
 * @param self the tree handle
 * @param node the node to replace
 * @param replacement the replacement subtree
 * @return the root of the new tree
 */
AVLTree_Node_ptr_t
avl_tree_replace_leaf_node(AVLTree_ptr_t self, AVLTree_Node_ptr_t node, AVLTree_Node_ptr_t replacement);

#endif //VORONOI_AVLTREE_H
