#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);

	for (int i = 0; i < GRID_SIZE; i++) {
		group_indices[i] = i;
	}

}

//--------------------------------------------------------------
void ofApp::update() {
	group_count = 0;// int(ofRandom(100));


	for (int i = 0; i < GRID_SIZE; i++) {
		for (int j = 0; j < GRID_SIZE; j++) {
			// 500, 405
			img[i][j] = (int)(ofNoise(float(i) / 20.0f, float(j) / 10.0f, mouseX / 2.0f) + 0.5f);
			groupPtrs[i][j] = new int*(new int(0));
		}
	}

	for (int i = 0; i < GRID_SIZE; i++) {
		for (int j = 0; j < GRID_SIZE; j++) {

			bool self = img[i][j];
			
			if (i == 0) {
				// first row
				if (j == 0) {
					**groupPtrs[i][j] = group_count++;
				}
				else {
					if (img[0][j - 1] ^ img[0][j]) {
						// previous pixel is different, so you have a new route to a new group
						**groupPtrs[i][j] = group_count++;
					}
					else {
						// previous pixel is the same, so you copy the route
						groupPtrs[0][j] = groupPtrs[0][j - 1];
					}
				}
			}
			else {
				if (j == 0) {
					// first column, from the second row down
					// check north and north-east
					// o o o o
					// ^ ^
					// |/
					// o o o o
					// o o o o
					// o o o o

					// these bools are true if the pixels north or north-east match yourself
					bool n = !(self ^ img[i - 1][0]);
					bool ne = !(self ^ img[i - 1][1]);

					if (ne) {
						// north-east matches, so you copy the route
						groupPtrs[i][0] = groupPtrs[i - 1][1];
					}
					else if (n) {
						// north matches, so you copy the route
						groupPtrs[i][0] = groupPtrs[i - 1][0];
					}
					else {
						// neither match, so you belong to a new group for now
						**groupPtrs[i][j] = group_count++;
					}
				}
				else {
					// not the first row or column, we're in the thick of it now!
					// check difference between self, north-east, north, north-west, and west

					// these bools are set to true if they match the current pixel
					bool w = !(self ^ img[i][j - 1]);
					bool nw = !(self ^ img[i - 1][j - 1]);
					bool n = !(self ^ img[i - 1][j]);
					bool ne = false;
					// if we're in the last column than ne must be false
					if (j != GRID_SIZE - 1) {
						ne = !(self ^ img[i - 1][j + 1]);
					}

					bool belonging = w | nw | n | ne;

					// this line is unnessecary, but reads well
					if (belonging) {
						if (ne) {
							groupPtrs[i][j] = groupPtrs[i - 1][j + 1];

							// re-route groups to route of current group
							if (w) {
								*groupPtrs[i][j - 1] = *groupPtrs[i][j];
							}
							if (nw) {
								*groupPtrs[i - 1][j - 1] = *groupPtrs[i][j];
							}
						}
						else if (n) {
							groupPtrs[i][j] = groupPtrs[i - 1][j];
							
							if (w) {
								*groupPtrs[i][j - 1] = *groupPtrs[i][j];
							}
						}
						else if (nw) {
							groupPtrs[i][j] = groupPtrs[i - 1][j - 1];
						}
						else if (w) {
							groupPtrs[i][j] = groupPtrs[i][j - 1];
						}
					}
					else {
						**groupPtrs[i][j] = group_count++;//&new_route;
					}
				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(255);

	int x_off = GRID_SIZE * TILE_SIZE;

	for (int i = 0; i < GRID_SIZE; i++) {
		for (int j = 0; j < GRID_SIZE; j++) {
			// draws representation of img
			ofSetColor(255 * img[i][j]);
			ofFill();
			ofDrawRectangle(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE);

			//draws edges/XOR relationships
			if (i > 0) {
				ofSetColor(0, 0, 0, 0);
				if (img[i][j] ^ img[i - 1][j]) {
					ofSetColor(255, 0, 255);
				}
				ofFill();
				ofDrawRectangle(j * TILE_SIZE + x_off, i * TILE_SIZE, TILE_SIZE, TILE_SIZE / 2.0);
			}
			if (j > 0) {
				ofSetColor(0, 0, 0, 0);
				if (img[i][j] ^ img[i][j - 1]) {
					ofSetColor(0, 255, 255);
				}
				ofFill();
				ofDrawRectangle(j * TILE_SIZE + x_off, i * TILE_SIZE, TILE_SIZE / 2.0, TILE_SIZE);
			}

			// draws error-ridden group colorizer
			ofSetColor(ofColor::fromHsb(fmod((int(**groupPtrs[i][j])) * 37.0f, 360.0f), 255, fmod((int(**groupPtrs[i][j])) * 37.0f, 255.0f)));
			ofFill();
			ofDrawRectangle(j * TILE_SIZE + x_off * 2, i * TILE_SIZE, TILE_SIZE, TILE_SIZE);
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	// useful for debugging, especially when colors aren't clear
	for (int i = 0; i < GRID_SIZE; i++) {
		for (int j = 0; j < GRID_SIZE; j++) {
			cout << std::hex << **groupPtrs[i][j];
			cout << " ";
		}
		cout << endl;
	}
	cout << endl;
	cout << endl;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	cout << mouseX << endl;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
