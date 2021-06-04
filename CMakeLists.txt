cmake_minimum_required(VERSION 3.16)
project(voronoi C)
set(CMAKE_C_STANDARD 99)

set(OPENMP "-fopenmp")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")
# Live
add_executable(voronoi src/main.c src/Point.h src/Point.c src/PQueue.h src/PQueue.c src/DCEL.h src/AVLTree.c src/AVLTree.h src/Voronoi.c src/Voronoi.h)
target_link_libraries(voronoi -lm)

# Test
add_executable(voronoi_queue_test src/PQueue_test.c)
add_executable(voronoi_avl_tree_test src/AVLTree_test.c)
target_link_libraries(voronoi_queue_test -lm)
