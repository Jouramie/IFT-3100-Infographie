#pragma once
#include "ofMain.h"
#include "ofxRay.h"
#include "ofxOpenCv.h"
#include "primitive3d.h"
#include <limits>
#include "scene.h"
#include "primitive2d.h"


class renderer
{
public:

	renderer();

	ofColor background;

	bool isCameraMoveLeft;
	bool isCameraMoveRight;
	bool isCameraMoveUp;
	bool isCameraMoveDown;
	bool isCameraMoveForward;
	bool isCameraMoveBackward;

	void setup();
	void update();
	void draw();
	void imageExport(const string name, const string extension) const;
	void chekFilters();
	//2D primitives
	void createSquare(float x, float y, float width, float height);
	void createSquare(float x, float y, float width, float height, ofColor fillColor, ofColor strokeColor);
	void createCircle(float x, float y, float r1, float r2);
	void createCircle(float x, float y, float r1, float r2, ofColor fillColor, ofColor strokeColor);
	void createLine(float x, float y, float xDelta, float yDelta);
	void createLine(float x, float y, float xDelta, float yDelta, ofColor fillColor);
	void createTriangle(float x1, float y1, float x2, float y2, float x3, float y3);
	void createTriangle(float x1, float y1, float x2, float y2, float x3, float y3, ofColor fillColor, ofColor strokeColor);
	void createPoint(float x, float y, float radius);
	void createPoint(float x, float y, float radius, ofColor fillColor, ofColor strokeColor);

	void createCube(int x, int y, int z, int w, int h, int d);
	void createCube(int x, int y, int z, int w, int h, int d, ofColor fillCol);
	void createSphere(int x, int y, int z, int sizeX, int sizeY, int sizeZ);
	void createSphere(int x, int y, int z, int sizeX, int sizeY, int sizeZ, ofColor color);
	void clearPrimitives();
	void changeWireFrameMode();
	void changeCameraMode();
	void selectPrimitive(int x, int y, bool shiftHeld);

	void setScene(scene * scn) { this->scn = scn; }

	void addBlur();
	void removeBlur();
	void addInvert();
	void removeInvert();
	void addDilate();
	void removeDilate();

	~renderer();

private:
	void setupCamera();

	//ofEasyCam mainCam;
	bool wireFrame;
	bool blur;
	bool invert;
	bool dilate;
	//int rotate;
	
	float cameraSpeed;
	float cameraDeplacement;

	float timeCurrent;
	float timeElapsed;
	float timeLast;

	ofVec3f cameraPosition;
	ofVec3f cameraTarget;

	scene * scn;
	ofCamera camera;

	std::list<ofRay> rays;

	ofxCvColorImage filter;
	ofImage sceneImg;
	ofPixels scenePixels;
};

