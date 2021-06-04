//
// Created by denko on 4/24/2021.
//

#include <assert.h>
#include "PQueue.c"
#include <stdio.h>

static int8_t min_heap_comparator(void *first, void *second)
{
    int32_t first_in = *(int32_t *) first;
    int32_t second_in = *(int32_t *) second;
    if (first_in < second_in) return 1;
    else if (first_in > second_in) return -1;
    else return 0;
}

void test_priority_queue_init()
{
    PQueue_t *queue = priority_queue_new(10, min_heap_comparator);
    assert(queue != NULL);
    assert(queue->heap != NULL);
    assert(priority_queue_is_empty(queue));
    priority_queue_destroy(queue);
}

void test_priority_queue_enqueue()
{
    PQueue_t *queue = priority_queue_new(10, min_heap_comparator);
    assert(queue != NULL);
    assert(queue->heap != NULL);
    assert(priority_queue_is_empty(queue));
    int32_t first_node = 5;
    int32_t second_node = 11;
    int32_t third_node = 44;
    int32_t fourth_node = 39;
    int32_t fifth_node = 1;
    int32_t sixth_node = 11;
    priority_queue_enqueue(queue, &first_node);
    priority_queue_enqueue(queue, &second_node);
    priority_queue_enqueue(queue, &third_node);
    priority_queue_enqueue(queue, &fourth_node);
    priority_queue_enqueue(queue, &fifth_node);
    priority_queue_enqueue(queue, &sixth_node);
    assert(*(int32_t *)queue->heap[1] == 1);
    assert(*(int32_t *)queue->heap[2] == 5);
    assert(*(int32_t *)queue->heap[3] == 11);
    assert(*(int32_t *)queue->heap[4] == 39);
    assert(*(int32_t *)queue->heap[5] == 11);
    assert(*(int32_t *)queue->heap[6] == 44);
    priority_queue_destroy(queue);
}

void test_priority_queue_dequeue()
{
    PQueue_t *queue = priority_queue_new(10, min_heap_comparator);
    assert(queue != NULL);
    assert(queue->heap != NULL);
    assert(priority_queue_is_empty(queue));
    int32_t first_node = 5;
    int32_t second_node = 11;
    int32_t third_node = 44;
    int32_t fourth_node = 39;
    int32_t fifth_node = 1;
    int32_t sixth_node = 11;
    priority_queue_enqueue(queue, &first_node);
    priority_queue_enqueue(queue, &second_node);
    priority_queue_enqueue(queue, &third_node);
    priority_queue_enqueue(queue, &fourth_node);
    priority_queue_enqueue(queue, &fifth_node);
    priority_queue_enqueue(queue, &sixth_node);

    priority_queue_dequeue(queue);
    assert(*(int32_t *)queue->heap[1] == 5);
    assert(*(int32_t *)queue->heap[2] == 11);
    assert(*(int32_t *)queue->heap[3] == 11);
    assert(*(int32_t *)queue->heap[4] == 39);
    assert(*(int32_t *)queue->heap[5] == 44);

    priority_queue_dequeue(queue);
    assert(*(int32_t *)queue->heap[1] == 11);
    assert(*(int32_t *)queue->heap[2] == 39);
    assert(*(int32_t *)queue->heap[3] == 11);
    assert(*(int32_t *)queue->heap[4] == 44);

    priority_queue_dequeue(queue);
    assert(*(int32_t *)queue->heap[1] == 11);
    assert(*(int32_t *)queue->heap[2] == 39);
    assert(*(int32_t *)queue->heap[3] == 44);

    priority_queue_dequeue(queue);
    assert(*(int32_t *)queue->heap[1] == 39);
    assert(*(int32_t *)queue->heap[2] == 44);

    priority_queue_dequeue(queue);
    assert(*(int32_t *)queue->heap[1] == 44);

    priority_queue_dequeue(queue);
    assert(priority_queue_is_empty(queue));
    priority_queue_destroy(queue);
}

int main(int argc, char *argv[])
{
    test_priority_queue_init();
    test_priority_queue_enqueue();
    test_priority_queue_dequeue();
}