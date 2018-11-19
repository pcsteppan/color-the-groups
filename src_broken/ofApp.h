#pragma once

#include "ofMain.h"
#include <iostream>

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		// You will notice more errors the larger the GRID_SIZE!
		static const int GRID_SIZE = 32;
		static const int TILE_SIZE = 256 / GRID_SIZE;
		int group_count;

		bool destroyArray;
		bool img[GRID_SIZE][GRID_SIZE];
		int** groupPtrs[GRID_SIZE][GRID_SIZE];
};
