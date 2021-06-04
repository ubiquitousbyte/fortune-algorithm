//
// Created by denko on 4/28/2021.
//

#include <assert.h>
#include <stdio.h>
#include "AVLTree.c"

int8_t comparator(void *a, void *b)
{
    int32_t *int_a = (int32_t *) a;
    int32_t *int_b = (int32_t *) b;
    if (*int_a > *int_b) return 1;
    else if (*int_a < *int_b) return -1;
    else return 0;
}

void test_node_rotate_left()
{
    // Init
    int32_t first = 18;
    AVLTree_Node_ptr_t first_node = avl_tree_node_new(&first);
    int32_t second = 22;
    AVLTree_Node_ptr_t second_node = avl_tree_node_new(&second);
    int32_t third = 43;
    AVLTree_Node_ptr_t third_node = avl_tree_node_new(&third);
    first_node->right = second_node;
    second_node->parent = first_node;
    second_node->right = third_node;
    third_node->parent = second_node;

    // Execute
    first_node = avl_tree_node_rotate_left(first_node);

    // Assert
    assert(*(int32_t *) first_node->data == 22);
    assert(*(int32_t *) first_node->left->data == 18);
    assert(*(int32_t *) first_node->right->data == 43);
    assert(first_node->left->parent == first_node);
    assert(first_node->right->parent == first_node);

}

void test_node_rotate_right()
{
    // Init
    int32_t first = 43;
    AVLTree_Node_ptr_t first_node = avl_tree_node_new(&first);
    int32_t second = 22;
    AVLTree_Node_ptr_t second_node = avl_tree_node_new(&second);
    int32_t third = 18;
    AVLTree_Node_ptr_t third_node = avl_tree_node_new(&third);
    first_node->left = second_node;
    second_node->parent = first_node;
    second_node->left = third_node;
    third_node->parent = second_node;

    // Execute
    first_node = avl_tree_node_rotate_right(first_node);

    // Assert
    assert(*(int32_t *) first_node->data == 22);
    assert(*(int32_t *) first_node->left->data == 18);
    assert(*(int32_t *) first_node->right->data == 43);
    assert(first_node->left->parent == first_node);
    assert(first_node->right->parent == first_node);
}

void test_node_rotate_left_right()
{
    // Init
    int32_t first = 43;
    AVLTree_Node_ptr_t first_node = avl_tree_node_new(&first);
    int32_t second = 18;
    AVLTree_Node_ptr_t second_node = avl_tree_node_new(&second);
    int32_t third = 22;
    AVLTree_Node_ptr_t third_node = avl_tree_node_new(&third);
    first_node->left = second_node;
    second_node->parent = first_node;
    second_node->right = third_node;
    third_node->parent = second_node;

    // Execute
    first_node = avl_tree_node_rotate_left_right(first_node);

    // Assert
    assert(*(int32_t *) first_node->data == 22);
    assert(*(int32_t *) first_node->left->data == 18);
    assert(*(int32_t *) first_node->right->data == 43);
    assert(first_node->left->parent == first_node);
    assert(first_node->right->parent == first_node);
    assert(first_node->parent == NULL);

}

void test_node_rotate_right_left()
{
    // Init
    int32_t first = 43;
    AVLTree_Node_ptr_t first_node = avl_tree_node_new(&first);
    int32_t second = 45;
    AVLTree_Node_ptr_t second_node = avl_tree_node_new(&second);
    int32_t third = 42;
    AVLTree_Node_ptr_t third_node = avl_tree_node_new(&third);
    first_node->right = second_node;
    second_node->parent = first_node;
    second_node->left = third_node;
    third_node->parent=second_node;

    // Execute
    first_node = avl_tree_node_rotate_right_left(first_node);

    // Assert
    assert(*(int32_t *) first_node->data == 42);
    assert(*(int32_t *) first_node->left->data == 43);
    assert(*(int32_t *) first_node->right->data == 45);
    assert(first_node->left->parent == first_node);
    assert(first_node->right->parent == first_node);
    assert(first_node->parent == NULL);

}

void test_tree_insert()
{
    // Init
    AVLTree_t *tree = avl_tree_new(comparator);
    int32_t first = 43;
    int32_t second = 18;
    int32_t third = 22;

    // Execute
    avl_tree_insert(tree, (void *) &first);
    avl_tree_insert(tree, (void *) &second);
    avl_tree_insert(tree, (void *) &third);

    // Assert
    assert(*(uint32_t *) tree->root->data == 22);
    assert(*(uint32_t *) tree->root->left->data == 18);
    assert(tree->root->left->parent == tree->root);
    assert(*(uint32_t *) tree->root->right->data == 43);
    assert(tree->root->right->parent == tree->root);

    avl_tree_destroy(tree);
}

void test_tree_insert_deep()
{
    // Init
    AVLTree_t *tree = avl_tree_new(comparator);
    int32_t first = 43;
    int32_t second = 18;
    int32_t third = 22;
    int32_t fourth = 15;
    int32_t fifth = 19;

    // Execute
    avl_tree_insert(tree, (void *) &first);
    avl_tree_insert(tree, (void *) &second);
    avl_tree_insert(tree, (void *) &third);
    avl_tree_insert(tree, (void *) &fourth);
    avl_tree_insert(tree, (void *) &fifth);
    assert(*(uint32_t *) tree->root->data == 22);
    assert(*(uint32_t *) tree->root->left->data == 18);
    assert(*(uint32_t *) tree->root->right->data == 43);
    assert(*(uint32_t *) tree->root->left->left->data == 15);
    assert(*(uint32_t *) tree->root->left->right->data == 19);

    avl_tree_destroy(tree);
}

void test_tree_remove_deep()
{
    // Init
    AVLTree_t *tree = avl_tree_new(comparator);
    int32_t first = 43;
    int32_t second = 18;
    int32_t third = 22;
    int32_t fourth = 15;
    int32_t fifth = 19;
    avl_tree_insert(tree, (void *) &first);
    avl_tree_insert(tree, (void *) &second);
    avl_tree_insert(tree, (void *) &third);
    avl_tree_insert(tree, (void *) &fourth);
    avl_tree_insert(tree, (void *) &fifth);

    // Execute
    avl_tree_remove(tree, (void *) &first);

    // Assert
    assert(*(int32_t *) tree->root->data == 19);
    assert(*(int32_t *) tree->root->left->data == 18);
    assert(*(int32_t *) tree->root->right->data == 22);
    assert(*(int32_t *) tree->root->left->left->data == 15);

    avl_tree_destroy(tree);
}

int main(int argc, char *argv[])
{
    test_node_rotate_left();
    test_node_rotate_right();
    test_node_rotate_left_right();
    test_node_rotate_right_left();
    test_tree_insert();
    test_tree_insert_deep();
    test_tree_remove_deep();
}