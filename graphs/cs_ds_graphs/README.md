

A simple application that displays a graph, generates connections using a triangulation and calculates a path from 
one node to another.

Files:

	* mainwindow.cpp, mainwindow.h -> base UI for the rest of the application. Has callback for some methods, like save and open.
	* gldrawingarea.cpp, gldrawingarea.h ->  the actual scene. It has all related to the scene, graph construction and save/open features.
			methods: 
				. nextAnimationFrame -> Here the collision between nodes, edges and obstacles takes place
				. paintEvent -> main drawing function, to draw all the scene
				. initGraphNodes -> initializes the nodes randomly in the scene
				. initGraphConnections -> initializes the graph connections using the delaunay triangulation
				. mouseMoveEvent, mousePressEvent -> mouse callbacks used to handle some features, like selecting nodes, placing obstacle, etc.
				. placeObstacle -> callback used when requesting an obstacle
				. saveGraph -> saves the graph into a .txt file
				. openGraph -> opens a graph from a .txt file
				. calculatePath -> executes A* search to calculate a path from one node to another

	* lib/delaunay.c , lib/delaunay.h -> delaunay library from github : https://github.com/eloraiby/delaunay
	* graph/ -> the graph datastructure implemented for the assignment

Will be porting to opengl to ensure easy compilation.