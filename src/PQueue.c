//
// Created by denko on 4/24/2021.
//

#include "PQueue.h"
/**
 * Implemented by Atanas Denkov
 *
 * The implementation is inspired by Frank Pfenning's beautiful lecture notes on
 * "Principles of Imperative Computation"
 * http://jamiemorgenstern.com/teaching/su-122/lectures/14
 */

/**
 * Swaps the node at index first with the node at index second
 *
 * @param self the queue handle
 * @param first the first node index
 * @param second the second node index
 */
static void swap(PQueue_t *self, uint64_t first, uint64_t second)
{
    void* tmp = self->heap[first];
    self->heap[first] = self->heap[second];
    self->heap[second] = tmp;
}

/**
 * Restores the ordering invariant of the heap tree after an enqueue operation
 *
 * @param self the queue handle
 * @param i the current node index of the heap tree
 * @return the new node index
 */
static uint64_t sift_up(PQueue_t *self, uint64_t i)
{
    // Iterate until we reach the root node index, which is 1
    uint64_t current = i;
    while (current > 1)
    {
        // Compute the position of the parent node
        uint64_t parent = current/2;
        // If the parent has a lower priority than its child, swap them
        if (self->cmp(self->heap[parent], self->heap[current]) == -1)
        {
            swap(self, parent, current);
        }
        // Set the current node to the parent node to continue the iteration
        current = parent;
    }
    return current;
}

/**
 * Restores the ordering invariant of the heap tree after a dequeue operation
 *
 * @param self the queue handle
 * @param i the current node index of the heap three
 */
static void sift_down(PQueue_t *self, uint64_t i)
{
    while(1)
    {
        // Compute the positions of i's children
        uint64_t left_child = 2*i;
        uint64_t right_child = 2*i+1;
        uint64_t current_child;

        // We need to take the path of highest priority
        // We therefore compare the priority measurements of the children
        // First, let's check if i even has both children
        if (right_child >= self->next)
        {
            if (left_child >= self->next)
            {
                // i is a leaf node, so we return gracefully
                return;
            }
            // i has no right child, but it does have a left child
            // This is the only path to take
            current_child = left_child;
        }
        // i has both children, so compare their priorities to determine the path
        else if (self->cmp(self->heap[left_child], self->heap[right_child]) >= 0)
        {
            // The left child has a higher or equal priority to the right child. We therefore take its path.
            current_child = left_child;
        }
        else
        {
            // The right child has a higher priority than the left child. We therefore take its path
            current_child = right_child;
        }
        if (self->cmp(self->heap[current_child], self->heap[i]) <= 0) return;
        else
        {
            // Swap the child with the current node if its priority is higher
            swap(self, current_child, i);
            // Continue the iteration on the next layer of the heap
            i = current_child;
        }
    }
}

/**
 * Allocate memory for a priority queue with a maximum capacity of size
 *
 * @param size the maximum size of the queue
 * @param comparator a cmp function responsible for prioritizing one element over the other
 * @return a handle to a PQueue object
 */
PQueue_t *priority_queue_new(uint64_t size, priority_queue_comparator comparator)
{
    PQueue_t *queue = malloc(sizeof(PQueue_t));
    queue->heap = calloc(size, sizeof(void*));
    queue->size = size;
    queue->cmp = comparator;
    queue->next = 1;
    return queue;
}

/**
 * Returns a flag indicating whether self is empty or not
 *
 * @param self the queue handle
 * @return 1 if it is empty, 0 otherwise
 */
uint8_t priority_queue_is_empty(PQueue_t *self)
{
    // A priority queue is empty only if the index of the next element is the index of the root note, that is, the index 1.
    // Note that the starting index is 1 and not 0.
    return self->next == 1;
}

/**
 * Returns a flag indicating whether self is full or not
 *
 * @param self the queue handle
 * @return 1 if it is full, 0 otherwise
 */
uint8_t priority_queue_is_full(PQueue_t *self)
{
    return self->next == self->size;
}

/**
 * Insert element into self
 *
 * @param self the queue handle
 * @param element the element to enqueue
 */
void priority_queue_enqueue(PQueue_t *self, void *element)
{
    if (! priority_queue_is_full(self))
    {
        // Add the node to the end of the tree
        // This breaks the ordering invariant of the algorithm!
        self->heap[self->next] = element;
        self->next++;
        // Reconstruct the ordering invariant by pushing the element up in the tree
        sift_up(self, self->next-1);
    }
}

/**
 * Delete the element with the highest priority from self
 *
 * @param self the queue handle
 * @return the data element of the dequeued node
 */
void *priority_queue_dequeue(PQueue_t *self)
{
    if (priority_queue_is_empty(self)) return NULL;
    else
    {
        void* prev_root = self->heap[1];
        // Swap the last element with the root
        // This effectively dequeues the root element whilst preserving the tree structure of the heap
        swap(self, 1, self->next-1);
        self->next--;
        if (self->next > 1) sift_down(self, 1);
        return prev_root;
    }
}

/**
 * Deletes the element at position idx from the queue
 *
 * @param self the queue handle
 * @param idx the position of the element to delete
 */
void priority_queue_delete(PQueue_t *self, size_t idx)
{
    // The invariant is broken here
    swap(self, idx, self->next-1);
    self->next--;
    if (idx < self->next)
    {
        // Determine the parent of the node that broke the invariant
        size_t parent = idx/2;
        // If the parent's priority is lower than the node, sift up to restore the invariant
        if (parent >= 1 && self->cmp(self->heap[parent], self->heap[idx]) == -1)
        {
            sift_up(self, idx);
        }
        // If the parent's priority is higher than this node's, sift down to restore the invariant
        else
        {
            sift_down(self, idx);
        }
    }
}

/**
 * Frees the memory chunks used by self
 *
 * @param self the priority queue handle
 */
void priority_queue_destroy(PQueue_t *self)
{
    if (self)
    {
        if (self->heap)
        {
            free(self->heap);
        }
        free(self);
    }
}