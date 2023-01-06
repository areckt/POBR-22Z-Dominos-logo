#pragma once
const int RESIZED_WIDTH = 800;
const int RESIZED_HEIGHT = 600;
const int SEGMENT_SIZE_THRESHOLD = 40;

enum ScalingAlgorithmType {
	NearestNeighbour,
	Bilinear
};

enum Color { 
	WHITE, 
	BLUE, 
	RED, 
	OTHER 
};

enum State {
	ADDED,
	VISITED,
	UNVISITED,
	MISSED
};
