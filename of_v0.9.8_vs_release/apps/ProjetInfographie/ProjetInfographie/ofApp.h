#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "renderer.h"

class ofApp : public ofBaseApp {

public:

	const float MaxX = 2000, MaxY = 1000, MaxZ = 1000;
	const float MinX = 0, MinY = 0, MinZ = -1000;

	renderer * rend;

	ofApp();

	void setup();
	void update();
	void draw();

	void exit();

	~ofApp();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);


private:

	bool isListenersUnlocked;

	ofxPanel gui;
	ofxButton btn2D;
	ofxButton btn3D;
	ofxButton btnSelect;
	ofxButton btnProperty;
	ofxButton btnDraw;
	ofxButton btnExit;

	int i = 0;

	bool isPropertiesDisplay;
	bool is2dDisplay;
	bool is3dDisplay;
	

	ofParameterGroup groupPosition;
	ofParameter<float> posX;
	ofParameter<float> posY;
	ofParameter<float> posZ;

	ofParameterGroup groupDimension;
	ofParameter<float> height;
	ofParameter<float> width;
	ofParameter<float> depth;
	ofParameter<float> radius;



	ofParameterGroup groupThick;
	ofParameter<float> strokeThickness;

	ofParameterGroup groupStroke;
	ofColor stroke;
	ofParameter<float> strokeHue;
	ofParameter<float> strokeSaturation;
	ofParameter<float> strokeBrightess;
	ofParameter<float> strokeAlpha;

	ofParameterGroup groupFill;
	ofColor fill;
	ofParameter<float> fillHue;
	ofParameter<float> fillSaturation;
	ofParameter<float> fillBrightess;
	ofParameter<float> fillAlpha;

	ofParameterGroup groupBackground;
	ofColor background;
	ofParameter<float> bgHue;
	ofParameter<float> bgSaturation;
	ofParameter<float> bgBrightess;

	void initGroups();

	void initPosition();
	void initDimension();
	void initColors();
	void setColors();

	void setupGui();
	void displayProperties();
	void display2D();
	void display3D();

	void btnSelectClicked();
	void btnPropertyClicked();
	void btn2DClicked();
	void btn3DClicked();
	void btnDrawClicked();
	void btnExitClicked();

};
