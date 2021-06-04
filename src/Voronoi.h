//
// Created by denko on 5/7/2021.
//

#ifndef VORONOI_VORONOI_H
#define VORONOI_VORONOI_H

#include <stddef.h>
#include "Point.h"
#include "DCEL.h"

typedef struct CircleEvent;

typedef struct Arc {
    Point_t_ptr site;
    struct CircleEvent *circle_event; // This is where the arc will disappear. The lowest point of the circle
} Voronoi_Arc_t;

typedef Voronoi_Arc_t* Voronoi_Arc_ptr_t;

typedef struct Breakpoint {
    Point_t_ptr left_site;
    Point_t_ptr right_site;
    DCEL_HalfEdge_ptr_t half_edge;
} Voronoi_Breakpoint_t;

typedef Voronoi_Breakpoint_t* Voronoi_Breakpoint_ptr_t;

typedef struct SiteEvent {
    Point_t_ptr site;
} Voronoi_SiteEvent_t;

typedef Voronoi_SiteEvent_t* Voronoi_SiteEvent_ptr_t;

typedef struct CircleEvent {
    Point_t_ptr circle_point; // The lowest point of the circle
    Voronoi_Arc_ptr_t arc; // The arc that will dissapear in this event
} Voronoi_CircleEvent_t;

typedef Voronoi_CircleEvent_t* Voronoi_CircleEvent_ptr_t;

typedef struct Event {
    uint8_t is_circle_event;
    union {
        Voronoi_CircleEvent_ptr_t circle_event;
        Voronoi_SiteEvent_ptr_t site_event;
    };
} Voronoi_Event_t;

typedef Voronoi_Event_t* Voronoi_Event_ptr_t;

/**
 * Computes the Voronoi diagram for a set of points
 *
 * @param points the points array
 * @param count the number of points inside the array
 * @return a doubly-connected edge list representing the diagram
 */
DCEL_t voronoi_diagram(Point_t_ptr *points, size_t count);



#endif //VORONOI_VORONOI_H
