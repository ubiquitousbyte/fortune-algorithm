/**
 * Implemented by Atanas Denkov
 *
 * This implementation is inspired by multiple sources:
 * "Theory of Computational Algorithms" by Prof. E. Welzl and Prof. B Gärtner
 * https://www.ti.inf.ethz.ch/ew/courses/CG12/lecture/Chapter%205.pdf
 *
 * "Computational Geometry: Algorithms and Applications" by M. de Berg, O. Cheong, M. van Krevald and M. Overmars
 * Chapters 2 and 7
 */

#ifndef VORONOI_DCEL_H
#define VORONOI_DCEL_H
#include "Point.h"

struct DCEL_HalfEdge;

typedef struct {
    Point_t position; // The position of the vertex in the space
    struct DCEL_HalfEdge *inc_edge; // An arbitrary half-edge that is incident to this vertex
} DCEL_Vertex_t;

typedef DCEL_Vertex_t *DCEL_Vertex_ptr_t;

typedef struct {
    struct DCEL_HalfEdge *inc_edge; // An arbitrary half-edge that is incident to this face on its outer boundary
    struct DCEL_HalfEdge **inner_edges; // Holds a pointer to a half-edge for each hole in the face
} DCEL_Face_t;

typedef DCEL_Face_t *DCEL_Face_ptr_t;

typedef struct DCEL_HalfEdge {
    DCEL_Vertex_ptr_t origin; // The origin vertex the edge is pointing to. The target vertex is twin->origin.
    DCEL_Face_ptr_t inc_face; // A pointer to the incident face
    struct DCEL_HalfEdge *twin; // A pointer to the twin half-edge. That is, the same edge in the reverse direction
    struct DCEL_HalfEdge *next; // A pointer to the half-edge following self along the face boundary
    struct DCEL_HalfEdge *prev; // A pointer to the half-edge preceding self along the face boundary
} DCEL_HalfEdge_t;

typedef DCEL_HalfEdge_t *DCEL_HalfEdge_ptr_t;

/**
 * A doubly connected edge list that can be used to represent complex geometrical shapes as a sequence of interconnected
 * line segments. An example for such a geometrical shape is the Voronoi diagram.
 */
typedef struct {
    DCEL_Vertex_ptr_t *vertices;
    DCEL_HalfEdge_ptr_t *half_edges;
    DCEL_Face_ptr_t *faces;
} DCEL_t;


#endif //VORONOI_DCEL_H
