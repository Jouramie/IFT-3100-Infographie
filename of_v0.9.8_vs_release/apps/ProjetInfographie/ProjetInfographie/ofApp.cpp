#include "ofApp.h"

ofApp::ofApp()
{
	rend = nullptr;
}

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetWindowTitle("Top 10 Album of all time by the rolling stone magazine!");

	rend = new renderer();
	rend->setup();

	ofLog() << "<app::setup>";
}

//--------------------------------------------------------------
void ofApp::update()
{
	rend->update();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	rend->draw();
}

void ofApp::exit()
{
	ofLog() << "<app::exit>";
}

ofApp::~ofApp()
{
	if (nullptr != rend)
		delete rend;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	if (key == ' ')
		rend->imageExport("render", "png");
	else if (key == 'r')
	{
		ofColor c = ofColor(rand() % 256, rand() % 256, rand() % 256);
		rend->createCube(rand() % 492 + 10, rand() % 363 + 10, rand() % 100, 100, c);
	}
	else if (key == 's')
	{
		ofColor c = ofColor(rand() % 256, rand() % 256, rand() % 256);
		rend->createSphere(rand() % 492 + 10, rand() % 363 + 10, rand() % 100, 100, c);
	}

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
