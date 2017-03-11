#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "renderer.h"
#include "camera.h"
#include "ofxRay.h"
#include "scene.h"

class ofApp : public ofBaseApp {

public:

	const float MaxX = 1000, MaxY = 1000, MaxZ = 1000;
	const float MinX = -1000, MinY = -1000, MinZ = -1000;

	renderer * rend;
	scene * scn;

	ofApp();

	void setup();
	void update();
	void draw();

	void exit();

	~ofApp();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseMovedGui(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	bool cursorIsInControl(int x, int y);


private:
	
	camera *cam;

	bool isListenersUnlocked;

	ofxPanel gui;
	ofxPanel cameraMenu;
	ofxButton btnSelect;
	ofxButton btnDrawPrimitive;
	ofxButton btnExit;

	ofParameterGroup groupProperty;
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

#pragma region "Primitives"

	ofParameterGroup groupPrimitiveType;
	ofParameter<bool> primType2D;
	ofParameter<bool> primType3D;
	ofParameter<bool> primTypeSphere;
	ofParameter<bool> primTypeCube;

	ofParameterGroup groupPrimitivePosition3D;
	ofParameterGroup groupPrimitivePosition2D;
	ofParameter<float> primPosX;
	ofParameter<float> primPosY;
	ofParameter<float> primPosZ;

	ofParameterGroup groupPrimitiveSizeCube2D;
	ofParameterGroup groupPrimitiveSizeCube3D;
	ofParameterGroup groupPrimitiveSizeSphere;
	ofParameter<float> primSizeHeight;
	ofParameter<float> primSizeWidth;
	ofParameter<float> primSizeDepth;
	ofParameter<float> primSizeRadius;

#pragma endregion

	void initGroups();

	void initPosition();
	void initDimension();
	void initColors();
	void setColors();

	float colorParameterChanged(float & value);
	
	void initPrimitives();

	void setupGui(); 
	void setupCameraMenu();

	void btnSelectClicked();
	void btnDrawPrimitiveClicked();
	void btnDrawClicked();
	void btnExitClicked();

	void primDim2DChanged(bool& value);
	void primDim3DChanged(bool& value);
	void primTypeCubeChanged(bool& value);
	void primTypeSphereChanged(bool& value);

	void updateKeys();

	bool isKeyPressUp;
	bool isKeyPressDown;
	bool isKeyPressLeft;
	bool isKeyPressRight;
	bool isKeyPressPageUp;
	bool isKeyPressPageDown;

};
