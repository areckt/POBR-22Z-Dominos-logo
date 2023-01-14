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

const double RED_NORM1_MODEL = 0.201113;
const double RED_NORM1_THRESHOLD = 0.05;
const double RED_NORM2_MODEL = 4.72441e-07;
const double RED_NORM2_THRESHOLD = 0.1;
const double RED_NORM3_MODEL = 8.05831e-07;
const double RED_NORM3_THRESHOLD = 0.1;
const double RED_NORM4_MODEL = 5.85783e-08;
const double RED_NORM4_THRESHOLD = 0.1;
const double RED_NORM5_MODEL = -1.15908e-14;
const double RED_NORM5_THRESHOLD = 0.1;
const double RED_NORM6_MODEL = -4.02584e-11;
const double RED_NORM6_THRESHOLD = 0.1;
const double RED_NORM7_MODEL = 0.0101115;
const double RED_NORM7_THRESHOLD = 0.006;
const double RED_WH_MODEL = 1.0;
const double RED_WH_THRESHOLD = 0.5;

const double BLUE_NORM1_MODEL = 0.229965;
const double BLUE_NORM1_THRESHOLD = 0.05;
const double BLUE_NORM2_MODEL = 0.000363102;
const double BLUE_NORM2_THRESHOLD = 0.1;
const double BLUE_NORM3_MODEL = 1.05411e-06;
const double BLUE_NORM3_THRESHOLD = 0.1;
const double BLUE_NORM4_MODEL = 5.80308e-08;
const double BLUE_NORM4_THRESHOLD = 0.1;
const double BLUE_NORM5_MODEL = 9.84522e-15;
const double BLUE_NORM5_THRESHOLD = 0.1;
const double BLUE_NORM6_MODEL = -1.10566e-09;
const double BLUE_NORM6_THRESHOLD = 0.1;
const double BLUE_NORM7_MODEL = 0.0131302;
const double BLUE_NORM7_THRESHOLD = 0.005;
const double BLUE_WH_MODEL = 1.0;
const double BLUE_WH_THRESHOLD = 0.5;

const double WHITE_NORM1_MODEL = 0.159155;
const double WHITE_NORM1_THRESHOLD = 0.02;
const double WHITE_NORM2_MODEL = 1.0888e-08;
const double WHITE_NORM2_THRESHOLD = 0.3;
const double WHITE_NORM3_MODEL = 1.153e-08;
const double WHITE_NORM3_THRESHOLD = 0.3;
const double WHITE_NORM4_MODEL = 1.04152e-15;
const double WHITE_NORM4_THRESHOLD = 0.3;
const double WHITE_NORM5_MODEL = -3.43534e-27;
const double WHITE_NORM5_THRESHOLD = 0.3;
const double WHITE_NORM6_MODEL = -1.03342e-19;
const double WHITE_NORM6_THRESHOLD = 0.3;
const double WHITE_NORM7_MODEL = 0.00633255;
const double WHITE_NORM7_THRESHOLD = 0.002;
const double WHITE_WH_MODEL = 1.0;
const double WHITE_WH_THRESHOLD = 0.5;