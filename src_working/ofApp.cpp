#include "ofApp.h"

#include <iostream>
#include <set>
#include <memory>
#include <bitset>

class Node
{
public:
	Node(int thePosition, Node* theLink)
		: position(thePosition), link(theLink) {};

	Node* getLink() const { return link; }
	int getPosition() const { return position; }
	void setLink(Node* pointer) { link = pointer; }

private:
	int position;
	Node *link;
};

class NodeList
{
public:
	NodeList(Node* origin)
		: head(origin), tail(origin) {};

	Node* getTail() const { return tail; }
	void setTail(Node* pointer) { tail = pointer; }

	Node* getHead() const { return head; }
	void setHead(Node* pointer) { head = pointer; }

	void combine(NodeList* other) {
		Node* tempTail = other->getTail();
		// assign the link of our tail node to the head node of other
		this->getTail()->setLink((other->getHead()));
		// assign our tail pointer to the tail of other
		this->setTail(tempTail);
	}

	void addNewNodeToEnd(int position) {
		// assign the link of our tail node to a new node with a given value
		this->getTail()->setLink(new Node(position, NULL));
		// assign our tail pointer to the new node
		this->setTail(this->getTail()->getLink());
	}

	bool sameHead(NodeList* other) {
		return (other->getHead() == this->getHead());
	}

private:
	Node *head, *tail;
};

class GroupFinder
{
public:
	GroupFinder(bool* theImage, const int theGridSize)
		: image(theImage), gridSize(theGridSize) {};

	std::vector<std::shared_ptr<NodeList>> findGroups() {
		//NodeList * vertices = new NodeList*[gridSize * gridSize];
		//std::shared_ptr<NodeList[]*> vertices(new NodeList[gridSize * gridSize]*);
		std::vector<std::shared_ptr<NodeList>> vertices (gridSize * gridSize, nullptr);
		std::vector<std::shared_ptr<NodeList>> heads;

		// first head established at first pixel
		vertices[0] = std::shared_ptr<NodeList> (new NodeList(new Node(0, NULL)));
		heads.push_back(vertices[0]);

		// iterates through first row of image
		for (int j = 1; j < gridSize; j++) {
			if (image[j] == image[j - 1])
			{
				vertices[j - 1].get()->addNewNodeToEnd(j);
				vertices[j] = vertices[j - 1];
			}
			else
			{
				//vertices[j] = new NodeList(new Node(j, NULL));
				vertices[j] = std::shared_ptr<NodeList>(new NodeList(new Node(j, NULL)));
				heads.push_back(vertices[j]);
			}
		}


		// iterates through second row downwards
		for (int i = 1; i < gridSize; i++) {
			// first column; first pixel of each row
			// compare north and north east pixels in image
			int self = i * gridSize;
			int n = self - gridSize;
			int ne = n + 1;

			if (image[self] == image[n]) {
				vertices[n].get()->addNewNodeToEnd(self);
				vertices[self] = vertices[n];
			}
			else if (image[self] == image[ne]) {
				vertices[ne].get()->addNewNodeToEnd(self);
				vertices[self] = vertices[ne];
			}
			else {
				//vertices[self] = new NodeList(new Node(self, NULL));
				vertices[self] = std::shared_ptr<NodeList>(new NodeList(new Node(self, NULL)));
				heads.push_back(vertices[self]);
			}

			for (int j = 1; j < gridSize; j++) {
				int self = i * gridSize + j;
				int n = self - gridSize;
				int nw = n - 1;
				int ne = n + 1;
				int w = self - 1;

				if (image[self] == image[ne] && (j + 1) < gridSize) {
					vertices[ne].get()->addNewNodeToEnd(self);
					vertices[self] = vertices[ne];
					if (image[self] == image[nw] && !(vertices[self].get()->sameHead(vertices[nw].get()))) {
						vertices[self].get()->combine(vertices[nw].get());

						heads.erase(std::remove(heads.begin(), heads.end(), vertices[nw]), heads.end());
						// all pointers that used to point to other now point to us
						Node* it = vertices[nw].get()->getHead();
						while (it != NULL) {
							int pos = it->getPosition();
							vertices[pos] = vertices[self];
							it = it->getLink();
						}


					}
					else if (image[self] == image[w] && !(vertices[self].get()->sameHead(vertices[w].get()))) {
						vertices[self].get()->combine(vertices[w].get());

						heads.erase(std::remove(heads.begin(), heads.end(), vertices[w]), heads.end());
						// all pointers that used to point to other now point to us
						Node* it = vertices[w].get()->getHead();
						while (it != NULL) {
							int pos = it->getPosition();
							vertices[pos] = vertices[self];
							it = it->getLink();
						}
						//heads.erase(vertices[w]);
					}
				}
				else if (image[self] == image[n]) {
					vertices[n].get()->addNewNodeToEnd(self);
					vertices[self] = vertices[n];
				}
				else if (image[self] == image[nw]) {
					vertices[nw].get()->addNewNodeToEnd(self);
					vertices[self] = vertices[nw];
				}
				else if (image[self] == image[w]) {
					vertices[w].get()->addNewNodeToEnd(self);
					vertices[self] = vertices[w];
				}
				else {
					vertices[self] = std::shared_ptr<NodeList>(new NodeList(new Node(self, NULL)));
					//vertices[self] = new NodeList(new Node(self, NULL));
					heads.push_back(vertices[self]);
				}
			}
		}

		return heads;
	}

private:
	bool* image;
	const int gridSize;
};

GroupFinder* groupFinder;
const int GRID_SIZE = 64;
const int TILE_SIZE = 512 / GRID_SIZE;
bool myImage[GRID_SIZE * GRID_SIZE];
bool showScanning = false;

//--------------------------------------------------------------
void ofApp::setup(){
}

//--------------------------------------------------------------
void ofApp::update(){
	if (groupFinder)
		delete groupFinder;
	/*
	for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++) {
		//delete image[i];
		myImage[i] = trunc(
			ofNoise(
			(i / GRID_SIZE) / (mouseY + 1),
			(i % GRID_SIZE) / 50.0f,
			mouseX / 100.0f)
		+ 0.5f);
	}
	*/
	for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++) {
		//delete image[i];
		myImage[i] = trunc(
			ofNoise(
			(i / GRID_SIZE) / 10.0f,
				(i % GRID_SIZE) / 12.0f,
				sin(ofGetElapsedTimef() / 90.0f) * 20) * ofMap(mouseY, 0, ofGetHeight(), 0.5, 2)
			+ 0.5f);
	}
	/*
	std::bitset<GRID_SIZE*GRID_SIZE> bar (ofMap(mouseX, 0, ofGetWidth(), 0, pow(2, GRID_SIZE)));

	for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++) {
		//delete image[i];
		myImage[i] = bar[i];
	}
	*/
	groupFinder = new GroupFinder(myImage, GRID_SIZE);
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);
	std::vector<std::shared_ptr<NodeList>> groups = groupFinder->findGroups();

	bool z = false;// mouseY > ofGetHeight() / 2;

	for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++) {
		ofSetColor(myImage[i] * 240, myImage[i] * 240, myImage[i] * 180 + 75);
		int x = i % GRID_SIZE;
		int y = i / GRID_SIZE;
		ofFill();
		ofDrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
	}

	int hue_d = 255.0f / (groups.size());
	int count = 0;
	int totalDrawCalls = 0;
	for (auto group : groups) {
		count++;
		Node* curr = group.get()->getHead();
		int prevX = -1;
		int prevY = -1;
		while (curr != NULL) {
			ofSetColor(
				ofColor::fromHsb(
					count * hue_d + ofRandom(5),
					200 - count * hue_d,
					pow(ofMap(count * hue_d, 0, 255, 1, 0), 2.0) * 255
				)
			);
			ofFill();
			int pos = curr->getPosition();
			if (prevX == -1) {
				prevX = pos % GRID_SIZE;
				prevY = pos / GRID_SIZE;
			}
			int x = pos % GRID_SIZE;
			int y = pos / GRID_SIZE;
			
			ofDrawRectangle(x * TILE_SIZE + 512 + count * z, y * TILE_SIZE + count * z, TILE_SIZE, TILE_SIZE);
			ofSetColor(ofColor::coral);

			if(showScanning)
				ofDrawLine((prevX + 0.5f) * TILE_SIZE, (prevY + 0.5f) * TILE_SIZE, (x + 0.5f) * TILE_SIZE, (y + 0.5f) * TILE_SIZE);
			Node* temp = &(*curr);
			curr = curr->getLink();
			delete temp;

			prevX = x;
			prevY = y;
			totalDrawCalls++;
		}
	}

	

	//cout << "uniques: " << groups.size() << endl;
	//cout << ofGetFrameRate() << endl;
	cout << totalDrawCalls << endl;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
	showScanning = !showScanning;
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
