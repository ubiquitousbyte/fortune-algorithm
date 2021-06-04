//
// Created by denko on 4/23/2021.
//
#include <math.h>
#include <stdlib.h>
#include "Point.h"

/**
 * Initializes a point with the given x and y coordinates
 *
 * @param self the point handle
 * @param x the x coordinate
 * @param y the y coordinate
 */
void point_init(Point_t *self, uint64_t x, uint64_t y)
{
    self->x = x;
    self->y = y;
}

/**
 * Frees the memory block used by self
 *
 * @param self the point handle to be freed
 */
void point_destroy(Point_t *self)
{
    if (self)
        free(self);
}

/**
 * Computes the euclidean distance between point self and other
 *
 * @param self the point handle
 * @param q the other point handle
 * @return the euclidean distance between the points
 */
double point_euclidean_distance(Point_t *self, Point_t *other)
{
    return sqrt(pow( self->x - other->x, 2) + pow( self->y - other->y, 2));
}
