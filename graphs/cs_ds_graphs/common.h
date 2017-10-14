#pragma once

#include <cstdlib>


// render to use
//#define RENDER_QT 1



// NUMBER OF RANDOM NODES IN THE GRAPH
#define NUM_GRAPH_NODES 10000



// QT defines
#define QT_GRAPH_NODE_SIZE 1
//#define QT_DRAW_TEXT 1


// some GL defines
#define CAM_SPEED_X 500.0f
#define CAM_SPEED_Y 500.0f
// PARAMETER FOR THE COLLISION DETECTION WITH A NODE
#define RANGE_PICK 15.0f

#define USE_LIGHT_MODE_ONLY 1
#define USE_SAVED_GRAPH 1


// Some other useful stuff
#define RANDOM() ( rand() / ( float )RAND_MAX )