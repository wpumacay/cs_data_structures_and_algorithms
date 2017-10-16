#pragma once

#include <cstdlib>


// render to use
//#define RENDER_QT 1



// NUMBER OF RANDOM NODES IN THE GRAPH
#define NUM_GRAPH_NODES 100000

#define MAX_PARALLEL_REQUESTS 3

// QT defines
#define QT_GRAPH_NODE_SIZE 1
//#define QT_DRAW_TEXT 1


// some GL defines
#define CAM_SPEED_X 500.0f
#define CAM_SPEED_Y 500.0f
// PARAMETER FOR THE COLLISION DETECTION WITH A NODE
#define RANGE_PICK 10.0f

//#define USE_LIGHT_MODE_ONLY 1
#define USE_SAVED_GRAPH 1
#define USE_SAVED_GRAPH_PATH "res/10000points.data"
#define USE_CLUSTERING_BFS 1
#define USE_PARALLEL_REQUESTS 1
#ifdef USE_PARALLEL_REQUESTS
	#define MAX_PARALLEL_REQUESTS 3
#endif

#define USE_BATCH_RENDER 1

// Some other useful stuff
#define RANDOM() ( rand() / ( float )RAND_MAX )