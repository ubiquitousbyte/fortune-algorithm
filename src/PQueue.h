/**
 * Implemented by Atanas Denkov
 *
 * The implementation is inspired by Frank Pfenning's beautiful lecture notes on
 * "Principles of Imperative Computation"
 * http://jamiemorgenstern.com/teaching/su-122/lectures/14
 */

#ifndef VORONOI_PQUEUE_H
#define VORONOI_PQUEUE_H
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/**
 * A comparator function responsible for prioritizing one element over the other
 * Returns:
 *  -1 if first has a lower priority than second
 *   0 if both have the same priority
 *   1 if first has a higher priority than second
 */
typedef int8_t (*priority_queue_comparator)(void* first, void* second);

/**
 * A bounded priority queue implemented as a heap tree
 * The two main invariants of the queue are:
 * The key of each node in the tree is less or equal to all of its children's keys.
 * The insertion order follows the breadth-first traversal method.
 *
 * This data structure allows us to insert and delete n elements with O(n log n) time complexity
 * Peeking comes at a negligible cost of O(1), since the operation simply looks up the root node of the heap tree.
 *
 * NOTE: We use this data structure as an event queue that stores site and circle events of a Voronoi diagram.
 * The algorithm to compute the Voronoi diagram requires us to delete elements from the queue which are not at the root node.
 * This means that the standard interface of a priority queue won't cut it. We need  an additional operation that allows us to
 * remove elements from the queue at an arbitrary index provided by the user! We implement this operation and call it 'delete'.
 * Note that a delete, compared to a dequeue, has a time complexity of O(log n) and not O(1).
 */
typedef struct {
    uint64_t size; // The size of the underlying heap
    uint64_t next; // Index where the next element should be added
    priority_queue_comparator cmp; // A comparator function that determines the priority of the heap nodes
    void **heap; // An efficient representation of a heap tree
} PQueue_t;

/**
 * Allocate memory for a priority queue with a maximum capacity of size
 *
 * @param size the maximum size of the queue
 * @param comparator a cmp function responsible for prioritizing one element over the other
 * @return a handle to a PQueue object
 */
PQueue_t *priority_queue_new(uint64_t size, priority_queue_comparator comparator);

/**
 * Returns a flag indicating whether self is empty or not
 *
 * @param self the queue handle
 * @return 1 if it is empty, 0 otherwise
 */
uint8_t priority_queue_is_empty(PQueue_t *self);

/**
 * Returns a flag indicating whether self is full or not
 *
 * @param self the queue handle
 * @return 1 if it is full, 0 otherwise
 */
uint8_t priority_queue_is_full(PQueue_t *self);

/**
 * Insert element into self
 *
 * @param self the queue handle
 * @param element the element to enqueue
 */
void priority_queue_enqueue(PQueue_t *self, void *element);

/**
 * Delete the element with the highest priority from self
 *
 * @param self the queue handle
 * @return the data element of the dequeued node
 */
void *priority_queue_dequeue(PQueue_t *self);

/**
 * Deletes the element at position idx from the queue
 *
 * @param self the queue handle
 * @param idx the position of the element to delete
 */
void priority_queue_delete(PQueue_t *self, size_t idx);

/**
 * Frees the memory chunks used by self
 *
 * @param self the priority queue handle
 */
void priority_queue_destroy(PQueue_t *self);

#endif //VORONOI_PQUEUE_H
