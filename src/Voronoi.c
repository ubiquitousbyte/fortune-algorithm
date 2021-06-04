//
// Created by denko on 5/7/2021.
//

#include "Voronoi.h"
#include "PQueue.h"
#include "AVLTree.h"

static Voronoi_SiteEvent_ptr_t voronoi_site_event_new(Point_t_ptr site)
{
    Voronoi_SiteEvent_ptr_t event = malloc(sizeof(Voronoi_SiteEvent_t));
    event->site = site;
    return event;
}

static Voronoi_CircleEvent_ptr_t voronoi_circle_event_new(Point_t_ptr circle_point)
{
    Voronoi_CircleEvent_ptr_t event = malloc(sizeof(Voronoi_CircleEvent_t));
    event->circle_point = circle_point;
    event->arc = NULL;
    return event;
}

static Voronoi_Event_ptr_t voronoi_event_new(Point_t_ptr point, uint8_t is_circle_event)
{
    Voronoi_Event_ptr_t event = malloc(sizeof(Voronoi_Event_t));
    event->is_circle_event = is_circle_event;
    if (is_circle_event)
    {
        Voronoi_CircleEvent_ptr_t circle_event = voronoi_circle_event_new(point);
        event->circle_event = circle_event;
        event->site_event = NULL;
    }
    else
    {
        Voronoi_SiteEvent_ptr_t site_event = voronoi_site_event_new(point);
        event->site_event = site_event;
        event->circle_event = NULL;
    }
    return event;
}

static uint64_t voronoi_event_get_priority(Voronoi_Event_ptr_t event)
{
    uint64_t event_key;
    if (event->is_circle_event)
    {
        event_key = event->circle_event->circle_point->y;
    }
    else
    {
        event_key = event->site_event->site->y;
    }
    return event_key;
}

static int8_t voronoi_event_queue_comparator(void *first, void *second)
{
    uint64_t first_priority = voronoi_event_get_priority((Voronoi_Event_ptr_t) first);
    uint64_t second_priority = voronoi_event_get_priority((Voronoi_Event_ptr_t) second);
    if (first_priority > second_priority) return 1;
    if (first_priority < second_priority) return -1;
    return 0;
}

static PQueue_t* voronoi_event_queue_init(Point_t_ptr *points, size_t count)
{
    PQueue_t *queue = priority_queue_new(count, voronoi_event_queue_comparator);
    for (int i = 0; i < count; i++)
    {
        Voronoi_Event_ptr_t site_event = voronoi_event_new(points[i], 0);
        priority_queue_enqueue(queue, (void *) site_event);
    }
    return queue;
}

void voronoi_process_site_event(Point_t_ptr site);

void voronoi_process_circle_event(Voronoi_Arc_ptr_t arc);

/**
 * Computes the Voronoi diagram for a set of points
 *
 * @param points the points array
 * @param count the number of points inside the array
 * @return a doubly-connected edge list representing the diagram
 */
DCEL_t voronoi_diagram(Point_t_ptr *points, size_t count)
{
    PQueue_t *event_queue = voronoi_event_queue_init(points, count);
    while(! priority_queue_is_empty(event_queue))
    {
        Voronoi_Event_ptr_t event = (Voronoi_Event_ptr_t) priority_queue_dequeue(event_queue);
        if (! event->is_circle_event)
        {
            voronoi_process_site_event(event->site_event->site);
        }
        else
        {
            voronoi_process_circle_event(event->circle_event->arc);
        }
    }

    priority_queue_destroy(event_queue);

}