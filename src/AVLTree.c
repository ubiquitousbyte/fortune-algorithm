//
// Created by denko on 4/30/2021.
//

#include <stdint.h>
#include <stdlib.h>

#include "AVLTree.h"

struct AVLTree_Node
{
    void *data;
    struct AVLTree_Node *left;
    struct AVLTree_Node *right;
    struct AVLTree_Node *parent;
    int32_t height;
};

struct AVLTree
{
    struct AVLTree_Node *root;
    avl_tree_node_comparator cmp;
    int32_t count;
};


/**
 * Allocates memory for a tree node holding some data
 *
 * @param data the data that the tree node will hold
 * @return a handle to the node
 */
AVLTree_Node_ptr_t avl_tree_node_new(void *data)
{
    AVLTree_Node_ptr_t node = malloc(sizeof(AVLTree_Node_t));
    if (NULL == node) return NULL;
    node->left = node->right = node->parent = NULL;
    node->data = data;
    node->height = 0;
    return node;
}

/**
 * Allocates memory for the balanced binary search tree
 *
 * @param cmp the comparator function to be used when traversing the tree
 * @return a tree handle
 */
AVLTree_ptr_t avl_tree_new(avl_tree_node_comparator cmp)
{
    AVLTree_ptr_t tree = malloc(sizeof(AVLTree_t));
    if (NULL == tree) return NULL;
    tree->cmp = cmp;
    tree->root = NULL;
    tree->count = 0;
    return tree;
}

/**
 * Deallocates the memory for the given node
 * This function also deallocates the memory of the node's children
 *
 * @param node the node
 */
void avl_tree_node_destroy(AVLTree_Node_ptr_t node)
{
    if (node)
    {
        if (node->left)
        {
            avl_tree_node_destroy(node->left);
        }
        if (node->right)
        {
            avl_tree_node_destroy(node->right);
        }
        free(node);
    }
}

/**
 * Deallocates the memory used by self
 *
 * @param self the tree handle
 */
void avl_tree_destroy(AVLTree_ptr_t self)
{
    if (self)
    {
        if (self->root)
        {
            avl_tree_node_destroy(self->root);
        }
        free(self);
    }
}


/**
 * Performs a left rotation of a subtree given a starting position
 *
 * E.g:
 * 4 -> NULL
 *   -> 6 -> NULL
 *        -> 8
 * yields  6 -> 4
 *           -> 8
 * @param node the root node of the tree to perform the rotation on
 * @return a handle to the new node
 */
static AVLTree_Node_ptr_t avl_tree_node_rotate_left(AVLTree_Node_ptr_t node)
{
    AVLTree_Node_ptr_t median = node->right;
    node->right = median->left;
    median->left = node;
    median->parent = node->parent;
    node->parent = median;
    return median;
}

/**
 * Performs a right rotation of a subtree given a starting position
 *
 * E.g:
 *  8 -> 6 -> 4
 *         -> NULL
 *    -> NULL
 * yields 6 -> 4
 *          -> 8
 * @param node the root node of the tree to perform the rotation on
 * @return a handle to the new node
 */
static AVLTree_Node_ptr_t avl_tree_node_rotate_right(AVLTree_Node_ptr_t node)
{
    AVLTree_Node_ptr_t median = node->left;
    node->left = median->right;
    median->right = node;
    median->parent = node->parent;
    node->parent = median;
    return median;
}

/**
 * Performs a left rotation and then a right rotation to balance the subtree
 *
 * E.g:
 *  5 -> 3 -> NULL
 *         -> 4
 *    -> NULL
 * yields after a left rotation:
 * 5 -> 4 -> 3
 *        -> NULL
 *   -> NULL
 * and after the right rotation:
 * 4 -> 3
 *   -> 5
 * @param node the root node of the subtree
 * @return the new root node
 */
static AVLTree_Node_ptr_t avl_tree_node_rotate_left_right(AVLTree_Node_ptr_t node)
{
    node->left = avl_tree_node_rotate_left(node->left);
    return avl_tree_node_rotate_right(node);
}

/**
 * Performs a right rotation and then a left rotation to balance the subtree
 *
 * @see avl_tree_node_rotate_left_right. It's the same but the operations are performed in reversed order
 * @param node the root node of the subtree
 * @return the new root node
 */
static AVLTree_Node_ptr_t avl_tree_node_rotate_right_left(AVLTree_Node_ptr_t node)
{
    node->right = avl_tree_node_rotate_right(node->right);
    return avl_tree_node_rotate_left(node);
}

/**
 * Checks whether node is a leaf node or not
 *
 * @param node the node to check
 * @return 1 if it is, 0 otherwise
 */
int8_t avl_tree_node_is_leaf(AVLTree_Node_ptr_t node)
{
    return node->left == NULL && node->right == NULL;
}

/**
 * Yields the height of the node
 *
 * @param node the node
 * @return its height
 */
int32_t avl_tree_node_height(AVLTree_Node_ptr_t node)
{
    if (! node ) return -1;
    if (avl_tree_node_is_leaf(node)) return 0;
    return node->height;
}

/**
 * Recomputes the height of the node
 *
 * @param node the node
 */
static void avl_tree_node_update_height(AVLTree_Node_ptr_t node)
{
    int32_t left_height = avl_tree_node_height(node->left);
    int32_t right_height = avl_tree_node_height(node->right);
    node->height = ((left_height > right_height) ? left_height : right_height) + 1;
}

/**
 * Balances the 2-layer subtree whose root is the node
 *
 * @param current the node
 * @return a pointer to the newly balanced tree
 */
static AVLTree_Node_ptr_t avl_tree_node_balance(AVLTree_ptr_t self, AVLTree_Node_ptr_t current)
{
    int32_t left_height = avl_tree_node_height(current->left);
    int32_t right_height = avl_tree_node_height(current->right);
    int32_t diff = left_height - right_height;
    if (diff > 1)
    {
        int32_t left_left_height = avl_tree_node_height(current->left->left);
        int32_t left_right_height = avl_tree_node_height(current->left->right);
        if (left_left_height > left_right_height)
        {
            current = avl_tree_node_rotate_right(current);
        }
        else
        {
            current = avl_tree_node_rotate_left_right(current);
        }
        avl_tree_node_update_height(current->left);
        avl_tree_node_update_height(current->right);
    }
    else if (diff < -1)
    {
        int32_t right_left_height = avl_tree_node_height(current->right->left);
        int32_t right_right_height = avl_tree_node_height(current->right->right);
        if (right_left_height > right_right_height)
        {
            current = avl_tree_node_rotate_right_left(current);
        }
        else
        {
            current = avl_tree_node_rotate_left(current);
        }
    }
    avl_tree_node_update_height(current);
    // Readjust the root pointer
    if (NULL == current->parent)
    {
        self->root = current;
    }
    return current;
}

/**
 * Balances a full tree
 *
 * @param self the tree handle
 * @param root the root node of the tree
 */
static void avl_tree_balance(AVLTree_ptr_t self, AVLTree_Node_ptr_t root)
{
    AVLTree_Node_ptr_t iterator = root;
    while (iterator)
    {
        iterator = avl_tree_node_balance(self, iterator);
        iterator = iterator->parent;
    }
}

/**
 * Recursively adds a node holding data to the tree defined by self.
 *
 * @param self the tree
 * @param current the start point of the recursion
 * @param data the data point to add
 */
static void avl_tree_add_node(AVLTree_ptr_t self, AVLTree_Node_ptr_t current, void *data)
{
    int8_t diff = self->cmp(data, current->data);
    if (diff < 0)
    {
        if (! current->left)
        {
            current->left = avl_tree_node_new(data);
            current->left->parent = current;
            self->count++;
        }
        else
        {
            avl_tree_add_node(self, current->left, data);
        }
        avl_tree_balance(self, current->left);
    }
    else
    {
        if (! current->right)
        {
            current->right = avl_tree_node_new(data);
            current->right->parent = current;
            self->count++;
        }
        else
        {
            avl_tree_add_node(self, current->right, data);
        }
        avl_tree_balance(self, current->right);
    }
}

/**
 * Inserts the data into the tree
 *
 * @param self the tree
 * @param data the data
 */
void avl_tree_insert(AVLTree_ptr_t self, void *data)
{
    if (! self) return;
    if (! data) return;
    if (! self->root)
    {
        self->root = avl_tree_node_new(data);
        self->count++;
        return;
    }
    avl_tree_add_node(self, self->root, data);
}

/**
 * Finds the in-order successor of the given node
 *
 * @param node the node
 * @return a handle to the in-order successor
 */
static AVLTree_Node_ptr_t avl_tree_node_in_order_successor(AVLTree_Node_ptr_t node)
{
    AVLTree_Node_ptr_t right = node->right;
    while (right->left)
    {
        right = right->left;
    }
    return right;
}

/**
 * Finds the in-order predecessor of the given node
 *
 * @param node the node
 * @return a handle to the in-order predecessor
 */
static AVLTree_Node_ptr_t avl_tree_node_in_order_predecessor(AVLTree_Node_ptr_t node)
{
    AVLTree_Node_ptr_t left = node->left;
    while (left->right)
    {
        left = left->right;
    }
    return left;
}

static AVLTree_Node_ptr_t avl_tree_node_remove(AVLTree_ptr_t self, AVLTree_Node_ptr_t current, void *data)
{
    if (avl_tree_node_is_leaf(current))
    {
        if (current == self->root)
        {
            self->root = NULL;
        }
        self->count--;
        avl_tree_node_destroy(current);
        return NULL;
    }

    int8_t diff = self->cmp(current->data, data);
    if (diff == -1)
    {
        current->right = avl_tree_node_remove(self, current->right, data);
    }
    else if (diff == 1)
    {
        current->left = avl_tree_node_remove(self, current->left, data);
    }
    else
    {
        if (current->left)
        {
            AVLTree_Node_ptr_t predecessor = avl_tree_node_in_order_predecessor(current);
            current->data = predecessor->data;
            current->left = avl_tree_node_remove(self, current->left, predecessor->data);
        }
        else
        {
            AVLTree_Node_ptr_t successor = avl_tree_node_in_order_successor(current);
            current->data = successor->data;
            current->right = avl_tree_node_remove(self, current->right, successor->data);
        }
    }
    avl_tree_balance(self, current);
    return current;
}

/**
 * Removes the node holding the data from the tree
 *
 * @param self the tree handle
 * @param data the data
 */
AVLTree_Node_ptr_t avl_tree_remove(AVLTree_ptr_t self, void *data)
{
    return avl_tree_node_remove(self, self->root, data);
}

/**
 * Finds the node in the tree that holds data
 *
 * @param self the tree
 * @param data the data
 * @return the node that holds the data
 */
AVLTree_Node_ptr_t avl_tree_find(AVLTree_ptr_t self, void *data)
{
    if (! self) return NULL;
    AVLTree_Node_ptr_t iterator = self->root;
    while (iterator)
    {
        int8_t cmp = self->cmp(data, iterator->data);
        if (cmp == 0)
        {
            break;
        }
        else if (cmp < 0)
        {
            iterator = iterator->left;
        }
        else
        {
            iterator = iterator->right;
        }
    }
    return iterator;
}

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
avl_tree_replace_leaf_node(AVLTree_ptr_t self, AVLTree_Node_ptr_t node, AVLTree_Node_ptr_t replacement)
{
    if (! avl_tree_node_is_leaf(node)) return NULL;
    if (replacement)
    {
        replacement->parent = node->parent;
    }
    if (node->parent && node->parent->left == node)
    {
        node->parent->left = replacement;
    }
    else if (node->parent && node->parent->right == node)
    {
        node->parent->right = replacement;
    }
    else
    {
        self->root = replacement;
    }
    if (replacement)
    {
        avl_tree_node_update_height(replacement);
    }
    else if (node->parent)
    {
        avl_tree_node_update_height(node->parent);
    }
    return self->root;
}