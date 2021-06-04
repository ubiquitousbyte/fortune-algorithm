//
// Created by nasdenkov on 4/23/2021.
//

#ifndef VORONOI_POINT_H
#define VORONOI_POINT_H
#include <stdint.h>

// A pair (x, y)
typedef struct {
    uint64_t x;
    uint64_t y;
} Point_t;

typedef Point_t* Point_t_ptr;

/**
 * Initializes a point with the given x and y coordinates
 *
 * @param self the point handle
 * @param x the x coordinate
 * @param y the y coordinate
 */
void point_init(Point_t *self, uint64_t x, uint64_t y);

/**
 * Frees the memory block used by self
 *
 * @param self the point handle to be freed
 */
void point_destroy(Point_t *self);

/**
 * Computes the euclidean distance between point self and other
 *
 * @param self the point handle
 * @param q the other point handle
 * @return the euclidean distance between the points
 */
double point_euclidean_distance(Point_t *self, Point_t *other);

#endif //VORONOI_POINT_H
